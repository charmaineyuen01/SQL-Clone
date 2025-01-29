#ifndef CONSTANT_CPP
#define CONSTANT_CPP

#include <string>

const int MAX_COLUMNS = 256;
const int MAX_ROWS = 40;
const int MAX_BUFFER = 200;

//arrays for the state machine for stk
const char ALFA[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char DIGITS[] = "0123456789";
const char OPERATORS[] = "!+-%&|";
const char SPACES[] = {' ', '\t', '\n', '\0'};
const char PUNC[] = "?.,:;'`~!";
const char COMPARE[] = "><";
const char NAME[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ .";
const char OTHERS[] = "()*";

#endif