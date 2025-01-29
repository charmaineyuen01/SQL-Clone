#ifndef BTREE_ARRAY_FUNCTIONS_H
#define BTREE_ARRAY_FUNCTIONS_H

#include <iostream>
#include <iomanip>
#include <vector>
using namespace std;

template <class T> //return the larger of the two items
T maximal(const T& a, const T& b){return max(a, b);}

template <class T> //swap the two items
void swap(T& a, T& b){
    T temp = a;
    a = b;
    b = temp;
}
/////////////////////////////////////////////////////////////////////

//NEED TO WORK ON
template <class T> //return index of the largest item in data
int index_of_maximal(T data[ ], int n){
    T temp = data[0];
    int index = 0;
    for(int i = 0; i < n; i++){
        if(data[i] > max){    
            max = data[i];
            index = i;
        }
    }
    return index;
}  

template <class T> //insert entry at index i in data
void insert_item(T data[ ], int i, int& n, T entry){
    for (int j = n; j > i; j--){
        data[j] = data[j - 1];
    }
    n++;  
    data[i] = entry;    
}   

template <class T> //insert entry into the sorted array data with length n
void ordered_insert(T data[ ], int& n, T entry){
    int tempIndex = 0;
    while (data[tempIndex] < entry && tempIndex < n){tempIndex++;}
    insert_item(data, tempIndex, n, entry);
}       
                                                      
template <class T> //return the first index in data that is not less than entry
int first_ge(const T data[ ], int n, const T& entry){
    int i = 0;
    if (n == 0 || entry > data[n - 1]){
        return n;
    }
    else{
        while (data[i] < entry && i < n){i++;}
    }
    return i;
} 
                                                       
template <class T> //append entry to the right of data
void attach_item(T data[ ], int& n, const T& entry){
    data[n] = entry;
    n++;
}    

template <class T> //remove the last element in data and place it in entry
void detach_item(T data[ ], int& n, T& entry){
    entry = data[n - 1];
    n--;
}  

template <class T> //delete item at index i and place it in entry
void delete_item(T data[ ], int i, int& n, T& entry){
    entry = data[i];
    for (int j = i; j < n; j++){
        data[j] = data[j + 1];
    }
    n--;
} 

template <class T> //append data2 to the right of data1
void merge(T data1[ ], int& n1, T data2[ ], int& n2){
    for (int i = 0; i < n2; i++){
        attach_item(data1, n1, data2[i]);
    }
    n2 = 0;
}   

template <class T> //move n/2 elements from the right of data1 and move to data
void split(T data1[ ], int& n1, T data2[ ], int& n2){
    //starting index of data1 that need to move to data2 (the CS way: 0, 1, 2)
    int startTemp = n1 -  n1/2 ;
    T entry;
    for (int i = startTemp; i < n1; i++){
        attach_item(data2, n2, data1[i]);
    }
    int originalSize = n1;
    for (int i = startTemp; i < originalSize; i++){
        detach_item(data1, n1, entry);
    }
} 
                                                        
template <class T> //copy src[] into dest[]
void copy_array(T dest[], const T src[], int& dest_size, int src_size){
    dest_size = src_size;
    for (int i = 0; i < src_size; i++){
        dest[i] = src[i];
    }
}          

template <class T> //print array data
void print_array(const T data[], int n, int pos = -1){
    for (int i = 0; i < n; i++){
        cout << data[i] << "|";
    }
    cout << endl;
} 

template <class T> //item > all data[i]
bool is_gt(const T data[], int n, const T& item){
    for (int i = 0; i < n; i++){
        if(item <= data[i]){
            return false;
        }
    }
    return true;
}  

template <class T> //item <= all data[i]
bool is_le(const T data[], int n, const T& item){
    for (int i = 0; i < n; i++){
        if(item > data[i]){return false;}
    }
    return true;
}       

//-------------- Vector Extra operators: ---------------------
template <typename T>
ostream& operator <<(ostream& outs, const vector<T>& list){
    for (int i = 0; i < list.size(); i++){
        outs << list[i] << "|";
    }
    return outs;
} //print vector list

template <typename T>
vector<T>& operator +=(vector<T>& list, const T& addme){
    list.push_back(addme);
    return list;
} //list.push_back addme

#endif