#ifndef RELATIONAL_H
#define RELATIONAL_H

#include "token.h"
#include "tokenstr.h"
#include "../table/table.h"
using namespace std;

class Relational : public Token
{
public:
    Relational():Token(){}

    Relational(string str): Token(str, TokenType::RELATIONAL){}

    string relational(TokenStr left, TokenStr right, Relational op, vector<mmap_sl> _table, vectorstr _fields){
        //get the index of the field that is comparing
        int fieldIndex;
        for (int i = 0; i < _fields.size(); i++){
            if (_fields[i] == left.get_token_str()){
                fieldIndex = i;
                break;
            } 
        }
        _r_select_list.clear();
        //from mmap get the temp._select_list
        string condition = right.get_token_str();
        if (op.get_token_str() == "="){
            _r_select_list = _table[fieldIndex].at(condition); //return a vector
        }
        else if (op.get_token_str() == "<"){
            for (mmap_sl::Iterator it = _table[fieldIndex].begin(); it != _table[fieldIndex].lower_bound(condition); ++it) {
                for (const auto& recno : (*it).value_list) {
                    _r_select_list.push_back(recno); //add values into the vector one by one
                }
            }
        }
        else if (op.get_token_str() == "<="){
            for (mmap_sl::Iterator it = _table[fieldIndex].begin(); it != _table[fieldIndex].upper_bound(condition); ++it) {
                for (const auto& recno : (*it).value_list) {
                    _r_select_list.push_back(recno);
                }
            }
        }
        else if (op.get_token_str() == ">"){
            for (mmap_sl::Iterator it = _table[fieldIndex].upper_bound(condition); it != _table[fieldIndex].end(); ++it) {
                for (const auto& recno : (*it).value_list) {
                    _r_select_list.push_back(recno);
                }
            }
        }
        else if (op.get_token_str() == ">="){
            for (mmap_sl::Iterator it = _table[fieldIndex].lower_bound(condition); it != _table[fieldIndex].end(); ++it) {
                for (const auto& recno : (*it).value_list) {
                    _r_select_list.push_back(recno);
                }
            }
        } 
        return vectorlong_to_str(_r_select_list);  
    }

    //changing the vector of long into a string, to pass as a token
    //matching with str_to_vectorlong() in resultset.h
    string vectorlong_to_str(vectorlong values){
        string temp = "";
        for (int i = 0; i < values.size(); i++){
            temp += to_string(values[i]) + " ";
        }
        return temp;
    }

private:
    string _str;
    vectorlong _r_select_list;   
};

#endif