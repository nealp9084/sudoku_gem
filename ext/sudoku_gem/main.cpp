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

#include "sudoku.h"
#include <iostream>

int main(int argc, char* argv[])
{
  Sudoku puzzle;
  puzzle.read_puzzle_from_file(std::cin);

  if (puzzle)
  {
    std::cout << "Read the board in OK." << std::endl;
  }
  else
  {
    std::cout << "Failed to read the board from cin." << std::endl;
    return 1;
  }

  std::cout << "Here's the current state of the board." << std::endl;
  puzzle.print(std::cout);

  std::cout << std::endl << "Solving the puzzle..." << std::endl;

  puzzle.solve_colorability_style();

  std::cout << "A solution was found!" << std::endl;
  puzzle.print(std::cout);

  return 0;
}
