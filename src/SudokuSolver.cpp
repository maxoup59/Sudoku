//! \file
//! \brief SudokuSolver implementation.
//! \author Mathieu Turcotte

#include <vector>
#include <limits>
#include <deque>
#include <utility>

#include "SudokuSolver.hpp"

using namespace std;

bool DancingLinksSolver::solve(Sudoku& s) {
    Node* cover_matrix_root = build_cover_matrix(s);
    bool solved = solve(cover_matrix_root);
    delete_cover_matrix(cover_matrix_root);
    return solved;
}

DancingLinksSolver::Node* DancingLinksSolver::build_cover_matrix(Sudoku& s) {

    unsigned short s_size = s.size();
    unsigned int s_num_cells = s_size * s_size;
    unsigned short s_region_num_rows = s.region_num_rows();
    unsigned short s_region_num_columns = s.region_num_columns();

    unsigned int cm_num_columns = s_num_cells * 4;
    std::vector<Node*> cm_columns_headers(cm_num_columns);

    // Start the build process with the root.
    Node* cover_matrix_root = new Node;
    cover_matrix_root->down = cover_matrix_root;
    cover_matrix_root->up = cover_matrix_root;
    cover_matrix_root->left = cover_matrix_root;
    cover_matrix_root->right = cover_matrix_root;

    // Fill cm_col_headers with the header nodes.
    Node* predecessor = cover_matrix_root;

    for (unsigned int i = 0; i < cm_columns_headers.size(); ++i) {
        Node* header = new Node;

        header->up = header->down = header;
        header->left = predecessor;
        header->right = cover_matrix_root;
        header->payload.header.id = i;
        header->payload.header.count = 0;

        cover_matrix_root->left = header;
        predecessor->right = header;

        cm_columns_headers[i] = header;
        predecessor = header;
    }

    // In order to fill the cover matrix, go over each cell of the grid.
    for (unsigned int s_line = 0; s_line < s_size; s_line++) {
        for (unsigned int s_col = 0; s_col < s_size; s_col++) {

            // Retrieve the cell at position s_line, s_col.
            Sudoku::Cell& cell = s.cell(s_line, s_col);

            unsigned short start = 0;
            unsigned short stop = s_size;

            if (cell.is_set()) {
                start = cell.get_value();
                stop = start + 1;
            }

            // For each value in the cell domain, add a row to the cover matrix.
            for (unsigned int value = start; value < stop; value++) {

                //std::vector<unsigned int> pos_col(4);
                unsigned int pos_col[4];

                // Calculate new node positions in the cover matrix
                // with respect to the current cell positon in the
                // sudoku grid and one of its domain values.
                pos_col[0] = s_num_cells*0 + s_line * s_size + s_col;
                pos_col[1] = s_num_cells*1 + s_line * s_size + value;
                pos_col[2] = s_num_cells*2 + s_col  * s_size + value;
                unsigned int s_region = (s_line/s_region_num_rows
                        + s_col/s_region_num_columns * s_region_num_columns);
                pos_col[3] = s_num_cells*3 + s_region * s_size + value;

                // Build the first node of the row separately because
                // it needs special care for its left and right pointers.
                Node* header = cm_columns_headers[pos_col[0]];
                Node* first = new Node;

                // Set up the first node.
                first->payload.cell.cellptr = &cell;
                first->payload.cell.value = value;
                first->header = header;
                first->up = header->up;
                first->down = header;
                first->left = first;
                first->right = first;

                // Update neighbours of the first node.
                first->up->down = first;
                header->up = first;
                header->payload.header.count++;

                // At the begining, the first node in
                // the row is also the last.
                Node* last = first;

                // Then, build the rest of the row.
                for (unsigned int i = 1; i < 4; ++i) {
                    Node* header = cm_columns_headers[pos_col[i]];
                    Node* current = new Node;

                    // Set up the current node.
                    current->payload.cell.cellptr = &cell;
                    current->payload.cell.value = value;
                    current->header = header;
                    current->up = header->up;
                    current->down = header;
                    current->left = last;
                    current->right = first;

                    // Update current node neighbours.
                    current->up->down = current;
                    current->left->right = current;
                    header->up = current;
                    first->left = current;
                    header->payload.header.count++;

                    // Set the current node as the last node of the row.
                    last = current;
                }
            }
        }
    }
    return cover_matrix_root;
}

void DancingLinksSolver::delete_cover_matrix(Node* root) {
    Node* header = root->right;
    // Go over each header of the cover matrix.
    while (header != root) {
        Node* col_el = header->down;
        // Then, go over each element of
        // the corresponding column.
        while (col_el != header) {
            Node* old_col_el = col_el;
            col_el = col_el->down;
            delete old_col_el;
        }
        Node* old_header = header;
        header = header->right;
        delete old_header;
    }
    // Finally, delete the root node.
    delete root;
}

bool DancingLinksSolver::solve(Node* root) {
    bool solved = false;
    // node* column_header = root->right; // slow !!!
    Node* column_header = choose_next_column(root);

    if (column_header == root)
        return true;

    cover_column(column_header);

    Node* column_element = column_header->down;
    while (column_element != column_header) {

        Node* row_element = column_element->right;
        while (row_element != column_element) {
            cover_column(row_element->header);
            row_element = row_element->right;
        }

        solved = solve(root);

        row_element = column_element->left;
        while (row_element != column_element) {
            uncover_column(row_element->header);
            row_element = row_element->left;
        }

        // If we've solved the exact cover problem,
        // set the cell value accordingly.
        if (solved) {
            unsigned int cell_value = column_element->payload.cell.value;
            Sudoku::Cell* cell = column_element->payload.cell.cellptr;
            cell->set_value(cell_value);
            break;
        }

        column_element = column_element->down;
    }

    uncover_column(column_header);
    return solved;
}

DancingLinksSolver::Node* DancingLinksSolver::choose_next_column(Node* root) {

    unsigned int lower_header_count = std::numeric_limits<unsigned int>::max();

    Node* current_header = root->right;
    Node* next_header = current_header;

    while (current_header != root) {

        unsigned int header_count = current_header->payload.header.count;
        if (header_count < lower_header_count) {
            lower_header_count = header_count;
            next_header = current_header;
        }

        current_header = current_header->right;
    }
    return next_header;
}

void DancingLinksSolver::cover_column(Node* header) {
    // Remove the header from the header row.
    header->left->right = header->right;
    header->right->left = header->left;
    // Go over each column elements.
    Node* col_el = header->down;
    while (col_el != header) {
        // Remove each neighbour elements in the
        // row of the current column element.
        Node* row_el = col_el->right;
        while (row_el != col_el) {
            row_el->up->down = row_el->down;
            row_el->down->up = row_el->up;
            row_el->header->payload.header.count--;
            row_el = row_el->right;
        }
        col_el = col_el->down;
    }
}

void DancingLinksSolver::uncover_column(Node* header) {
    // Go over each element in the column.
    Node* col_el = header->up;
    while (col_el != header) {
        Node* row_el = col_el->left;
        while (row_el != col_el) {
            row_el->up->down = row_el;
            row_el->down->up = row_el;
            row_el->header->payload.header.count++;
            row_el = row_el->left;
        }
        col_el = col_el->up;
    }
    // Reinsert the header in its row.
    header->left->right = header;
    header->right->left = header;
}
