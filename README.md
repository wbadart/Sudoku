# Sudoku
A C++ sudoku player and solver.

This program reads formatted text files and translates them into a sudoku board. The core class at work here, `Puzzle`, is templated, so that the board can be comprised of elements of an arbitrary data type (such as `char`s for a wordoku puzzle).

### Two core features

1. The `Puzzle` class features a public member function, `play()`, which initiates an interactive mode with the user, allowing them to manually fill in the board and play the game. This mode features victory detection and access to the other core feature, the solver.

2. The `Puzzle` class also features a public member function, `solve()`, which uses a combination of two scanning algorithms to analyze and ultimately fill in the board with the solution. Interestingly, the constraints of the assignment did not allow the use of a recursive solving algorithm (maybe that will be featured in a future release).

### Developer interface

The main files of this project, 'Puzzle.hpp' and 'VecFunc.hpp' feature a robust library of utility functions that allow you to easily create, manipulate, and solve puzzles in your own program. Please see the header files for descriptions and prototypes.
