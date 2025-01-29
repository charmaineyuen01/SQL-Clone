#ifndef RPAREN_H
#define RPAREN_H

#include "token.h"
using namespace std;

class Rparen : public Token
{
public:
    Rparen():Token(){}

    Rparen(string str): Token(str, TokenType::RPAREN){}

private:
    string _str;
   
};

#endif