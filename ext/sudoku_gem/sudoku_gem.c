#include <ruby.h>
#include <string>
#include "sudoku.h"

typedef VALUE (* ruby_method)(...);

extern "C"
VALUE sudoku_gem_solve(VALUE self, VALUE rb_puzzle)
{
  Sudoku sudoku;
  std::string cpp_puzzle = StringValueCStr(rb_puzzle);

  if (!sudoku.read_puzzle_from_string(cpp_puzzle))
  {
    return T_NIL;
  }

  sudoku.solve_colorability_style();

  std::string cpp_solution = sudoku.to_s();

  return rb_str_new(cpp_solution.c_str(), cpp_solution.length());
}

extern "C"
void Init_sudoku_gem()
{
  VALUE klass = rb_define_class("SudokuGem", rb_cObject);
  rb_define_singleton_method(klass, "solve", (ruby_method)&sudoku_gem_solve, 1);
}
