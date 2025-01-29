#ifndef SHUNTING_H
#define SHUNTING_H

#include "rpn.h"
//token (not in queue)
#include "../token/lparen.h"
#include "../token/rparen.h"
using namespace std;

vectorlong shunting(vectorstr condition, vector<mmap_sl> _table, vectorstr _fields){
    Stack<Token*> stack;
    Queue<Token*> queue;
    for (int i = 0; i < condition.size(); ++i) {
        if (condition[i] == "("){
            stack.push(new Lparen(condition[i]));
        }
        else if (condition[i] == ")"){
            while (!stack.empty() && stack.top()->get_type() != LPAREN){
                if(stack.one_left()){throw 105;}
                queue.push(stack.pop());
            }
            Token* temp2 = stack.pop();
        }
        else if (condition[i] == "and"){
            while (!stack.empty() && stack.top()->get_type() == RELATIONAL){
                queue.push(stack.pop());
            }
            stack.push(new Logical(condition[i]));
        }
        else if (condition[i] == "or"){
            while (!stack.empty() && (stack.top()->get_type() == RELATIONAL || stack.top()->get_token_str() == "and")){
                queue.push(stack.pop());
            }
            stack.push(new Logical(condition[i]));
        }
        else if (condition[i]=="="|| condition[i]=="<"|| condition[i]=="<="|| condition[i]==">"|| condition[i]==">="){
            stack.push(new Relational(condition[i]));
        }
        else{
            queue.push(new TokenStr(condition[i]));
        }
    }
    while (!stack.empty()){
        if (stack.top()->get_type() == LPAREN){throw 106;}
        queue.push(stack.pop());
    }    
    return(rpn(queue, _table, _fields));
}


#endif