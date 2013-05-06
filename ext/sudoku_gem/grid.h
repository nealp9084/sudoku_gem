/*
 *  Sudoku Base - a library for solving Sudoku puzzles
 *  Copyright (C) 2013  Neal Patel <nealp9084@gmail.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef GRID_H
#define GRID_H

#include <cstddef>
#include <string>
#include <streambuf>
#include <boost/multi_array.hpp>

/**
 * @brief A class that represents the state of a Sudoku board
 * 
 * The Grid class represents the current state of a particular Sudoku board. It is essentially a
 * wrapper around a square, 2D array, except that it does not need to be defined at compile-time.
 * The grid is stored in an underlying boost::multi_array<int, 2>, and it has all the typical
 * accessors: set an element, get an element, get the grid size, and set the grid size.
 **/
class Grid
{
public:
  /**
   * @brief Construct a n*n grid
   *
   * @param n Side length of the square, 2D array.
   **/
  Grid(std::size_t n);
  /**
   * @brief Copy-construct a grid from another grid
   *
   * @param grid The other grid.
   **/
  Grid(Grid const& grid);
  virtual ~Grid();

  /**
   * @brief Copy another grid
   *
   * @param grid The other grid.
   * @return Grid& The current grid.
   **/
  Grid& operator =(Grid const& grid);

  /**
   * @brief Resize the existing grid so that it is an n*n grid
   *
   * @param n Side length of the square, 2D array.
   **/
  void reset(std::size_t n);

  /**
   * @brief Get the value of a specific element
   *
   * @param x The X-index of the element.
   * @param y The Y-index of the element.
   * @return int The value of the element.
   **/
  int get(std::size_t x, std::size_t y) const;
  /**
   * @brief Set the value of a specific element
   *
   * @param x The X-index of the element.
   * @param y The Y-index of the element.
   * @param i The new value of the element.
   **/
  void set(std::size_t x, std::size_t y, int i);

  /**
   * @brief The side length of the square, 2D array
   *
   * @return std::size_t The side-length.
   **/
  std::size_t n() const;
  
  /**
   * @brief Show the grid in a human-readable form.
   *
   * @return std::string The grid.
   **/
  std::string to_s() const;
  /**
   * @brief Convert the grid to a human-readable form and pipe that into the given output stream.
   *
   * @param out The output stream.
   **/
  void to_s(std::ostream& out) const;

private:
  /**
   * @brief The underlying grid.
   **/
  boost::multi_array<int, 2> matrix;
  /**
   * @brief The side length of the grid.
   **/
  std::size_t dim;
};

#endif // GRID_H
