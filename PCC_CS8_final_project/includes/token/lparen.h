#ifndef LPAREN_H
#define LPAREN_H

#include "token.h"
using namespace std;

class Lparen : public Token
{
public:
    Lparen():Token(){}

    Lparen(string str): Token(str, TokenType::LPAREN){}

private:
    string _str;
   
};

#endif