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

#include "validator.h"

bool Validator::is_good_row(Grid const& cur_grid, std::size_t y)
{
  const std::size_t n = cur_grid.n();
  std::uint_fast64_t mask = 0, valid_mask = (1 << n) - 1;

  for (std::size_t x = 0; x < n; x++)
  {
    int a = cur_grid.get(x, y);

    //reject rows that are incomplete or have duplicates
    if ((a == -1) || ((mask & (1 << (a - 1))) != 0))
    {
      return false;
    }
    else
    {
      mask |= (1 << (a - 1));
    }
  }

  return (mask == valid_mask);
}

bool Validator::is_good_column(Grid const& cur_grid, std::size_t x)
{
  const std::size_t n = cur_grid.n();
  std::uint_fast64_t mask = 0, valid_mask = (1 << n) - 1;

  for (std::size_t y = 0; y < n; y++)
  {
    int a = cur_grid.get(x, y);

    //reject cols that are incomplete or have duplicates
    if ((a == -1) || ((mask & (1 << (a - 1))) != 0))
    {
      return false;
    }
    else
    {
      mask |= (1 << (a - 1));
    }
  }

  return (mask == valid_mask);
}

bool Validator::is_good_block(Grid const& cur_grid, std::size_t x, std::size_t y)
{
  const std::size_t n = cur_grid.n(), n_root = std::size_t(sqrt(n) + 0.5);
  std::uint_fast64_t mask = 0, valid_mask = (1 << n) - 1;

  for (std::size_t y_off = 0; y_off < n_root; y_off++)
  {
    for (std::size_t x_off = 0; x_off < n_root; x_off++)
    {
      int a = cur_grid.get(x + x_off, y + y_off);

      //reject blocks that are incomplete or have duplicates
      if ((a == -1) || ((mask & (1 << (a - 1))) != 0))
      {
        return false;
      }
      else
      {
        mask |= (1 << (a - 1));
      }
    }
  }

  return (mask == valid_mask);
}

bool Validator::is_good_board(Grid const& cur_grid)
{
  const std::size_t n = cur_grid.n(), n_root = std::size_t(sqrt(n) + 0.5);

  for (std::size_t y = 0; y < n; y++)
  {
    if (!is_good_row(cur_grid, y))
    {
      return false;
    }
  }

  for (std::size_t x = 0; x < n; x++)
  {
    if (!is_good_column(cur_grid, x))
    {
      return false;
    }
  }

  for (std::size_t x = 0; x < n_root; x++)
  {
    for (std::size_t y = 0; y < n_root; y++)
    {
      if (!is_good_block(cur_grid, x * n_root, y * n_root))
      {
        return false;
      }
    }
  }

  return true;
}

std::uint_fast64_t Validator::row_colors(Grid const& cur_grid, std::size_t y)
{
  const std::size_t n = cur_grid.n();
  std::uint_fast64_t mask = 0;

  for (std::size_t x = 0; x < n; x++)
  {
    int a = cur_grid.get(x, y);

    //ignore incomplete elements
    if (a != -1)
    {
      mask |= (1 << (a - 1));
    }
  }

  return mask;
}

std::uint_fast64_t Validator::column_colors(Grid const& cur_grid, std::size_t x)
{
  const std::size_t n = cur_grid.n();
  std::uint_fast64_t mask = 0;

  for (std::size_t y = 0; y < n; y++)
  {
    int a = cur_grid.get(x, y);

    //ignore incomplete elements
    if (a != -1)
    {
      mask |= (1 << (a - 1));
    }
  }

  return mask;
}

std::uint_fast64_t Validator::block_colors(Grid const& cur_grid, std::size_t x, std::size_t y)
{
  const std::size_t n = cur_grid.n(), n_root = std::size_t(sqrt(n) + 0.5);
  std::uint_fast64_t mask = 0;

  for (std::size_t y_off = 0; y_off < n_root; y_off++)
  {
    for (std::size_t x_off = 0; x_off < n_root; x_off++)
    {
      int a = cur_grid.get(x + x_off, y + y_off);

      //ignore incomplete elements
      if (a != -1)
      {
        mask |= (1 << (a - 1));
      }
    }
  }

  return mask;
}

bool Validator::is_good_color(Grid const& cur_grid, std::size_t x, std::size_t y, int i)
{
  const std::size_t n = cur_grid.n(), n_root = std::size_t(sqrt(n) + 0.5);

  int bit = (1 << (i - 1));
  std::uint_fast64_t row_mask = row_colors(cur_grid, y),
    col_mask = column_colors(cur_grid, x),
    block_mask = block_colors(cur_grid, (x / n_root) * n_root, (y / n_root) * n_root);

  return ((row_mask & bit) == 0 && (col_mask & bit) == 0 && (block_mask & bit) == 0);
}

std::uint_fast64_t Validator::good_colors(Grid const& cur_grid, std::size_t x, std::size_t y)
{
  const std::size_t n = cur_grid.n(), n_root = std::size_t(sqrt(n) + 0.5);

  std::uint_fast64_t row_mask = row_colors(cur_grid, y),
    col_mask = column_colors(cur_grid, x),
    block_mask = block_colors(cur_grid, (x / n_root) * n_root, (y / n_root) * n_root);

  return (~(row_mask | col_mask | block_mask)) & ((1 << n) - 1);
}

bool Validator::is_good_partial_row(Grid const& cur_grid, std::size_t y)
{
  const std::size_t n = cur_grid.n();
  std::uint_fast64_t mask = 0;

  for (std::size_t x = 0; x < n; x++)
  {
    int a = cur_grid.get(x, y);

    //ignore incomplete elements
    if (a != -1)
    {
      //reject rows that have duplicates
      if ((mask & (1 << (a - 1))) != 0)
      {
        return false;
      }
      else
      {
        mask |= (1 << (a - 1));
      }
    }
  }

  return true;
}

bool Validator::is_good_partial_column(Grid const& cur_grid, std::size_t x)
{
  const std::size_t n = cur_grid.n();
  std::uint_fast64_t mask = 0;

  for (std::size_t y = 0; y < n; y++)
  {
    int a = cur_grid.get(x, y);

    //ignore incomplete elements
    if (a != -1)
    {
      //reject cols that have duplicates
      if ((mask & (1 << (a - 1))) != 0)
      {
        return false;
      }
      else
      {
        mask |= (1 << (a - 1));
      }
    }
  }

  return true;
}

bool Validator::is_good_partial_block(Grid const& cur_grid, std::size_t x, std::size_t y)
{
  const std::size_t n = cur_grid.n(), n_root = std::size_t(sqrt(n) + 0.5);
  std::uint_fast64_t mask = 0;

  for (std::size_t y_off = 0; y_off < n_root; y_off++)
  {
    for (std::size_t x_off = 0; x_off < n_root; x_off++)
    {
      int a = cur_grid.get(x + x_off, y + y_off);

      //ignore incomplete elements
      if (a != -1)
      {
        //reject blocks that have duplicates
        if ((mask & (1 << (a - 1))) != 0)
        {
          return false;
        }
        else
        {
          mask |= (1 << (a - 1));
        }
      }
    }
  }

  return true;
}

bool Validator::is_good_partial_board(Grid const& cur_grid)
{
  const std::size_t n = cur_grid.n(), n_root = std::size_t(sqrt(n) + 0.5);

  for (std::size_t y = 0; y < n; y++)
  {
    if (!is_good_partial_row(cur_grid, y))
    {
      return false;
    }
  }

  for (std::size_t x = 0; x < n; x++)
  {
    if (!is_good_partial_column(cur_grid, x))
    {
      return false;
    }
  }

  for (std::size_t x = 0; x < n_root; x++)
  {
    for (std::size_t y = 0; y < n_root; y++)
    {
      if (!is_good_partial_block(cur_grid, x * n_root, y * n_root))
      {
        return false;
      }
    }
  }

  return true;
}
