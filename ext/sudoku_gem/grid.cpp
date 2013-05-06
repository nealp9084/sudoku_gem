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

#include "grid.h"

#include <sstream>

Grid::Grid(std::size_t n) : matrix(boost::extents[n][n]), dim(n)
{
}

Grid::Grid(const Grid& grid) : matrix(grid.matrix), dim(grid.dim)
{
}

Grid& Grid::operator=(const Grid& grid)
{
  this->matrix = grid.matrix;
  this->dim = grid.dim;
  return *this;
}

int Grid::get(std::size_t x, std::size_t y) const
{
  return this->matrix[y][x];
}

void Grid::set(std::size_t x, std::size_t y, int i)
{
  this->matrix[y][x] = i;
}

std::size_t Grid::n() const
{
  return this->dim;
}

void Grid::reset(std::size_t n)
{
  this->matrix.resize(boost::extents[n][n]);
  this->dim = n;
}

std::string Grid::to_s() const
{
  std::stringstream out;
  this->to_s(out);
  return out.str();
}

void Grid::to_s(std::ostream& out) const
{
  for (std::size_t y = 0; y < this->dim; y++)
  {
    if (y != 0)
    {
      out << std::endl;
    }

    for (std::size_t x = 0; x < this->dim; x++)
    {
      if (x != 0)
      {
        out << ' ';
      }

      out << this->matrix[y][x];
    }
  }
}

Grid::~Grid()
{
}
