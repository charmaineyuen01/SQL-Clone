#ifndef STK_TOKEN_H
#define STK_TOKEN_H

#include <iostream>
#include <assert.h>

using namespace std;

class STKToken
{
public:
    //Constructor
    STKToken() : _token(""), _type(0){}
    STKToken(string str, int type): _token(str), _type(type){}

    //insertion opeartor
    ostream& operator <<(ostream& outs){
        outs << _token;//outs << "|" << t._token << "|";
        return outs;
    }

    //getters
    int type() const{return _type;}
    string token_str() const{return _token;}

private:
    string _token;
    int _type;
};

#endif