/* Puzzle.h
 *
 * Combined interface and implementation
 * to the Puzzle class.
 *
 * Will Badart
 * FEB 2016
 *
 */

#ifndef PUZZLE_H
#define PUZZLE_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <cctype>
#include <boost/format.hpp>

#include "VecFunc.hpp"

using namespace std;


// Puzzle class
template<typename T>
class Puzzle{

    // Friendly neighborhood iostream operators
    template<typename U>
    friend ostream& operator<<(ostream&, Puzzle<U>&);
    template<typename U>
    friend istream& operator>>(istream&, Puzzle<U>&);

    // Constructors and utilities
    public:

        // Will create a board of 0's
        Puzzle(int dim_in = 9);

        // Automatically parses board data
        Puzzle(fstream&);

        // Opens the specified file, parses it, and closes it
        Puzzle(string filename);
    
        // Get the width of board and read accepted input chars
        int readDim(fstream& fs);

        // Determines if a vector only contains only unique vals
        // Skips appropriate empty cell value (i.e. '0' or 0)
        bool isunique(vector<T>);
        
        // Determines if a vector contains a zero value
        bool has0s(vector<T>);

        // Interact with the game board
        void play(void);

        // Use isunique to determine if a board is solved
        bool victory(void);

        // Solve the puzzle (returns false if unsolveable)
        bool solve(void);

        // Determine if elem is already present in the sub grid
        // containing (x,y)
        bool check3x3(T elem, int x, int y);

        // Runs all three logic checks on the correctness of elem at (x,y)
        bool checkpos(T elem, int x, int y);

        // Return the row or column at given index
        vector<T> getRow(int index);
        vector<T> getCol(int index);

        // Takes a 3D vector of possible values and places all singletons
        // Returns false if no values placed
        bool placeSingletons(vector<vector<vector<T>>>&);

        // Clears the 3D input vector and re-populates with legal values
        void resetPoss(vector<vector<vector<T>>>&);

    private:

        // Contains the data of the puzzle; 0's represent empty cells
        vector<vector<T>> board;

        // Tracks allowed values
        vector<T> legalvals;

        // Stores the side-length of the board
        int  dimension;

        // Tracks whether the puzzle is sud- or wordoku
        bool word; //true for wordoku, false for sudoku

        // In interactive mode, allow/ban entering element already present
        // in row/col/sub grid. Default false
        bool allowDups{false};
};

//==========================================//
/* ========= Begin Implementation ========= */
//==========================================//

// Enables you to pipe Puzzles into a stream
template<typename T>
ostream& operator<<(ostream& stream, Puzzle<T>& puzzle){
    //bool nines = puzzle.dimension == 9; //apply special eyecandy for 9x9 board
    bool nines = false;
    typename vector<T>::const_iterator j;

    // Column labels
    if(nines){
        stream << "   ";
        for(int i = 0; i < puzzle.dimension; i++){
            stream << " " << (i + 1);
            if(nines && (i + 1) % 3 == 0 && i != puzzle.dimension - 1) stream << " |";
        }
        stream << endl;
    }

    // Board data
    for(unsigned int i = 0; i < puzzle.board.size(); i++){
        // Row labels
        if(nines && i % 3 == 0) stream << "  +-------+-------+-------+\n";
        if(nines) stream << (i + 1) << " ";
        int cols = 0;
        // Prints the real guts of the board
        for(j = puzzle.board[i].begin(); j != puzzle.board[i].end(); j++){
            if(nines && cols % 3 == 0) stream << "| ";
            stream << *j << " ";
            cols++;
        }
        if(nines) stream << "|\n";
        else stream << endl;
    }

    // Bottom board border
    if(nines) stream << "  +-------+-------+-------+\n";
    return stream;
}

// Enables the easy reading of properly formatted files into a Puzzle object
template<typename T>
istream& operator>>(istream& stream, Puzzle<T>& puzzle){
    T data; //tmp variable to store current output of stream

    // Nested for loops to traverse the grid
    for(int i = 0; i < puzzle.dimension; i++){
        for(int j = 0; j < puzzle.dimension; j++){

            stream >> data; //collect stream value
            puzzle.board[i][j] = data; //write to board

            // For first cell, check character type and set 'word' boolean
            if(isalpha(puzzle.board[i][j])){
                puzzle.word = true;
                // Optional: report the type of board being read
                // cout << boost::format("Reading a %s puzzle\n") % (puzzle.word?"Wordoku":"Sudoku");
            }
        }
    }

    return stream;
}

// First constructor: board of 0's
template<typename T>
Puzzle<T>::Puzzle(int dim_in){
    dimension = dim_in;
    T default_val = 0;
    for(int i = 0; i < dimension; i++){
        vector<T> tmp;
        board.push_back(tmp);
        for(int j = 0; j < dimension; j++)
            board[i].push_back(default_val);
    }
}

// Second constructor: read pre-opened stream
// Might adapt to accept stdin
template<typename T>
Puzzle<T>::Puzzle(fstream& fs){
    // Open streams should be given to 3rd constructor
    if(!fs.is_open()){
        cout << "Error: constructor requires open fstream\n";
        return;
    }

    // Determine width of board
    dimension = readDim(fs);

    // Set vectors to appropriate lengths
    T default_val = 0;
    for(int i = 0; i < dimension; i++){
        vector<T> tmp;
        board.push_back(tmp);
        for(int j = 0; j < dimension; j++)
            board[i].push_back(default_val);
    }

    // Read file data
    fs >> (*this);
}

// Third constructor: takes filename as string
template<typename T>
Puzzle<T>::Puzzle(string fname){

    // Open the filestream and get length
    fstream fs;
    fs.open(fname.c_str(), fstream::in);
    dimension = readDim(fs);
    if(!word){
        fs.close();
        fs.open(fname.c_str(), fstream::in);
    }

    // Initialize board
    T default_val = 0;
    for(int i = 0; i < dimension; i++){
        vector<T> tmp;
        board.push_back(tmp);
        for(int j = 0; j < dimension; j++)
            board[i].push_back(default_val);
    }

    // Read file data and close stream
    fs >> (*this);
    fs.close();
}

// Gets the width of a puzzle board by couting the valid
// characters of the file's first line
template<typename T>
int Puzzle<T>::readDim(fstream& fs){
    char line[256]; int result = 0;
    fs.getline(line, 256);
    word = isalpha(line[0]);
    for(unsigned int i = 0; i < strlen(line); i += 2){
        if(isalnum(line[i])) result++;
        //if(!vecHas(legalvals, word?ine[i]:(i + 1)) )
            legalvals.push_back(word?line[i]:result);
    }
    return result;
}

// Determines whether or not a vector has duplicated values
// Poor time complexity, yes, but excellent space complexity
template<typename T>
bool Puzzle<T>::isunique(vector<T> vec_in){
    typename vector<T>::const_iterator i, j;
    for(i = vec_in.begin(); i != vec_in.end(); i++){
        if(*i == 0 || *i == '0') continue;
        // Scan the values behind i for equivalence
        for(j = i + 1; j != vec_in.end(); j++){
            if(*i == *j) return false;
        }
    }
    return true;
}

// Solve the puzzle
template<typename T>
bool Puzzle<T>::solve(void){

    // Possvals holds vectors of values that are legal at a given (x,y)
    vector<vector<vector<T>>> possvals;

    // Inflate possvals to have high enough dimension
    for(int row = 0; row < dimension; row++){
        possvals.push_back(vector<vector<T>>());
        for(int col = 0; col < dimension; col++)
            possvals[row].push_back(vector<T>());
    }

    // n will be used to iterate over the private vector of legal values
    typename vector<T>::const_iterator n;
    typename vector<T>::const_reverse_iterator r;

    // Populate possvals: iterate over every position
    for(int row = 0; row < dimension; row++)
        for(int col = 0; col < dimension; col++)
            for(n = legalvals.begin(); n != legalvals.end(); n++)
                if(board[row][col] == 0 && checkpos(*n, row, col))
                    possvals[row][col].push_back(*n);

    // Backup the values of board
    vector<vector<T>> backup = board;
    do{
        backup = board;
        placeSingletons(possvals);
        resetPoss(possvals);
        typename vector<T>::const_iterator uut;

        // check possvals by row
        for(int i = 0; i < dimension; i++)
            for(int j = 0; j < dimension; j++)
                for(uut = possvals[i][j].begin(); uut != possvals[i][j].end(); uut++){
                    bool only_n_in_row = true;
                    bool only_n_in_col = true;
                    
                    // hold row constant
                    for(int col = 0; col < dimension; col++){
                        if(col == j) continue;
                        for(unsigned int test = 0; test < possvals[i][col].size(); test++){
                            if(possvals[i][col][test] == *uut){
                                only_n_in_row= false;
                                break;
                            }
                        }
                        if(!only_n_in_row) break;
                    }
                    
                    // hold col constant
                    if(!only_n_in_row){//only need to do if uut wasn't uniq in row

                        for(int row = 0; row < dimension; row++){
                            if(row == i) continue;
                            for(unsigned int test = 0; test < possvals[row][j].size(); test++){
                                if(possvals[row][j][test] == *uut){
                                    only_n_in_col = false;
                                    break;
                                }
                            }
                            if(!only_n_in_col) break;
                        }
                    }
                    
                    if(only_n_in_row || only_n_in_col){
                        board[i][j] = *uut;
                        possvals[i][j].clear();
                        resetPoss(possvals);
                        break; //move to next cell
                    }

                    // reset possvals
                    resetPoss(possvals);
                }

        // reset possvals
        resetPoss(possvals);
    }while(!victory() && backup != board);

    return victory();
}


// Interactive mode
template<typename T>
void Puzzle<T>::play(void){

    // Initialize vars to collect usr input
    char usrIn = 0; int row, col;
    bool innerbreak = false, innerct = false, check_char, check_pos,
         uniqRow, uniqCol, uniqSub;
    cout << "Enter 'q' to quit the game.\nEnter 't' to toggle allow-duplicates.\nEnter 's' to solve the board.\n";
    T tmp_board_val;

    // Play. It's while(true) because I use a few breaks
    while(true){
        // Reset the innerbreak bool set inside the nested while
        innerbreak = false; innerct = false;

        // Collect cell coordinates
        cout << "row> ";
        cin >> usrIn;
        if(usrIn == 'q') break;
        if(usrIn == 's'){
            solve();
            break;
        }
        if(usrIn == 't'){
            allowDups = !allowDups;
            continue;
        }
        row = usrIn - 48 - 1; // -48 char->int, -1 to get index
        cout << "col> ";
        cin >> usrIn;
        if(usrIn == 'q') break;
        if(usrIn == 's'){
            solve();
            break;
        }
        if(usrIn == 't'){
            allowDups = !allowDups;
            continue;
        }
        col = usrIn - 48 - 1;

        // Collect VALID usr input for cell value
        do{
            // Get the value, quit if specified
            cout << "val> ";
            cin >> usrIn;
            if(usrIn == 'q'){
                innerbreak = true;
                break;
            }
            if(usrIn == 's'){
                solve();
                innerbreak = true;
                break;
            }
            if(usrIn == 't'){
                allowDups = !allowDups;
                innerct = true;
                break;
            }

            // Validate character type, convert char numeral to int if need be
            check_char = (isalpha(usrIn) && !word) || (!isalpha(usrIn) && word);
            //TODO replace above with vecHas(legalvals, usrIn);
            
            // Convert usrIn from char to int if need be
            if(!word) usrIn -= 48;
            
            // Row/col checks require value to be placed, so we save the existing val here
            // and place the user's input
            tmp_board_val = board[row][col];
            board[row][col] = usrIn;
            
            // See if the move conflicts with existing board setup
            uniqRow = isunique(getRow(row));
            uniqCol = isunique(getCol(col));

            // The sub-grid check requires the pre-move coniguration, so we replace the original value
            board[row][col] = tmp_board_val;
            uniqSub = check3x3(usrIn, row, col);

            // The correctness of a move is the logical combination of the above tests
            //check_pos = uniqRow && uniqCol && uniqSub- checkpos runs all three tests
            check_pos = checkpos(usrIn, row, col);

            // Prompt for further input if invalid
            if(check_char) cout << boost::format("Please enter a %s\n") % (word?"letter":"number");
            if(!check_pos) cout << boost::format("There's already a %c in that %s %s %s\n") % (word?usrIn:((char)(usrIn+48))) % (uniqRow?"":"row") % (uniqCol?"":"col") % (uniqSub?"":"grid");

        }while(check_char || (!allowDups && !check_pos));

        // Quit if need be, set new cell value, print board, check for victory
        if(innerbreak) break;
        if(innerct) continue;
        board[row][col] = usrIn;
        cout << (*this);
        if(victory()) break;

    } //end while(true)
} //end Puzzle::play

// Determine if the board has been solved
template<typename T>
bool Puzzle<T>::victory(void){
    
    vector<T> tmp = getRow(0);
    has0s(tmp);

    // Check rows
    for(int i = 0; i < dimension; i++)
        if(!isunique(board[i]) || has0s(board[i])) return false;

    // Check cols
    for(int i = 0; i < dimension; i++){
        vector<T> tmp = getCol(i); //assign col vals to tmp vector
        if(!isunique(tmp) || has0s(tmp)) return false;
    }

    // Rows and cols being ALL correct is logically equivalent
    // to the sub-grids being solved.

    return true;
}

// Determine if a vector still has 0's in it
template<typename T>
bool Puzzle<T>::has0s(vector<T> vec_in){
    typename vector<T>::const_iterator i;
    for(i = vec_in.begin(); i != vec_in.end(); i++)
        if(*i == '0' || *i == 0) return true;
    return false;
}

template<typename T>
bool Puzzle<T>::check3x3(T elem, int x, int y){
    while(x % 3 != 0) x--;
    while(y % 3 != 0) y--;
    for(int i = x; i < (x + 3); i++)
        for(int j = y; j < (y + 3); j++)
            if(board[i][j] == elem) return false;
    return true;
}

template<typename T>
vector<T> Puzzle<T>::getRow(int i){
    if(i < dimension) return board[i];
    return vector<T>();//empty vector
}

template<typename T>
vector<T> Puzzle<T>::getCol(int i){
    vector<T> result;
    if(i >= dimension) return result;
    for(int j = 0; j < dimension; j++)
        result.push_back(board[j][i]);
    return result;
}

template<typename T>
bool Puzzle<T>::checkpos(T elem, int x, int y){
    return
        !VecFunc::vecHas(getRow(x), elem) &&
        !VecFunc::vecHas(getCol(y), elem) &&
        check3x3(elem, x, y);
}

template<typename T>
bool Puzzle<T>::placeSingletons(vector<vector<vector<T>>>& vec){
    bool result = false;
    for(int row = 0; row < dimension; row++)
        for(int col = 0; col < dimension; col++)
            if(vec[row][col].size() == 1){
                board[row][col] = vec[row][col][0];
                vec[row][col].pop_back();
                result = true;
            }
    return result;
}

// Reset possval
template<typename T>
void Puzzle<T>::resetPoss(vector<vector<vector<T>>>& vec){
    typename vector<T>::const_iterator n;
    for(int row = 0; row < dimension; row++)
        for(int col = 0; col < dimension; col++)
            vec[row][col].clear();
    for(int row = 0; row < dimension; row++)
        for(int col = 0; col < dimension; col++)
            for(n = legalvals.begin(); n != legalvals.end(); n++)
                if(board[row][col] == 0 && checkpos(*n, row, col))
                    vec[row][col].push_back(*n);
}

#endif
