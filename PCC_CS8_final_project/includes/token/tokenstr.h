#ifndef TOKENSTR_H
#define TOKENSTR_H

#include "token.h"
using namespace std;

class TokenStr : public Token
{
public:
    TokenStr():Token(){}

    TokenStr(string str): Token(str, TokenType::TOKEN_STR){}

private:
    string _str;
   
};

#endif