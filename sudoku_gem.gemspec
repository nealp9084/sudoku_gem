Gem::Specification.new do |s|
  s.name = 'sudoku_gem'
  s.version = '1.0.2'
  s.summary = 'Ruby interface to a C++ sudoku solver'
  s.authors = ['Neal Patel']
  s.license = 'GPL-3'
  s.description = 'This ruby gem is an interface to a fast C++ sudoku solver library'
  s.email = 'nap7jz@virginia.edu'
  s.homepage = 'https://www.epicdomain.name'

  s.files = Dir.glob('lib/*.rb') + Dir.glob('ext/**/*.{c,h,cpp,rb}')
  s.extensions = ['ext/sudoku_gem/extconf.rb']
end
