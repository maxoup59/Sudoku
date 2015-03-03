//! \file
//! \brief Sudoku grid implementation.
//! \author Mathieu Turcotte

#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>    // std::transform
#include <cctype>       // tolower
#include<sstream>
#include "Sudoku.hpp"

Sudoku::Cell::Cell(unsigned short domain): domain(domain), value(0),
    cell_is_set(false) {
}

Sudoku::Cell::Cell(unsigned short domain, unsigned short value) throw (std::domain_error):
    domain(domain), value(value), cell_is_set(true) {

    if (value >= domain)
        throw std::domain_error("Cell::Cell(unsigned short, unsigned short)");
}

unsigned short Sudoku::Cell::get_value() const throw(std::logic_error) {
    if (!cell_is_set)
        throw std::logic_error("Sudoku::Cell::get_value(void)");

    return value;
}

void Sudoku::Cell::set_value(unsigned short value) throw (std::domain_error) {
    if (value >= domain)
        throw std::domain_error("Cell::set_value(unsigned short)");

    cell_is_set = true;
    this->value = value;
}

bool Sudoku::Cell::is_set() const {
    return cell_is_set;
}

void Sudoku::Cell::reset() {
    cell_is_set = false;
}

bool Sudoku::Cell::operator==(const Cell& rhs) {
    if (cell_is_set) {
        return domain == rhs.domain &&
                value == rhs.value &&
                cell_is_set == rhs.cell_is_set;
    } else {
        return domain == rhs.domain &&
                cell_is_set == rhs.cell_is_set;
    }
}

bool Sudoku::Cell::operator!=(const Cell& rhs) {
    return !operator==(rhs);
}

Sudoku::Sudoku(std::string repr, unsigned short region_num_row,
        unsigned short region_num_col) throw (std::logic_error):
        region_num_row(region_num_row), region_num_col(region_num_col) {

    if (region_num_col > 5 || region_num_row > 5)
        throw std::logic_error("Sudoku::Sudoku(std::string, "
                               "unsigned short, unsigned short): "
                               "maximum grid size is 25x25.");

    // First, obtain the grid size from the region size.
    grid_size = region_num_col * region_num_row;

    // Then, obtain the number of cells in the grid.
    unsigned int num_cells = grid_size * grid_size;

    // Make sure that the representation size match
    // the number of cells in the grid.
    if (repr.size() != num_cells)
        throw std::logic_error("Sudoku::Sudoku(std::string, "
                               "unsigned short, unsigned short): "
                               "representation size doesn't match "
                               "specified grid size.");

    // Build an empty grid.
    cells.resize(grid_size);
    for (std::vector<std::vector<Cell> >::iterator it = cells.begin();
            it != cells.end(); ++it) {
        it->resize(grid_size, Cell(grid_size));
    }

    // Convert the representation to lowercase.
    std::transform(repr.begin(), repr.end(), repr.begin(), tolower);

    // Populate the grid with predefined values.
    for (unsigned short i = 0; i < num_cells; ++i) {
        if (repr[i] != 'x' && repr[i] != ' ') {
            unsigned short row = i / grid_size;     // Current row index.
            unsigned short column = i % grid_size;  // Current column index.
            unsigned short value;   // A temporary to store the value.

            // Convert the character to a short.
            if ('a' <= repr[i] && repr[i] <= 'w') {
                value = static_cast<unsigned short>(repr[i] - 87);
            } else if ('0' <= repr[i] && repr[i] <= '9') {
                value = static_cast<unsigned short>(repr[i] - 48);
            } else {
                throw std::logic_error("Sudoku::Sudoku(std::string, "
                                        "unsigned short, unsigned short): "
                                        "value out of range in representation");
            }

            // Then, update the cell representation.
            cells[row][column] = Cell(grid_size, value);
        }
    }
}

Sudoku::Cell& Sudoku::cell(unsigned short row, unsigned short col)
    throw(std::out_of_range) {

    if (row >= grid_size || col >= grid_size)
        throw std::out_of_range("Sudoku::Cell& cell(unsigned short, unsigned short)");

    return cells[row][col];
}

const Sudoku::Cell& Sudoku::cell(unsigned short row, unsigned short col) const
    throw(std::out_of_range) {

    if (row >= grid_size || col >= grid_size)
        throw std::out_of_range("const Sudoku::Cell& cell(unsigned short, unsigned short)");

    return cells[row][col];
}

unsigned short Sudoku::size() const {
    return grid_size;
}

unsigned short Sudoku::region_num_columns() const {
    return region_num_col;
}

unsigned short Sudoku::region_num_rows() const {
    return region_num_row;
}

std::pair<unsigned short, unsigned short> Sudoku::region_size() const {
    return std::make_pair(region_num_row, region_num_col);
}

bool Sudoku::operator==(const Sudoku& rhs) {

    if (region_num_col != rhs.region_num_col ||
            region_num_row != rhs.region_num_row) {
        return false;
    }

    for (unsigned short i = 0; i < grid_size; ++i) {
        for (unsigned short j = 0; j < grid_size; ++j) {
            if (cells[i][j] != rhs.cells[i][j]) {
                return false;
            }
        }
    }
    return true;
}

bool Sudoku::operator!=(const Sudoku& rhs) {
    return !operator==(rhs);
}

std::ostream& operator<<(std::ostream& os, const Sudoku::Cell& cell) {

    if (cell.is_set()) {
        unsigned int value = cell.get_value();
        if (value < 10) {
            os << static_cast<char>(value + 48) << " ";
        } else {
            os << static_cast<char>(value + 55) << " ";
        }
    } else {
        os << "  ";
    }

    return os;
}

std::ostream& operator<<(std::ostream& os, const Sudoku& sudoku) {

    unsigned int region_num_row = sudoku.region_num_rows();
    unsigned int region_num_col = sudoku.region_num_columns();

    // Iterate over each line.
    for (unsigned int i = 0; i < sudoku.size(); ++i) {

        // Are we on a region horizontal border ?
        // If so, print a line before the next row.
        if (i % region_num_row == 0 && i > 0) {
            for (unsigned int k = 0; k < sudoku.size(); k++) {
                os << "--";
            }
            for (unsigned int k = 2; k < sudoku.size() / region_num_col; k++) {
                os << "--";
            }
            os << "-" << std::endl;
        }

        // Print a row.
        for (unsigned int j = 0; j < sudoku.size(); ++j) {
            // Mark the sepration between region.
            if (j != 0 && j % region_num_col == 0) {
                os << "| ";
            }
            // Print cell.
            os << sudoku.cell(i, j);
        }
        os << std::endl;
    }
    return os;
}

std::string Sudoku::getString(const Sudoku& sudoku) {
   std::stringstream os;
    unsigned int region_num_row = sudoku.region_num_rows();
    unsigned int region_num_col = sudoku.region_num_columns();
    for (unsigned int i = 0; i < sudoku.size(); ++i) {
        for (unsigned int j = 0; j < sudoku.size(); ++j) {
            os << sudoku.cell(i, j);
        }
        //os += std::endl;
    }
    return os.str();
}
