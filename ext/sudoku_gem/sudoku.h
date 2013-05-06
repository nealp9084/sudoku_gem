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

#ifndef SUDOKU_H
#define SUDOKU_H

#include <cstddef>
#include <streambuf>
#include <string>
#include <vector>

#include "grid.h"

/**
 * @brief This is a class designed to quickly and easily solve puzzles for the popular game Sudoku.
 * 
 * If you want to solve a puzzle, first create a Sudoku object. Next, you have to tell the class
 * what it is working with: you have to provide a puzzle to the object. You can do this by calling
 * either read_puzzle_from_file() or read_puzzle_from_string(). Once you do that, you should check
 * to make sure the puzzle was read in correctly by calling good(). Now that the class knows what it
 * is dealing with, it can start solving the puzzle: just call one of the solver methods. These
 * methods include: solve_colorability_style() and solve_bruteforce_style(). Once you call one of
 * those methods, the solution to the puzzle will be saved in the object.
 * 
 * Please note that due to memory constraints, this class can only ever hope to solve puzzles up to
 * size 64*64. The actual Sudoku grid validations are performed by using bit hacks on 64-bit
 * unsigned integers, so anything over 64 would cause an overflow. However, it is certainly not
 * realistic to expect a solution for such a large board: if even a quarter of the cells in a 64*64
 * board were unknown, there would be over 1000 unknowns, which means there would be over 1000
 * recursive function calls, each one taking up a portion of the stack and each one attempting to
 * allocate memory on the heap. This could lead to a stack or heap overflow (memory exhaustion).
 **/
class Sudoku
{
public:
  /**
   * @brief Constructor for a Sudoku instance.
   **/
  Sudoku();
  virtual ~Sudoku();

  /**
   * @brief Read in the puzzle from a given FILE* (this defaults to standard input) and store it in
   *        memory.
   *
   * @param f The file from which we should read the n*n Sudoku board, with rows separated by
   *          newlines, columns separated by spaces, integers [1-n] as the known values, and '?' for
   *          unknown values.
   * @return bool Whether the parsing succeeded.
   **/
  bool read_puzzle_from_file(std::istream& f);
  /**
   * @brief Read in the puzzle from a given string representing the Sudoku board, and then store it
   *        in memory.
   *
   * @param s A string containing a n*n Sudoku board, with rows separated by newlines, columns
   *          separated by spaces, integers [1-n] as the known values, and '?' for unknown values.
   *                  unknown values.
   * @return bool Whether the parsing succeeded.
   **/
  bool read_puzzle_from_string(std::string const& s);

  /**
   * @brief Print the current state of the board to some output stream.
   *
   * @param out An output stream.
   **/
  void print(std::ostream& out) const;
  /**
   * @brief Return the current state of the board as a human-readable string.
   *
   * @return std::string The human-readable representation of the board.
   **/
  std::string to_s() const;

  /**
   * @brief Determine whether the puzzle has only a single solution by using the graph 9-coloring
   *        technique. If there are no solutions or multiple solutions, the method will return true.
   *
   * @return bool Whether the Sudoku board has only 1 solution.
   **/
  bool singular();

  /**
   * @brief Attempt to solve the puzzle using the graph 9-coloring technique. If the puzzle was
   *        successfully solved, then the solution will be saved to memory (overwriting the existing
   *        grid) and the method will return true. If the puzzle could not be solved for whatever
   *        reason, then this method will return false.
   **/
  void solve_colorability_style();
  /**
   * @brief Attempt to solve the puzzle by brute force. If the puzzle was successfully solved, then
   *        the solution will be saved to memory (overwriting the existing grid) and the method will
   *        return true. If the puzzle could not be solved for whatever reason, then this method
   *        will return false. Note that this approach will take a very, very long time. However, it
   *        will EVENTUALLY find a solution.
   **/
  void solve_bruteforce_style();

  /**
   * @brief Accessor for Sudoku::status_ok
   *
   * @return bool Whether the Sudoku board is ready to solve
   **/
  bool good() const;
  /**
   * @brief Inverse accessor for Sudoku::status_ok
   *
   * @return bool Whether the Sudoku board is NOT ready to solve
   **/
  bool bad() const;
  /**
   * @brief Implicit accessor for Sudoku::status_ok
   *
   * @return bool Whether the Sudoku board is ready to solve
   **/
  operator bool() const;

private:
  /**
   * @brief Helper method for parsing a puzzle from a file
   *
   * @param f The file from which we are reading.
   * @return bool Whether the parsing succeeded.
   **/
  bool parse_puzzle(std::istream& f);
  /**
   * @brief Helper method for parsing a single, tokenized row of a Sudoku grid
   *
   * @param tokens The row elements.
   * @param y The index of the row.
   * @return bool Whether the parsing succeeded.
   **/
  bool insert_row(std::vector<std::string>& tokens, std::size_t y);
  /**
   * @brief Helper method for checking whether the given puzzle is solvable
   * @return bool Whether the validation succeeded
   **/
  bool validate() const;

  /**
   * @brief Helper method for finding the next unknown (i.e., undetermined) Sudoku cell.
   *
   * @param cur_board The Sudoku game board.
   * @param cur_x The last x position considered on the game board.
   * @param cur_y The last y position considered on the game board.
   * @param x_out The x position of the next unknown cell.
   * @param y_out The y position of the next unknown cell.
   * @return bool Whether we were able to find an unknown cell.
   **/
  static bool find_unknown(Grid& cur_grid, std::size_t cur_x, std::size_t cur_y,
    std::size_t& x_out, std::size_t& y_out);

  /**
   * @brief Helper method for solving an instance of a Sudoku puzzle using the graph 9-colorability
   *        solution method. If a 9-coloring is found, the method will return true and overwrite
   *        cur_board with the solution (which corresponds with the graph coloring).
   *
   * @param cur_board The Sudoku game board.
   * @param cur_x The last x position considered on the game board. Defaults to 0.
   * @param cur_y The last y position considered on the game board. Defaults to 0.
   * @return bool Whether we were able to find a 9-coloring for the Sudoku board.
   **/
  static bool color_node(Grid& cur_grid, std::size_t cur_x = 0, std::size_t cur_y = 0);
  /**
   * @brief Helper method for solving an instance of a Sudoku puzzle using the bruteforce solution
   *        method. If a solution is found, the method will return true and overwrite cur_board with
   *        the solution.
   *
   * @param cur_grid ...
   * @param cur_x ... Defaults to 0.
   * @param cur_y ... Defaults to 0.
   * @return bool
   **/
  static bool bruteforce_node(Grid& cur_grid, std::size_t cur_x = 0, std::size_t cur_y = 0);

  static int singular_decider(Grid& cur_grid, bool found_one = false, std::size_t cur_x = 0,
                               std::size_t cur_y = 0);

  /**
   * @brief The Sudoku board, which we are saving in memory.
   **/
  Grid grid;

  /**
   * @brief Whether the board is initialized (i.e., can we operate on this object?)
   **/
  bool status_ok;
};

#endif // SUDOKU_H
