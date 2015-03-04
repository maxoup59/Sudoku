//! \file
//! \brief Sudoku grid interface.
//! \author Mathieu Turcotte

#ifndef SUDOKU_H_
#define SUDOKU_H_

// Visual C++ does not implement checked exceptions.
#pragma warning(disable: 4290)

#include <vector>
#include <string>
#include <stdexcept>
#include <ostream>

//! Class representing a sudoku grid.
class Sudoku {
public:
    //! Class representing a grid cell.
    class Cell {
    public:
        //! \brief Cell constructor (unset cell).
        //! \param domain The cell domain.
        Cell(unsigned short domain);

        //! \brief Cell constructor (set cell).
        //! \param domain The cell domain.
        //! \param value The cell value.
        Cell(unsigned short domain, unsigned short value) throw (std::domain_error);

        //! \brief Retrieve the cell value.
        //! \pre The cell value must be set.
        //! \return The cell value.
        unsigned short get_value() const throw(std::logic_error);

        //! \brief Set the cell value.
        //! \param value The new cell value.
        void set_value(unsigned short value) throw (std::domain_error);

        //! \brief Query the cell status.
        //! \return True if the cell value is set.
        bool is_set() const;

        //! \brief Reset the cell.
        void reset();

        //! \brief Comparaison operator
        //! \param rhs RHS cell.
        //! \return True if cells are equals.
        bool operator==(const Cell& rhs);

        //! \brief Comparaison operator
        //! \param rhs RHS cell.
        //! \return True if cells are different.
        bool operator!=(const Cell& rhs);

    protected:
        unsigned short domain;  /**< Domain of authorized values. */
        unsigned short value;   /**< Current cell value. */
        bool cell_is_set;       /**< Boolean to mark if the cell is set. */
    };

    //! \brief Sudoku constructor.
    //! \param repr A string representation of the grid.
    //! \param region_num_row Number of rows in a region.
    //! \param region_num_col Number of columns in a region.
    Sudoku(std::string repr, unsigned short region_num_row = 3,
            unsigned short region_num_col = 3) throw (std::logic_error);

    //! \brief Get a reference to a given cell.
    //! \param row The cell row.
    //! \param col The cell column.
    //! \return A reference to the specified cell.
    Cell& cell(unsigned short row, unsigned short col) throw(std::out_of_range);

    //! \brief Get a reference to a given cell.
    //! \param row The cell row.
    //! \param col The cell column.
    //! \return A const reference to the specified cell.
    const Cell& cell(unsigned short row, unsigned short col) const throw(std::out_of_range);

    //! \brief Get the grid size.
    //! \return The grid size.
    unsigned short size() const;

    //! \brief Get sudoku regions horizontal size.
    //! \return Region horizontal size, counted in number of rows.
    unsigned short region_num_columns() const;

    //! \brief Get sudoku regions vertical size.
    //! \return Region vertical size, counted in number of rows.
    unsigned short region_num_rows() const;

    //! \brief Get sudoku regions size.
    //! \return A pair containing regions vertical and horizontal size.
    std::pair<unsigned short, unsigned short> region_size() const;

    //! \brief Comparaison operator
    //! \param rhs RHS sudoku.
    //! \return True if both grids are equals.
    bool operator==(const Sudoku& rhs);

    std::string getString();

    //! \brief Comparaison operator
    //! \param rhs RHS sudoku.
    //! \return True if both grids are different.
    bool operator!=(const Sudoku& rhs);

protected:

    unsigned short grid_size;       /**< Size of the grid: 4x4, 9x9, etc. */
    unsigned short region_num_row;  /**< Vertical size of a region, counted in number rows. */
    unsigned short region_num_col;  /**< Horizontal size of a region, counted in number columns. */
    std::vector<std::vector<Cell> > cells;  /**< Grid of cells representing the sudoku. */
};

//! \brief operator<< overload to handle Sudoku::Cell.
std::ostream& operator<<(std::ostream& os, const Sudoku::Cell& cell);

//! \brief operator<< overload to handle Sudoku.
std::ostream& operator<<(std::ostream& os, const Sudoku& sudoku);

#endif // SUDOKU_H_
