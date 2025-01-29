#ifndef RESULTSET_H
#define RESULTSET_H

#include "token.h"
using namespace std;

class ResultSet : public Token
{
public:
    ResultSet():Token(){}

    ResultSet(string str): Token(str, TokenType::RESULT_SET){_str = str;}

    //using private variable _str, change to a vectorlong
    //the str is a list of numbers, with space between each number (matching with logical and relational)
    vectorlong str_to_vectorlong(){
        vector<long> temp;
        size_t start = 0;
        size_t end = _str.find(' '); //end when seeing a space, saying that the number ended
        while (end != string::npos) {
            if (end > start) {
                long value = stol(_str.substr(start, end - start));
                temp.push_back(value);
            }
            start = end + 1; //incrementing to ignore the space
            end = _str.find(' ', start);
        }
        return temp;
    }

private:
    string _str;
   
};

#endif