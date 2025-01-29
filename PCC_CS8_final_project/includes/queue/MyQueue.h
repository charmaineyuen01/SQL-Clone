#ifndef MYQUEUE_H
#define MYQUEUE_H

#include "linkedlistfunctionsTemplated.h"
using namespace std;

template <typename T>
class Queue
{
public:
    class Iterator{
    public:
        friend class Queue;                               //give access to list to access _ptr
        Iterator(){_ptr = NULL;}                          //default ctor
        Iterator(node<T>* p){_ptr = p;}                            //Point Iterator to where
                                                          //  p is pointing to
        T operator *(){return _ptr -> _item;}    //dereference operator
        bool is_null(){return _ptr == nullptr;}            //true if _ptr is NULL
        friend bool operator !=(const Iterator& left, const Iterator& right)  //true if left != right
        {return left._ptr != right._ptr;}
        friend bool operator ==(const Iterator& left, const Iterator& right)  //true if left == right
        {return left._ptr == right._ptr;}

        //member operator:++it or ++it = new_value
        Iterator& operator++(){
            return *this;
        }

        //friend operator: it++
        friend Iterator operator++(Iterator& it, int unused){
            assert(it._ptr!=NULL);
            it._ptr = it._ptr -> _next;
            return ++it;
        }

    private:
        node<T>* _ptr;                          //pointer being encapsulated
    };

    Queue();
    Queue(const Queue<T>& copyMe);
    ~Queue();
    Queue& operator=(const Queue<T>& RHS);

    bool empty();
    T front();
    T back();

    void push(T item);
    T pop();

    Iterator begin() const;                                     //Iterator to the head node
    Iterator end() const;                                       //Iterator to NULL
    void print_pointers();
    int size() const { return _size;}
    
    template<typename TT>
    friend ostream& operator << (ostream& outs, const Queue<TT>& printMe);

private:
    node<T>* _front;
    node<T>* _rear;
    int _size;
};

#include "MyQueue.cpp"
#endif