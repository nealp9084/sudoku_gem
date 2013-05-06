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
#include "validator.h"

#include <stdexcept>
#include <sstream>
#include <algorithm>
#include <boost/algorithm/string.hpp>

Sudoku::Sudoku() : grid(0), status_ok(false)
{
}

bool Sudoku::insert_row(std::vector<std::string>& tokens, std::size_t y)
{
  for (std::size_t x = 0; x < this->grid.n(); x++)
  {
    std::string const& token = tokens[x];

    try
    {
      int value = std::stoi(token);

      //integer tokens must be between 1 and n, and unknowns should be question marks
      if (value < 1 || value > (int)this->grid.n())
      {
        //invalid value in board
        return false;
      }
      else
      {
        this->grid.set(x, y, value);
      }
    }
    catch (std::invalid_argument &)
    {
      if (token == "?")
      {
        this->grid.set(x, y, -1);
      }
      else
      {
        //invalid character in board
        return false;
      }
    }
  }

  return true;
}

bool Sudoku::parse_puzzle(std::istream& f)
{
  std::string line;
  std::vector<std::string> tokens;

  //read the first line, to figure out n
  std::getline(f, line);
  boost::split(tokens, line, boost::is_any_of(" "));
  std::size_t n = tokens.size();

  if (n == 0 || line.empty())
  {
    //empty puzzle
    return false;
  }

  //make sure n is a perfect square
  std::size_t n_sq = std::size_t(sqrt(n));

  if (n_sq * n_sq != n && (n_sq + 1) * (n_sq + 1) != n)
  {
    //n is not a perfect square
    return false;
  }

  //make sure n is not too big

  if (n > 64)
  {
    //the board cannot be solved with this program
    return false;
  }

  //create the n*n grid
  this->grid.reset(n);

  //put in the first row
  if (!this->insert_row(tokens, 0))
  {
    return false;
  }

  //read n-1 lines
  for (std::size_t y = 1; y < n; y++)
  {
    std::getline(f, line);
    boost::split(tokens, line, boost::is_any_of(" "));

    //each line must have n tokens
    if (tokens.size() != n)
    {
      //invalid board
      return false;
    }

    //insert that row
    if (!this->insert_row(tokens, y))
    {
      return false;
    }
  }

  return true;
}

bool Sudoku::validate() const
{
  //whether this board is solvable
  return Validator::is_good_partial_board(this->grid);
}

bool Sudoku::read_puzzle_from_file(std::istream& f)
{
  if (this->parse_puzzle(f))
  {
    if (this->validate())
    {
      this->status_ok = true;
      return true;
    }
  }

  return false;
}

bool Sudoku::read_puzzle_from_string(std::string const& s)
{
  std::stringstream iss(s);

  if (this->parse_puzzle(iss))
  {
    if (this->validate())
    {
      this->status_ok = true;
      return true;
    }
  }

  return false;
}

void Sudoku::print(std::ostream& out) const
{
  if (!this->status_ok)
  {
    throw std::logic_error("Puzzle has not been initialized");
  }

  out << this->to_s() << std::endl;
}

std::string Sudoku::to_s() const
{
  std::string str = this->grid.to_s();
  boost::replace_all(str, "-1", "?");
  return str;
}

bool Sudoku::find_unknown(Grid& cur_grid, std::size_t cur_x, std::size_t cur_y,
  std::size_t& x_out, std::size_t& y_out)
{
  //find the next unknown node from where we left off, so we don't need to re-examine any elements
  for (std::size_t y = cur_y; y < cur_grid.n(); y++, cur_x = 0)
  {
    for (std::size_t x = cur_x; x < cur_grid.n(); x++)
    {
      if (cur_grid.get(x, y) == -1)
      {
        x_out = x;
        y_out = y;
        return true;
      }
    }
  }

  return false;
}

bool Sudoku::color_node(Grid& cur_grid, std::size_t cur_x, std::size_t cur_y)
{
  std::size_t unknown_x, unknown_y;

  //check if we can keep coloring nodes, or if we need to stop and assess the generated board
  if (find_unknown(cur_grid, cur_x, cur_y, unknown_x, unknown_y))
  {
    std::uint_fast64_t colors = Validator::good_colors(cur_grid, unknown_x, unknown_y);

    //clone the existing game board
    Grid new_grid(cur_grid);

    for (int i = 1; i <= (int)cur_grid.n(); i++)
    {
      //can we use this color here?
      if ((colors & (1 << (i - 1))) != 0)
      {
        //color the node
        new_grid.set(unknown_x, unknown_y, i);

        //if the coloring was successful, then return the colored graph indicate success
        if (color_node(new_grid, unknown_x, unknown_y))
        {
          cur_grid = new_grid;
          return true;
        }
      }
    }

    //we couldn't find a coloring :(
    return false;
  }
  else
  {
    //the board is completely colored, so we can't color any more nodes, but is it a valid coloring?
    return Validator::is_good_board(cur_grid);
  }
}

void Sudoku::solve_colorability_style()
{
  if (!this->status_ok)
  {
    throw std::logic_error("Puzzle has not been initialized");
  }

  color_node(this->grid);
}

bool Sudoku::bruteforce_node(Grid& cur_grid, std::size_t cur_x, std::size_t cur_y)
{
  std::size_t unknown_x, unknown_y;

  //check if we can keep coloring nodes, or if we need to stop and assess the generated board
  if (find_unknown(cur_grid, cur_x, cur_y, unknown_x, unknown_y))
  {
    //clone the existing game board
    Grid new_grid(cur_grid);

    for (int i = 1; i <= (int)cur_grid.n(); i++)
    {
      //color the cell value
      new_grid.set(unknown_x, unknown_y, i);

      //if the coloring was successful, then return the colored graph indicate success
      if (bruteforce_node(new_grid, unknown_x, unknown_y))
      {
        cur_grid = new_grid;
        return true;
      }
    }

    //we couldn't find a solution :(
    return false;
  }
  else
  {
    //the board is completely colored, so we can't color any more nodes, but is it a valid coloring?
    return Validator::is_good_board(cur_grid);
  }
}

void Sudoku::solve_bruteforce_style()
{
  if (!this->status_ok)
  {
    throw std::logic_error("Puzzle has not been initialized");
  }

  bruteforce_node(this->grid);
}

int Sudoku::singular_decider(Grid& cur_grid, bool found_one, std::size_t cur_x, std::size_t cur_y)
{
  std::size_t unknown_x, unknown_y;

  //check if we can keep coloring nodes, or if we need to stop and assess the generated board
  if (find_unknown(cur_grid, cur_x, cur_y, unknown_x, unknown_y))
  {
    std::uint_fast64_t colors = Validator::good_colors(cur_grid, unknown_x, unknown_y);

    //clone the existing game board
    Grid new_grid(cur_grid);

    for (int i = 1; i <= (int)cur_grid.n(); i++)
    {
      //can we use this color here?
      if ((colors & (1 << (i - 1))) != 0)
      {
        //color the node
        new_grid.set(unknown_x, unknown_y, i);

        //if the coloring was successful, then return the colored graph indicate success
        switch (singular_decider(new_grid, found_one, unknown_x, unknown_y))
        {
          case 0: { break; } //that branch did not yield a solution, so keep going
          case 1: { found_one = true; break; } //found one solution, so keep going
          case 2: { return 2; } //confirmed multiple solutions, so stop
        }
      }
    }

    //we're done with this branch
    return found_one ? 1 : 0;
  }
  else
  {
    //the board is completely colored, so we can't color any more nodes, but is it a valid coloring?
    if (Validator::is_good_board(cur_grid))
    {
      if (found_one)
      {
        //oh no, we've got multiple solutions!
        return 2;
      }
      else
      {
        //this is just the first solution
        return 1;
      }
    }
    else
    {
      //this branch did not yield a solution
      return 0;
    }
  }
}

bool Sudoku::singular()
{
  if (!this->status_ok)
  {
    throw std::logic_error("Puzzle has not been initialized");
  }

  if (this->validate())
  {
    return (singular_decider(this->grid) == 1);
  }
  else
  {
    return false;
  }
}

bool Sudoku::good() const
{
  return this->status_ok;
}

bool Sudoku::bad() const
{
  return !this->good();
}

Sudoku::operator bool() const
{
  return this->good();
}

Sudoku::~Sudoku()
{

}
