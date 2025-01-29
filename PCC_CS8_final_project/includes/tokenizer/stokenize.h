#ifndef STOKENIZE_H
#define STOKENIZE_H

#include <cstring>
#include <iostream>
#include "stk_token.h"
#include "state_machine_functions.h"
#include "constant.h"
#include "../table/typedefs.h"
using namespace std;

class STokenizer
{
public:
    //Construtors
    STokenizer();
    STokenizer(char str[]);

    bool done();            //true: there are no more tokens
    bool more();            //true: there are more tokens

    //extract one token (very similar to the way cin >> works)
    friend STokenizer& operator >> (STokenizer& s, STKToken& t);

    //getting each token string and put them into a vectorstr (similar to operator>>)
    vectorstr get_vector_string();

    //set a new string as the input string
    void set_string(const char str[]);

    int get_post(); //return the current position on the input

private:
    //create table for all the tokens we will recognize (e.g. doubles, words, etc.)
    void make_table(int _table[][MAX_COLUMNS]);

    //extract the longest string that match one of the acceptable token types
    bool get_token(int& start_state, string& token);
    bool try_token(char input[], int& pos, int& state, int table[][MAX_COLUMNS], string& t);
    //---------------------------------
    char _buffer[MAX_BUFFER];       //input string
    int _pos;                       //current position in the string
    static int _table[MAX_ROWS][MAX_COLUMNS];

};

#endif