require 'mkmf'

CONFIG['CC'] = CONFIG['CXX'] or 'g++'
$CFLAGS << ' --std=c++11'
have_library('stdc++')
$warnflags.gsub!(' -Wdeclaration-after-statement','')
$warnflags.gsub!(' -Wimplicit-function-declaration','')
create_makefile 'sudoku_gem/sudoku_gem'
