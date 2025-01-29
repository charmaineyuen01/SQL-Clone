#ifndef TABLE_H
#define TABLE_H

#include <cmath>
#include <iostream>
#include <iomanip>
#include <set>
#include <vector>
#include <fstream>
#include <algorithm>
#include "typedefs.h"
#include "../binary_files/file_record.h"
//
#include "../queue/rpn.h"
#include "../queue/shunting.h"
using namespace std;

class Table
{
public:
    Table(){_table_name = "";}

    Table(string tableName): _table_name(tableName){
        //get _fields from a .txt
        string tempFile = tableName + ".txt";
        fstream txtFile;
        open_fileRW(txtFile, tempFile.c_str());
        if (txtFile.is_open()) {
            string temp;
            while (txtFile >> temp) {
                _fields.push_back(temp);
            }
            txtFile.close();
        } 
        else{cout << "Unable to open file(txt) for reading.\n";}
        //initialize mmap in _table
        for (int i = 0; i < _fields.size(); i++){
            mmap_sl MMAP;
            _table.push_back(MMAP);
        }
        //READ INFO FROM BIN AND ADD TO TABLE
        //not using insert_into, becoz not writing into the bin
        //open old binary file
        string oldFile = tableName + ".bin";
        fstream binFile;
        open_fileRW(binFile, oldFile.c_str());
        //use insert_into
        if (binFile.is_open()) {
            FileRecord record;
            long recno = 0;
            while (record.read(binFile, recno) > 0) {
                // Convert the 2D char array to a vector of strings
                vectorstr oneRecord;
                for (int i = 0; i < FileRecord::MAX && record._record[i][0] != '\0'; i++) {
                    oneRecord.push_back(string(record._record[i]));
                }
                //insert into table
                for (int i = 0; i < oneRecord.size(); i++){
                    _table[i].insert(oneRecord[i], recno);
                }
                //insert into _recno_list;
                _recno_list.push_back(recno); 
                recno++;
            }
            //close the files
            binFile.close();
        } 
        else{cout << "Unable to open file(bin) for reading.\n";}
    }

    Table(string tableName, vectorstr fieldNames): _table_name(tableName), _fields(fieldNames){
        //output the _fields into a .txt
        string tempFile = tableName + ".txt";
        fstream outFile;
        open_fileW(outFile, tempFile.c_str());
        if (outFile.is_open()) {
            for (int i = 0; i < fieldNames.size(); i++){
                outFile << fieldNames[i] << " ";
            }
            outFile.close();
        } 
        else{cout << "Unable to open file for reading.\n";}
        //initialize mmap in _table
        for (int i = 0; i < fieldNames.size(); i++){
            mmap_sl MMAP;
            _table.push_back(MMAP);
        }
        //clear binary file
        string bin = tableName + ".bin";
        fstream binFile;
        open_fileW(binFile, bin.c_str());
        if (binFile.is_open()) {
            binFile.clear();
            binFile.close();
        } 
    }

    void insert_into(vectorstr info){
        //create the 2D record 
        FileRecord record(info);
        //for binary file
        fstream outFile;
        string tempFile = _table_name + ".bin";
        open_fileRW(outFile, tempFile.c_str());
        long recno = record.write(outFile);
        outFile.close();
        //insert into table
        for (int i = 0; i < info.size(); i++){
            _table[i].insert(info[i], recno);
        }
        //insert into _recno_list;
        _recno_list.push_back(recno); 
    }

    Table select_all(){
        //construct the table
        Table temp(_table_name + to_string(_serial_number), _fields);
        _select_list = _recno_list;
        _serial_number++;
        //open old binary file
        string oldFile = _table_name + ".bin";
        fstream inFile;
        open_fileRW(inFile, oldFile.c_str());
        //use insert_into
        if (inFile.is_open()) {
            FileRecord record;
            long recno = 0;
            while (record.read(inFile, recno) > 0) {
                // Convert the 2D char array to a vector of strings
                vectorstr oneRecord;
                for (int i = 0; i < FileRecord::MAX && record._record[i][0] != '\0'; i++) {
                    oneRecord.push_back(string(record._record[i]));
                }
                // Insert the record data into the new table
                temp.insert_into(oneRecord);
                recno++;
            }
            //close the files
            inFile.close();
        } 
        else{cout << "Unable to open file for reading.\n";}
        return temp;
    }

    Table select_field(vectorstr fieldNames){
        //construct the table
        Table temp(_table_name + to_string(_serial_number), fieldNames);
        _select_list = _recno_list;
        _serial_number++;
        //open old binary file
        string oldFile = _table_name + ".bin";
        fstream inFile;
        open_fileRW(inFile, oldFile.c_str());
        //USE INSERT_INTO
        if (inFile.is_open()) { //check if the oldBin is opened
            FileRecord record; //create the object of FileRecord
            //loop through each INDEX in the NEW _select_list
             for (long recno : _select_list) {
                record.read(inFile, recno);
                //Convert the 2D char array to a vector of strings
                //create a vector int to store which line in the record is needed
                vector<int> rows;
                for (int i = 0; i < fieldNames.size(); i++){
                    for (int j = 0; j < _fields.size(); j++){
                        if (fieldNames[i] == _fields[j]){
                            rows.push_back(j);
                        }
                    }  
                }
                //push_back to a vectorstr for each row needed
                vectorstr oneRecord;
                for (int i = 0; i < rows.size(); i++) {
                    oneRecord.push_back(string(record._record[rows[i]]));
                }
                // Insert the record data into the new table
                temp.insert_into(oneRecord);
            }
            //close the files
            inFile.close();
        }
        else{cout << "Unable to open file for reading.\n";}
        return temp;
    }

    //NEED TO FIX THE DUPLICATES FOR AGE
    Table select(vectorstr fieldNames, string field, string op, string condition){
        // need to change the designed field to the first
        Table temp(_table_name + "_" + to_string(_serial_number), fieldNames);
        _serial_number++;
        //get the index of the field that is comparing
        int fieldIndex;
        for (int i = 0; i < fieldNames.size(); i++){
            if (_fields[i] == field){
                fieldIndex = i;
                break;
            } 
        }
        //clear the _select_list
        _select_list.clear();
        //from mmap get the temp._select_list
        if (op == "="){
            _select_list = _table[fieldIndex].at(condition);
        }
        else if (op == "<"){
            for (mmap_sl::Iterator it = _table[fieldIndex].begin(); it != _table[fieldIndex].lower_bound(condition); ++it) {
                for (const auto& recno : (*it).value_list) {
                    _select_list.push_back(recno);
                }
            }
        }
        else if (op == "<="){
            for (mmap_sl::Iterator it = _table[fieldIndex].begin(); it != _table[fieldIndex].upper_bound(condition); ++it) {
                for (const auto& recno : (*it).value_list) {
                    _select_list.push_back(recno);
                }
            }
        }
        else if (op == ">"){
            for (mmap_sl::Iterator it = _table[fieldIndex].upper_bound(condition); it != _table[fieldIndex].end(); ++it) {
                for (const auto& recno : (*it).value_list) {
                    _select_list.push_back(recno);
                }
            }
        }
        else if (op == ">="){
            for (mmap_sl::Iterator it = _table[fieldIndex].lower_bound(condition); it != _table[fieldIndex].end(); ++it) {
                for (const auto& recno : (*it).value_list) {
                    _select_list.push_back(recno);
                }
            }
        } 
        //read from the old record from .bin to get the whole record
        //open old binary file
        string oldFile = _table_name + ".bin";
        fstream inFile;
        open_fileRW(inFile, oldFile.c_str());
        //USE INSERT_INTO
        if (inFile.is_open()) { //check if the oldBin is opened
            FileRecord record; //create the object of FileRecord
            //loop through each INDEX in the NEW _select_list
            for (long recno : _select_list) {
                record.read(inFile, recno);
                // Convert the 2D char array to a vector of strings
                vectorstr oneRecord;
                //push_back the condition field first
                oneRecord.push_back(string(record._record[fieldIndex]));
                for (int i = 0; i < FileRecord::MAX && record._record[i][0] != '\0'; i++) {
                    if (i != fieldIndex){ 
                        oneRecord.push_back(string(record._record[i]));
                    }
                }
                // Insert the record data into the new table
                temp.insert_into(oneRecord);
            }
            //close the files
            inFile.close();
        } 
        else{cout << "Unable to open file for reading.\n";}
        return temp;      
    }

    Table select(vectorstr fieldNames, Queue<Token*> condition){
        // need to change the designed field to the first
        Table temp(_table_name + "_" + to_string(_serial_number), fieldNames);
        _serial_number++;
        _select_list.clear();
        _select_list = rpn(condition, _table, _fields);
        cout << "select: " << _select_list << endl;
        //read from the old record from .bin to get the whole record
        //open old binary file
        string oldFile = _table_name + ".bin";
        fstream inFile;
        open_fileRW(inFile, oldFile.c_str());
        //USE INSERT_INTO
        if (inFile.is_open()) { //check if the oldBin is opened
            FileRecord record; //create the object of FileRecord
            //loop through each INDEX in the NEW _select_list
            for (long recno : _select_list) {
                record.read(inFile, recno);
                //Convert the 2D char array to a vector of strings
                //create a vector int to store which line in the record is needed
                vector<int> rows;
                for (int i = 0; i < fieldNames.size(); i++){
                    for (int j = 0; j < _fields.size(); j++){
                        if (fieldNames[i] == _fields[j]){rows.push_back(j);}
                    }  
                }
                //push_back to a vectorstr for each row needed
                vectorstr oneRecord;
                for (int i = 0; i < rows.size(); i++) {
                    oneRecord.push_back(string(record._record[rows[i]]));
                }
                // Insert the record data into the new table
                temp.insert_into(oneRecord);
            }
            //close the files
            inFile.close();
        } 
        else{cout << "Unable to open file for reading.\n";}
        return temp;
    }

    Table select(vectorstr fieldNames, vectorstr condition){
        // need to change the designed field to the first
        Table temp(_table_name + "_" + to_string(_serial_number), fieldNames);
        _serial_number++;
        _select_list.clear();
        _select_list = shunting(condition, _table, _fields);
        //read from the old record from .bin to get the whole record
        //open old binary file
        string oldFile = _table_name + ".bin";
        fstream inFile;
        open_fileRW(inFile, oldFile.c_str());
        //USE INSERT_INTO
        if (inFile.is_open()) { //check if the oldBin is opened
            FileRecord record; //create the object of FileRecord
            //loop through each INDEX in the NEW _select_list
             for (long recno : _select_list) {
                record.read(inFile, recno);
                //Convert the 2D char array to a vector of strings
                //create a vector int to store which line in the record is needed
                vector<int> rows;
                for (int i = 0; i < fieldNames.size(); i++){
                    for (int j = 0; j < _fields.size(); j++){
                        if (fieldNames[i] == _fields[j]){rows.push_back(j);}
                    }  
                }
                //push_back to a vectorstr for each row needed
                vectorstr oneRecord;
                for (int i = 0; i < rows.size(); i++) {
                    oneRecord.push_back(string(record._record[rows[i]]));
                }
                // Insert the record data into the new table
                temp.insert_into(oneRecord);
            }
            //close the files
            inFile.close();
        } 
        else{cout << "Unable to open file for reading.\n";}
        return temp;
    }

    vectorlong select_recnos()const{return _select_list;}

    vectorstr get_fields(){return _fields;}

    vector<mmap_sl> get_table(){return _table;}

    string get_table_name(){return _table_name;}

    friend ostream& operator<<(ostream& outs, const Table& print_me){
        print_me.print(outs);
        return outs;
    }

    void print(ostream& outs = cout) const{
        //print the member varaible of the class
        outs << "Table name: " << _table_name << ", records: " << _recno_list.size() << endl;
        outs << setw(20) << "record";
        for (int i = 0; i < _fields.size(); i++){
            outs << setw(20) << _fields[i];
        }
        outs << endl << endl;
        //print from the binary file
        string tempFile = _table_name + ".bin";
        fstream inFile;
        open_fileRW(inFile, tempFile.c_str());
        if (inFile.is_open()) {
            FileRecord record;
            long recno = 0;
            //while (record.read(inFile, recno) > 0) {
            while(record.read(inFile, recno) > 0 && recno < _recno_list.size()){
                outs<< setw(20) << recno << record << endl;
                recno++;
            }
            inFile.close();
        } 
        else{outs << "Unable to open file for reading.\n";}
    }

    friend Table operator+ (const Table& lhs, const Table& rhs){
        Table a = lhs + rhs;
        return a;
    }

private:
    int ROW = 20;
    int COLUMN = 18;

    int _serial_number = 1000;
    vector<mmap_sl> _table;
    vectorstr _fields;
    string _table_name;
    vectorlong _recno_list;
    vectorlong _select_list;
};


#endif