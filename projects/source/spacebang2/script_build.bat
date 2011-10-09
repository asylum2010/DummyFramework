@echo off
flex\bin\flex -olexer.cpp script.l
cd bison\bin
bison -o ../../parser.cpp -d ../../script.y

pause
