#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <assert.h>
#include "../table/typedefs.h"
using namespace std;

enum TokenType{
    TOKEN, LPAREN, RPAREN, TOKEN_STR, OPERATOR, RELATIONAL, LOGICAL, RESULT_SET, 
    TOKEN_NUMBER, TOKEN_ALPHA, TOKEN_SPACE, TOKEN_OPERATOR, TOKEN_PUNC, TOKEN_UNKNOWN, TOKEN_END
};

class Token
{
public:
    //Constructor
    Token() : _token(""), _type(TokenType::TOKEN){}
    Token(string str, TokenType type): _token(str), _type(type){}

    friend ostream& operator <<(ostream& outs, const Token& t){
        outs << t._token;//outs << "|" << t._token << "|";
        return outs;
    }

    string type_string() const{ //return token_type as a readable string
        if (_type == TokenType::TOKEN){return "TOKEN";}
        else if (_type == TokenType::LPAREN){return "LPAREN";}
        else if (_type == TokenType::RPAREN){return "RPAREN";}
        else if (_type == TokenType::TOKEN_STR) {return "TOKEN_STR";}
        else if (_type == TokenType::OPERATOR){return "OPERATOR";}
        else if (_type == TokenType::RELATIONAL){return "RELATIONAL";}
        else if (_type == TokenType::LOGICAL){return "LOGICAL";}
        else if (_type == TokenType::RESULT_SET){return "RESULT_SET";}
        else if (_type == TokenType::TOKEN_NUMBER){return "TOKEN_NUMBER";}
        else if (_type == TokenType::TOKEN_ALPHA){return "TOKEN_ALPHA";}
        else if (_type == TokenType::TOKEN_SPACE){return "TOKEN_SPACE";}
        else if (_type == TokenType::TOKEN_OPERATOR){return "TOKEN_OPERATOR";}
        else if (_type == TokenType::TOKEN_PUNC){return "TOKEN_PUNC";}
        else if (_type == TokenType::TOKEN_UNKNOWN){return "TOKEN_UNKNOWN";}
        else if (_type == TokenType::TOKEN_END){return "TOKEN_END";}
        else {exit(28);}
    }

    //getters and setters
    virtual string get_token_str() const{return _token;}
    virtual void set_token_str(string token){_token = token;}

    virtual TokenType get_type() const{return _type;}
    virtual void set_type(TokenType type){_type = type;}

private:
    string _token;
    TokenType _type;
};

#endif