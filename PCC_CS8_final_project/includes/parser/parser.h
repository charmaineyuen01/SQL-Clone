#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include "../table/table.h"
#include "../tokenizer/stokenize.h"
using namespace std;

class Parser{
public:
    Parser(): _input(""){
        make_state_machine();
        build_keyword_list();
    }

    Parser(string input): _input(input){
        make_state_machine();
        build_keyword_list();
        set_string(input);
    }

    void set_string(string input){
        _stk.set_string(input.c_str());
        _inputVTR = _stk.get_vector_string();
        _ptree.clear();
        not_failing_flag = get_parse_tree();
    }

    int get_column(string tokenstr) {
        if (_keyword.contains(tokenstr)){
            return _keyword.get(tokenstr);
        }
        else{return SYMBOL;}
    } 

    bool get_parse_tree(){
        int previous_state = 0;
        int current_state = 0;
        string token;
        int command_type = -1; //choose between MAKE(1), INSERT(4), SELECT(7); used for the looping symbol(12)
        // get the current token and current_state
        for (int i = 0; i < _inputVTR.size(); i++){ //loop through the whole vectorstr
            token = _inputVTR[i];
            previous_state = current_state; //for exception handling 
            current_state = _state_machine[current_state][get_column(token)];
            if (current_state == -1){break;}
            // adding the token into the _ptree
            else if (current_state == MAKE || current_state == INSERT || current_state == SELECT){
                _ptree.insert("command", token);
                command_type = current_state;
            }
            else if (current_state == 11 || current_state == 14){
                _ptree.insert("table_name", token);
            }
            else if(current_state == WHERE){
                _ptree.insert("where", "yes");
            }
            else if (current_state == 12){
                if (command_type == MAKE){
                    _ptree.insert("col", token);
                }
                else if (command_type == INSERT){
                    _ptree.insert("values", token);
                }
                else if (command_type == SELECT){
                    _ptree.insert("condition", token);
                }
            }
            else if (current_state == 13){
                _ptree.insert("fields", token);
            }
            else if (current_state == STAR){
                _ptree.insert("fields", token);
            }
            else if (current_state == 14){
                _ptree.insert("table_name", token);
            }
        }
        //return true if reaches a success state 
        if (_state_machine[current_state][0] == 1){ //success state
            return true;
        }
        else{ //fail state; state == 0 || state == -1
            _ptree.clear();
            if (previous_state == 8 || previous_state == 13){
                throw 100;
            }
            else if (previous_state == FROM){
                throw 101;
            }
            else if (previous_state == 14){
                throw 102;
            }
            else{throw 103;}
            return false;
        }
    }  

    mmap_ss parse_tree(){return _ptree;}

    //for testing purpose only
    void printVTR(){
        for (int i = 0; i < _inputVTR.size(); i++){cout << _inputVTR[i] << " | ";}
        cout << endl;
    }


private:
    bool not_failing_flag;
    string _input;
    vectorstr _inputVTR;
    STokenizer _stk;
    mmap_ss _ptree;

    Map<string, int> _keyword;

    void build_keyword_list(){
        _keyword.insert("make", MAKE);
        _keyword.insert("table", TABLE);
        _keyword.insert("fields", FIELDS);
        _keyword.insert("insert", INSERT);
        _keyword.insert("into", INTO);
        _keyword.insert("values", VALUES);
        _keyword.insert("select", SELECT);
        _keyword.insert("from", FROM);
        _keyword.insert("*", STAR);
        _keyword.insert("where", WHERE);
    }

    int _state_machine[MAX_ROWS][MAX_COLUMNS];

    void make_state_machine(){
        init_table(_state_machine);
        //Success State
        for (int i = 0; i <= 11; i++){
            mark_fail(_state_machine, i);
        }
        mark_success(_state_machine, 12);
        mark_fail(_state_machine, 13);
        mark_success(_state_machine, 14);
        //mark cells as designed in word document (row, column, state); row by row
        mark_cell(0, _state_machine, MAKE, 1);
        mark_cell(0, _state_machine, INSERT, 4);
        mark_cell(0, _state_machine, SELECT, 7);
        mark_cell(1, _state_machine, TABLE, 2);
        mark_cell(2, _state_machine, SYMBOL, 11);
        mark_cell(3, _state_machine, SYMBOL, 12);
        mark_cell(4, _state_machine, INTO, 5);
        mark_cell(5, _state_machine, SYMBOL, 11);
        mark_cell(6, _state_machine, SYMBOL, 12);
        mark_cell(7, _state_machine, STAR, 8);
        mark_cell(7, _state_machine, SYMBOL, 13);
        mark_cell(8, _state_machine, FROM, 9);
        mark_cell(9, _state_machine, SYMBOL, 14);
        mark_cell(10, _state_machine, SYMBOL, 12);
        mark_cell(11, _state_machine, FIELDS, 3);
        mark_cell(11, _state_machine, VALUES, 6);
        mark_cell(12, _state_machine, SYMBOL, 12);
        mark_cell(13, _state_machine, FROM, 9);
        mark_cell(13, _state_machine, SYMBOL, 13);
        mark_cell(14, _state_machine, WHERE, 10);
    }

    enum KEYS{ZERO, MAKE, TABLE, FIELDS, INSERT, INTO, VALUES, SELECT, STAR, FROM, WHERE, SYMBOL};
};

#endif