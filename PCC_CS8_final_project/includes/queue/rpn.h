#ifndef RPN_H
#define RPN_H

//stack and queue
#include "MyQueue.h"
#include "MyStack.h"
//token
#include "../token/logical.h"
#include "../token/relational.h"
#include "../token/tokenstr.h"
#include "../token/resultset.h"
//table
#include "../table/table.h"
using namespace std;

vectorlong rpn(Queue<Token*> condition, vector<mmap_sl> _table, vectorstr _fields){
    Stack<Token*> stack;
    Queue<Token*>::Iterator it = condition.begin();
    Relational r;
    Logical l;
    while (it != condition.end()) {
        Token* currentToken = *it;
        if (currentToken->get_type() == TOKEN_STR) {
            stack.push(currentToken);
        }
        else if (currentToken->get_type() == RELATIONAL)
        {
            Token* rightTemp = stack.pop();
            TokenStr* right = static_cast<TokenStr*>(rightTemp);
            Token* leftTemp = stack.pop();
            TokenStr* left = static_cast<TokenStr*>(leftTemp);
            //if (rightTemp->get_type() != leftTemp->get_type()){throw 108;}
            Relational* op = static_cast<Relational*>(currentToken);
            stack.push(new ResultSet(r.relational(*left, *right, *op, _table, _fields)));
        }
        else if (currentToken->get_type() == LOGICAL)
        {
            ResultSet* right = static_cast<ResultSet*>(stack.pop());
            ResultSet* left = static_cast<ResultSet*>(stack.pop());
            Logical* op = static_cast<Logical*>(currentToken);
            stack.push(new ResultSet(l.logical(*left, *right, *op)));
        }
        it++;
    }
    ResultSet* result = static_cast<ResultSet*>(stack.top());
    if (!stack.one_left()){throw 107;}
    vectorlong finalResult = result->str_to_vectorlong();
    return(finalResult);
}

#endif