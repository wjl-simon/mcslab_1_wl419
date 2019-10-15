game: main.o sudoku.o
	g++ -g main.o sudoku.o -o game

sudoku: sudoku.cpp sudoku.h
	g++ -g -c -Wall sudoku.cpp

main: main.cpp sudoku.h
	g++ -g -c -Wall main.cpp
