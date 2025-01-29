#ifndef LOGICAL_H
#define LOGICAL_H

#include "token.h"
#include "resultset.h"
using namespace std;

class Logical : public Token
{
public:
    Logical():Token(){}

    Logical(string str): Token(str, TokenType::LOGICAL){}

    string logical(ResultSet left, ResultSet right, Logical op){
        //getting a vectorlong from the string token, so that each number is read seperately
        vectorlong leftLong = left.str_to_vectorlong();
        vectorlong rightLong = right.str_to_vectorlong();
        vectorlong temp;
        if (op.get_token_str() == "and"){
            for (int i = 0; i < leftLong.size(); i++){
                for (int j = 0; j < rightLong.size(); j++){
                    if (leftLong[i] == rightLong[j]){ //when lhs == rhs
                        temp.push_back(leftLong[i]);
                    } 
                }
            }
        }
        else if (op.get_token_str() == "or"){
            // Add elements from leftLong
            for (int i = 0; i < leftLong.size(); i++){ //add the whole lhs
                temp.push_back(leftLong[i]);
            }   
            // Add elements from rightLong that are not already present in temp
            for (int i = 0; i < rightLong.size(); i++){
                bool found = false;
                for (int j = 0; j < temp.size(); j++){
                    if (rightLong[i] == temp[j]){
                        found = true; //stop going through the temp vectorlong after finding presence
                        break;
                    }
                }
                if (!found){temp.push_back(rightLong[i]);} //only adding when not found
            }
        }
        return vectorlong_to_str(temp);
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
};

#endif