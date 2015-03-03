//! \file
//! \brief SudokuSolver interface.
//! \mainpage Dancing links sudoku solver
//! \brief Sudoku solver based on the dancing links algorithm.
//! \author Mathieu Turcotte
//! \date December 2010

#ifndef SUDOKU_SOLVER_H_
#define SUDOKU_SOLVER_H_

// Visual C++ does not implement checked exceptions.
#pragma warning(disable: 4290)

#include <deque>

#include "Sudoku.hpp"

//! Sudoku solvers base class.
class SudokuSolver {
public:
    //! \brief Solve a sudoku grid.
    //! \param[out] sudoku The sudoku grid to solve.
    //! \return True if the grid was completely solved, false otherwise.
    virtual bool solve(Sudoku& sudoku) = 0;
};

//! \brief Sudoku solver based on the dancing links algorithm.
class DancingLinksSolver: public SudokuSolver {
protected:
    //! \brief Cover matrix node.
    //!
    //! Nodes are both used to represent column headers and column
    //! elements. The payload element is handled differently.
    struct Node {
        Node* up;       /**< Pointer to the up node. */
        Node* down;     /**< Pointer to the down node. */
        Node* left;     /**< Pointer to the left node. */
        Node* right;    /**< Pointer to the right node. */
        Node* header;   /**< Pointer to the column header. */
        union {
            struct {
                Sudoku::Cell* cellptr;  /**< Pointer to the corresponding sudoku grid cell. */
                unsigned int value;     /**< Value represented by this matrix element. */
            } cell;     /**< Associated data for standard nodes (matrix element). */
            struct {
                unsigned int count;     /**< Number of elements in the column. */
                unsigned int id;        /**< Column id.  */
            } header;   /**< Associated data for header node. */
        } payload;      /**< The node associated data. */
    };

public:
    //! \brief Solve a sudoku grid.
    //! \param[out] s The sudoku grid to solve.
    //! \return True if the grid was solved, false otherwise.
    //!
    //! The grid won't be modified modified if no solution are found.
    bool solve(Sudoku& s);

protected:

    //! \brief Build the cover matrix corresponding to a given sudoku grid.
    //! \param s A sudoku grid.
    //! \return A pointer to the root node of the cover matrix.
    Node* build_cover_matrix(Sudoku& s);

    //! \brief Free the cover matrix associated memory.
    //! \param root A pointer to the root node of the cover matrix.
    void delete_cover_matrix(Node* root);

    //! \brief Solve a sudoku cover matrix.
    //! \param root A pointer to the root node of the cover matrix.
    //! \return True if a solution was found, false otherwise.
    //! \post The solution vector contains the solution nodes.
    bool solve(Node* root);

    //! \brief Choose the next column to cover.
    //! \param root A pointer to the root node of the cover matrix.
    //! \return A pointer to the column header.
    Node* choose_next_column(Node* root);

    //! \brief Cover a column.
    //! \param header A pointer to the header of the column to be covered.
    void cover_column(Node* header);

    //! \brief Uncover a column.
    //! \param header A pointer to the header of the column to be uncovered.
    void uncover_column(Node* header);
};

#endif // SUDOKU_SOLVER_H_
