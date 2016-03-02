cc=g++
cflags=-ggdb3 -Wall -std=gnu++11 -gdwarf-2 -O0
exe=sudoku

all:$(exe).out

$(exe).out: main.o Puzzle.hpp
	$(cc) $(cflags) $< -o $@

%.o: %.cpp
	$(cc) $(cflags) -c $< -o $@

clean:
	@if [ -e *.o ]; \
	then \
		rm *.o; \
	fi;\
	if [ -e *.out ]; \
		then \
		rm *.out; \
	fi;

test: clean all
	@./$(exe).out
