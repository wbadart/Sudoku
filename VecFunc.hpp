#ifndef VECFUNC_H
#define VECFUNC_H

#include <iostream>
#include <vector>
#include <boost/format.hpp>
using namespace std;

// Contains a couple vector-related helper functions
// I namespaced it since there's some overlap with Puzzle
// member functions.
namespace VecFunc{
    template<typename T>
    bool vecHas(vector<T> vec_in, T elem);

    template<typename T>
    vector<T> getRow(vector<vector<T>> vec, int n);


    template<typename T>
    vector<T> getCol(vector<vector<T>> vec, int n);

    template<typename T>
    bool check3x3(vector<vector<T>> vec, T elem, int row, int col);

    template<typename T>
    bool checkpos(vector<vector<T>> vec, T elem, int row, int col);
    
    template<typename T>
    void print1(vector<T> vec);
    template<typename T>
    void print2(vector<vector<T>> vec);
    template<typename T>
    void print3(vector<vector<vector<T>>> vec);
};

template<typename T>
bool VecFunc::vecHas(vector<T> vec_in, T elem){
    typename vector<T>::const_iterator i;
    for(i = vec_in.begin(); i != vec_in.end(); i++)
        if(elem == *i) return true;
    return false;
}

// Return the nth row of a two dimensional vector
template<typename T>
vector<T> VecFunc::getRow(vector<vector<T>> vec, int n){
    return vec[n];
}

// Return the nth column of a 2D vector
template<typename T>
vector<T> VecFunc::getCol(vector<vector<T>> vec, int n){
    typename vector<T>::const_iterator row;
    vector<T> result;
    for(row = vec.begin(); row != vec.end(); row++)
        result.push_back(*row);
    return result;
}

// Checks around a subgrid in a board
template<typename T>
bool VecFunc::check3x3(vector<vector<T>> vec, T elem, int row, int col){
    while(row % 3 != 0) row--;
    while(col % 3 != 0) col--;
    for(int i = row; i < (row + 3); i++)
        for(int j = col; j < (col + 3); j++)
            if(vec[row][col] == elem) return false;
    return true;
}

// Perform all tests on an element in an arbitrary 2D board
template<typename T>
bool VecFunc::checkpos(vector<vector<T>> vec, T elem, int row, int col){
    return (
        !vecHas(getRow(vec, row), elem) &&
        !vecHas(getCol(vec, col), elem) &&
        check3x3(vec, elem, row, col)
    );
}

// Print an N-dimensional array; mainly made for gdb
template<typename T>
void VecFunc::print1(vector<T> vec){
    for(unsigned int val = 0; val < vec.size(); val++)
        cout << vec[val] << "  ";
    cout << endl;
}
template<typename T>
void VecFunc::print2(vector<vector<T>> vec){
    typename vector<T>::const_iterator val;
    for(unsigned int i = 0; i < vec.size(); i++){
        for(val = vec[i].begin(); val != vec[i].end(); val++)
            cout << *val << "  ";
        cout << endl;
    }
}
template<typename T>
void VecFunc::print3(vector<vector<vector<T>>> vec){
    typename vector<T>::const_iterator val;
    for(unsigned int i = 0; i < vec.size(); i++){
        for(unsigned int j = 0; j < vec.size(); j++){
            cout << boost::format("(%d, %d)\n") % i % j;
            for(val = vec[i][j].begin(); val != vec[i][j].end(); val++)
                cout << *val << "  ";
            cout << endl << endl;
        }
        cout << endl << endl;
    }
}

#endif
