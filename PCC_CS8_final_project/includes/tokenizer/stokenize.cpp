#include "stokenize.h"
using namespace std;

int STokenizer::_table[MAX_ROWS][MAX_COLUMNS];

//Constructors
STokenizer::STokenizer(){
    set_string("");
    make_table(_table);
    _pos = 0;
}

STokenizer::STokenizer(char str[]){
    set_string(str);
    make_table(_table);
    _pos = 0;
}

//true: there are no more tokens
bool STokenizer::done(){return _pos > strlen(_buffer);}

//true: there are more tokens       
bool STokenizer::more(){return _pos < strlen(_buffer);}

//extract one token (very similar to the way cin >> works)
STokenizer& operator>> (STokenizer& s, STKToken& t){
    int state = 0;
    string tempToken = "";
    if (s.get_token(state, tempToken)){
        t = STKToken(tempToken, state);
    }
    return s;
}

//getting each token string and put them into a vectorstr (similar to operator>>)
vectorstr STokenizer::get_vector_string(){
    vectorstr temp;
    int initial_pos = _pos;
    while (more()){ //when hasn't reach the end of the input
        int state = 0; //reset the state to 0 after getting each token
        string tempToken = ""; //reset the token
        if (get_token(state, tempToken)){ //if we can get a token
            //only push_back to the vectorstr when the token is the type we want
            if (state == 1|| state == 2|| state == 8|| state == 9|| state == 12|| state == 13|| state == 14){
                temp.push_back(tempToken);
            }
        }
        else{_pos++;}
    }
    return temp;
}

//set a new string as the input string
void STokenizer::set_string(const char str[]){
    strcpy(_buffer, str);
    _pos = 0;
}

int STokenizer::get_post(){return _pos;} //return the current position on the input

//create table for all the tokens we will recognize (e.g. doubles, words, etc.)
//reference my own word document 
void STokenizer::make_table(int _table[][MAX_COLUMNS]){
    init_table(_table);
    //Success State
    for (int i = 1; i <= 5; i++){mark_success(_table, i);}
    mark_fail(_table, 0);
    mark_fail(_table, 6);
    mark_success(_table, 7);
    mark_success(_table, 8);
    mark_success(_table, 9);
    //NUMBER
    mark_cells(0, _table, DIGITS, 1);
    mark_cells(1, _table, DIGITS, 1);
    mark_cells(1, _table, ".", 6);
    mark_cells(6, _table, DIGITS, 7);
    mark_cells(7, _table, DIGITS, 7);
    //ALPHA
    mark_cells(0, _table, ALFA, 2);
    mark_cells(2, _table, ALFA, 2);
    //SPACE
    mark_cells(0, _table, SPACES, 3);
    mark_cells(3, _table, SPACES, 3);
    //OPERATOR
    mark_cells(0, _table, OPERATORS, 4);
    //PUNC
    mark_cells(0, _table, PUNC, 5);
    // == 
    mark_cells(0, _table, "=", 8);
    //COMPARE ><
    mark_cells(0, _table, COMPARE, 9);
    mark_cells(9, _table, "=", 8);
    //TAKE CARE OF \" 
    mark_fail(_table, 10);
    mark_fail(_table, 11);
    mark_success(_table, 12);
    // \"
    mark_cell(0, _table, '\"', 10);
    // name with space
    mark_cells(10, _table, NAME, 11);
    mark_cells(11, _table, NAME, 11);
    // end of name
    mark_cell(11, _table, '\"', 12);
    //TAKE CARE OF () and *
    mark_success(_table, 13);
    mark_cells(0, _table, OTHERS, 13);
}

//extract the longest string that match one of the acceptable token types
bool STokenizer::get_token(int& start_state, string& token){
    return try_token(_buffer, _pos, start_state, _table, token);
}

bool STokenizer::try_token(char input[], int& pos, int& state, int table[][MAX_COLUMNS], string& t){
    int lastSuccessPos = pos;
    bool success = false; //determine if it is currently at a success state
    int lastSuccessState = -1;
    string tempT = "";

    while (more()){ //when hasn't reach the end of the input
        state = table[state][input[pos]]; //update to the current state
        //fail state
        if (state == -1){
            state = lastSuccessState; //return to the last success state
            if (!success){ //delete anything after the last success state
                pos = lastSuccessPos;
                t= tempT;
            }
            break; //break the while(more) loop and return a token
        }
        if (input[pos] != '"'){t += input[pos];} //only add to token when not "; taking care of \"
        pos++;
        //for success state; all things added are kept
        if (is_success(table, state)){
            success = true;
            lastSuccessState = state;
            lastSuccessPos = pos;
            tempT = t;
        }
        else{//for fail state; just adding char to token; can delete anytime
            success = false;
        }
    }
    return !t.empty(); //return a bool to determine if we successfully gain a token
}