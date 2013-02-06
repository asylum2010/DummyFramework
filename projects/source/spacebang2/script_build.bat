@echo off
call flex\bin\flex -olexer.cpp script.l

set CURR_DIR=%CD%

cd bison\bin
call bison -o %CURR_DIR%/parser.cpp -d %CURR_DIR%/script.y

cd %CURR_DIR%

