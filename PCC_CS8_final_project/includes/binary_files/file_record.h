#ifndef FILE_RECORD_H
#define FILE_RECORD_H

#include <iostream>
#include <cstring>
#include <string>
#include <iomanip>
#include <vector>
#include "utilities.h"
using namespace std;

//----------------------------------------------------------------------------
//                      C L A S S   R E C O R D
//----------------------------------------------------------------------------

class FileRecord{
public:
    //when you construct a Record, it's either empty or it contains a word
    FileRecord(){
        for(int i = 0; i < MAX; i++){
            _record[i][0] = '\0';
        }
        recno = -1;
    }

    FileRecord(vector<char*> str){
        for(int i = 0; i < MAX; i++){
            _record[i][0] = '\0';
        }
        for (int i = 0; i < MAX && i < str.size(); i++){
            strncpy(_record[i], str[i], MAX);
        }
    }

    FileRecord(vector<string> s){
        for(int i = 0; i < MAX; i++){
            _record[i][0] = '\0';
        }
        for(int i = 0; i < s.size(); i++){
            strncpy(_record[i], s[i].c_str(), MAX);
        }
    }

    //returns the record number
    long write(fstream& outs);

    //returns the number of bytes; read = MAX, or zero if read passed the end of file      
    long read(fstream& ins, long recno);    

    friend ostream& operator<<(ostream& outs, const FileRecord& r);

    static const int MAX = 100;
    char _record[MAX][MAX+1];
    int recno;

private:
    //static const int MAX = 100;
    //int recno;
    //char _record[MAX][MAX+1];
};

long FileRecord::write(fstream &outs){
    //r.write(f); //take the Record r and write it into file f
    //and return the record number of this Record

    //write to the end of the file.
    outs.seekg(0, outs.end);

    //retrieves the current position of the file pointer
    long pos = outs.tellp();    

    //outs.write(&record[0], sizeof(record));
    for(int i = 0; i < MAX; i++){
        outs.write(_record[i], sizeof(_record[i]));
    }

    return pos/sizeof(_record);  //record number
}

long FileRecord::read(fstream &ins, long recno){
    //returns the number of bytes read.
    //    r.read(f, 6);
    //    cout<<r<<endl;
    long pos = recno * sizeof(_record);
    ins.seekg(pos, ios_base::beg);

    //ins.read(&_record[0], sizeof(_record))
    //don't you want to mark the end of  the cstring with null?
    //_record[] => [zero'\0'trash trash trash trash]
    //don't need the null character, but for those of us with OCD and PTSD:
   
    for(int i = 0; i < MAX; i++){
        ins.read(_record[i], sizeof(_record[i]));
        _record[i][ins.gcount()] = '\0';
    }

    return ins.gcount();
}

ostream& operator<<(ostream& outs, const FileRecord& r){
    for(int i = 0; r._record[i][0]!='\0'; i++){
        outs << setw(20) <<  r._record[i];
    }
    return outs;
}

#endif