require 'sudoku_gem/sudoku_gem'

class SudokuGem
  def self.solution_for(puzzle)
    text = self.solve(puzzle)

    if text
      return text.split("\n").map { |x| x.split(' ').map{ |y| y.to_i } }
    else
      return nil
    end
  end
end
