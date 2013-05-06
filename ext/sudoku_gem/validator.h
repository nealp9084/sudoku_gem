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

#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <cstdint>
#include <cstddef>

#include "grid.h"

class Validator
{
public:
  /**
   * @brief Tells you whether a Sudoku puzzle has been solved.
   * 
   * A board is defined as "good" if every row has the digits 1-n used exactly once, if every column
   * has the digits 1-n used exactly once, and if every 3x3 block has the digits 1-n used exactly
   * once. Basically, this function is just checking whether the given board is a solution to some
   * Sudoku puzzle.
   *
   * @param cur_board A Sudoku puzzle board.
   * @return bool Whether that puzzle has been solved.
   **/
  static bool is_good_board(Grid const& cur_grid);
private:
  /**
   * @brief Helper function for the above task.
   *
   * @param cur_board A Sudoku puzzle board.
   * @param y The index for a particular row.
   * @return bool Whether the 1*n row contains no repeated or undetermined elements.
   **/
  static bool is_good_row(Grid const& cur_grid, std::size_t y);
  /**
   * @brief Helper function for the above task.
   *
   * @param cur_board A Sudoku puzzle board.
   * @param x The index for a particular column.
   * @return bool Whether the n*1 column contains no repeated or undetermined elements.
   **/
  static bool is_good_column(Grid const& cur_grid, std::size_t x);
  /**
   * @brief Helper function for the above task.
   *
   * @param cur_board A Sudoku puzzle board.
   * @param x The index for a particular starting row. Must be a multiple of 3.
   * @param y The index for a particular starting column.  Must be a multiple of 3.
   * @return bool Whether the sqrt(n)*sqrt(n) block contains no repeated or undetermined elements.
   **/
  static bool is_good_block(Grid const& cur_grid, std::size_t x, std::size_t y);

public:
  /**
   * @brief Tells you whether you can color a certain node in a certain way (i.e., is it okay to use
   *        a particular number in this Sudoku cell?)
   * 
   * This validation will tell you whether you can use a specific color, provided that it does not
   * appear in the same row, in the same column, or the same 3x3 block.
   *
   * @param cur_board A Sudoku puzzle board.
   * @param x The x position of the cell.
   * @param y The y position of the cell.
   * @param i The color you want to use (i.e., the number to assign the cell).
   * @return bool Whether the node can be colored that way.
   **/
  static bool is_good_color(Grid const& cur_grid, std::size_t x, std::size_t y, int i);

  /**
   * @brief Tells you which colors a certain node may use (i.e., which numbers can I put in this
   *        Sudoku cell?).
   * 
   * This validation will tell you whether you can use a specific color, provided that it does not
   * appear in the same row, in the same column, or the same 3x3 block. The result is encoded into a
   * 64-bit unsigned integer, with the least significant bit corresponding to whether you can use
   * the color 1, the next bit corresponding to whether you can use the number 2, and so on.
   *
   * @param cur_board A Sudoku puzzle board.
   * @param x The x position of the cell.
   * @param y The y position of the cell.
   * @return uint_fast64_t The various colors (numbers) you may use, encoded using the above scheme.
   **/
  static std::uint_fast64_t good_colors(Grid const& cur_grid, std::size_t x, std::size_t y);
private:
  /**
   * @brief Helper function for the above task. Tells you which colors have been used.
   *
   * @param cur_board A Sudoku puzzle board.
   * @param y The index for a particular row.
   * @return int Which colors are used by that particular 1*n row.
   **/
  static std::uint_fast64_t row_colors(Grid const& cur_grid, std::size_t y);
  /**
   * @brief Helper function for the above task. Tells you which colors have been used.
   *
   * @param cur_board A Sudoku puzzle board.
   * @param x The index for a particular column.
   * @return int Which colors are used by that particular n*1 column.
   **/
  static std::uint_fast64_t column_colors(Grid const& cur_grid, std::size_t x);
  /**
   * @brief Helper function for the above task. Tells you which colors have been used.
   *
   * @param cur_board A Sudoku puzzle board.
   * @param x The index for a particular starting row. Must be a multiple of 3.
   * @param y The index for a particular starting column.  Must be a multiple of 3.
   * @return int Which colors are used by that particular sqrt(n)*sqrt(n) block.
   **/
  static std::uint_fast64_t block_colors(Grid const& cur_grid, std::size_t x, std::size_t y);

public:
  /**
   * @brief Tells you if a given Sudoku board has a solution (if it has no repeats, then it does).
   * 
   * A partially-completed board is defined as "good" if every row has the digits 1-n used at most
   * once, if every column has the digits 1-n used at most once, and if every 3x3 block has the digits
   * 1-n used at most once. Basically, this function is just checking whether the given board is a
   * solvable Sudoku puzzle.
   *
   * This function can be used when solving a Sudoku puzzle with the graph n-colorability method, and
   * many other solution methods.
   *
   * @param cur_board A Sudoku puzzle board.
   * @return bool Whether the given board has any solutions.
   **/
  static bool is_good_partial_board(Grid const& cur_grid);
private:
  /**
   * @brief Helper function for the above task.
   *
   * @param cur_board A Sudoku puzzle board.
   * @param y The index for a particular row.
   * @return bool Whether the given 1*n row has any repeated elements.
   **/
  static bool is_good_partial_row(Grid const& cur_grid, std::size_t y);
  /**
   * @brief Helper function for the above task.
   *
   * @param cur_board A Sudoku puzzle board.
   * @param x The index for a particular column.
   * @return bool Whether the given n*1 column has any repeated elements.
   **/
  static bool is_good_partial_column(Grid const& cur_grid, std::size_t x);
  /**
   * @brief Helper function for the above task.
   *
   * @param cur_board A Sudoku puzzle board.
   * @param x The index for a particular starting row. Must be a multiple of 3.
   * @param y The index for a particular starting column.  Must be a multiple of 3.
   * @return bool Whether the given sqrt(n)*sqrt(n) block has any repeated elements.
   **/
  static bool is_good_partial_block(Grid const& cur_grid, std::size_t x, std::size_t y);
};

#endif // VALIDATOR_H
