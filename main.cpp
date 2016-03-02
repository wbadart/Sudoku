/* main.cpp
 *
 * Driver program for the Sudoku solver
 *
 * Will Badart
 * FEB 2016
 *
 */

#include "Puzzle.hpp"
using namespace std;

int main(int argc, char *argv[]){

    Puzzle<int> puz(argc==2?argv[1]:"boards/cc1.txt");
    puz.solve();
    cout << puz;

    return 0;

}
