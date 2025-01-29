#ifndef SQL_H
#define SQL_H

#include <iostream>
#include "../table/table.h"
#include "../parser/parser.h"
using namespace std;

class SQL{
public:

SQL(){
    //get table_name from a .txt
    fstream txtFile;
    open_fileRW(txtFile, "tableNames.txt");
    if (txtFile.is_open()) {
        string temp;
        while (txtFile >> temp) {
            _table = Table(temp);
            _storage.insert(_table.get_table_name(), _table);
        }
        txtFile.close();
    } 
    else{cout << "Unable to open file for reading.\n";}
}

Table command(string command){
    //get the _ptree
    _parser.set_string(command);
    _ptree = _parser.parse_tree();
    //if statements for (make, insert, select)
    if (_ptree.get("command")[0] == "make"){
        _table = Table(_ptree.get("table_name")[0], _ptree.get("col")); //get the table name for the Table constructor
        _storage.insert(_table.get_table_name(), _table);
        //output the table_name into a .txt
        fstream outFile;
        open_fileRW(outFile, "tableNames.txt");
        if (outFile.is_open()) {
            outFile << _table.get_table_name() << " ";
            outFile.close();
        } 
        else{cout << "Unable to open file for reading.\n";}
        return _table;
    }
    else if (_ptree.get("command")[0] == "insert"){
        if (_storage.contains(_ptree.get("table_name")[0])){
            _table = _storage.at(_ptree.get("table_name")[0]);
        }
        else{
            _table = Table(_ptree.get("table_name")[0]);
        }
        _table.insert_into(_ptree.get("values")); //insert new things into the table
        return _table;
    }
    else if (_ptree.get("command")[0] == "select"){
        _table = Table(_ptree.get("table_name")[0]);
        if (_ptree.get("fields")[0] == "*"){ //selecting all fields
            if (!_ptree.contains("where")){ //no condition
                return _table.select_all(); 
            }
            else{ //with condition
                return _table.select(_table.get_fields(), _ptree.get("condition"));
            }
        }
        else{ //only selected fields
            if (!_ptree.contains("where")){ //no condition
                return _table.select_field(_ptree.get("fields"));
            }
            else{ //with condition
                return _table.select(_ptree.get("fields"), _ptree.get("condition"));
            }
        }
    }
}

vectorlong select_recnos()const{return _table.select_recnos();}

const vectorstr batch = {
// 1. valid command:
    "select lname, fname, major from student where ((lname=Yang or major=CS) and age<23 )or lname=Jackson",
// 2. Expected comma:
    //"select lname fname, major from student where ((lname=Yang or major=CS) and age<23 )or lname=Jackson",
    "select * from employee",
// 3. Expected: field name
    //"select lname, , major from student where ((lname=Yang or major=CS) and age<23 )or lname=Jackson",
    "select * from student",
// 4. Expected: from
    "select lname, fname, major  student where ((lname=Yang or major=CS) and age<23 )or lname=Jackson",
// 5. Expected: table name
    "select lname, fname, major from  where ((lname=Yang or major=CS) and age<23 )or lname=Jackson",
// 6. Expected: condition
    "select lname, fname, major from student where",
// 7. Missing left paren
    "select lname, fname, major from student where (lname=Yang or major=CS) and age<23 )or lname=Jackson",
// 8. Missing right paren
    "select lname, fname, major from student where ((lname=Yang or major=CS and age<23 )or lname=Jackson",

// 9. Missing Yang
"select lname, fname, major from student where ((lname= or major=CS) and age<23 )or lname=Jackson",
// 10. Nothing wrong 
"select lname, fname, major from student where ((lname=Yang or major=CS) and age<23 )or lname=Jackson",
// 11.  Missing or
"select lname, fname, major from student where ((lname=Yang  major=CS) and age<23 )or lname=Jackson",
// 12. missing major = CS
"select lname, fname, major from student where ((lname=Yang or ) and age<23 )or lname=Jackson",
// 13. missing lname=Jackson
 "select lname, fname, major from student where ((lname=Yang or major=CS) and age<23 )or", 
// 14. nothing wrong
"select lname, fname, major from student where ((lname=Yang or major=CS) and age<23 )or lname=Jackson"
};

void run(){
    Table t;
    string userInput = "";
    cout << "FINAL PRESENTATION: " << endl<<endl;

    for (int i = 0; i < batch.size(); i++){
        cout << "Command " << i + 1 << ": " << batch[i] << endl;
        try{
            t = command(batch[i]);
            cout << "Records selected: " << select_recnos() << endl;  
            cout << t << endl;
        } catch (int e){
            if (e == 100){cout  << "Expected: from" << endl;}
            else if (e == 101){cout  << "Expected: table name" << endl;}
            else if (e == 102){cout  << "Expected: condition" << endl;}
            else if (e == 103){cout << "Error Occured in Parser." << endl;}
            else if (e == 104){cout << "Invalid Condition." << endl;}
            else if (e == 105){cout << "Missing left paren." << endl;}
            else if (e == 106){cout << "Missing right paren." << endl;}
            else if (e == 107){cout << "Invalid Condition." << endl;}
            //else if (e == 108){cout << "Missing Relational." << endl;}
            else{cerr << "Error Occured." << endl;}
        }
        cout << endl;
    }

    while (userInput != "finished working"){
        getline(cin, userInput);
        if (userInput == "finished working"){break;}
        try{
            t = command(userInput);
            cout << "Records selected: " << select_recnos() << endl;  
            cout << t << endl;
        } catch (int e){
            if (e == 100){cout  << "Expected: from" << endl;}
            else if (e == 101){cout  << "Expected: table name" << endl;}
            else if (e == 102){cout  << "Expected: condition" << endl;}
            else if (e == 103){cout << "Error Occured in Parser." << endl;}
            else if (e == 104){cout << "Invalid Condition." << endl;}
            else if (e == 105){cout << "Missing left paren." << endl;}
            else if (e == 106){cout << "Missing right paren." << endl;}
            else if (e == 107){cout << "Invalid Condition." << endl;}
            //else if (e == 108){cout << "Missing Relational." << endl;}
            else{cerr << "Error Occured." << endl;}
        }
        cout << endl;
    }

    cout << "Thank you for using the database." << endl << endl;

    
}

private:
Parser _parser;
mmap_ss _ptree;
Table _table;
Map<string, Table> _storage;
};


#endif