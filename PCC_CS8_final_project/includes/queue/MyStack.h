#ifndef MYSTACK_H
#define MYSTACK_H

#include "linkedlistfunctionsTemplated.h"
using namespace std;

template <typename ITEM_TYPE>
class Stack{
public:
    class Iterator{
    public:
        friend class Stack;                 //give access to list to access _ptr
        Iterator(){_ptr = NULL;}            //default ctor
        Iterator(node<ITEM_TYPE>* p): _ptr(p){}      //Point Iterator to where p is pointing to

        //dereference operator
        ITEM_TYPE operator *(){return _ptr -> _item;}
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
        node<ITEM_TYPE>* _ptr;    //pointer being encapsulated
    };

    Stack();
    Stack(const Stack<ITEM_TYPE>& copyMe);
    ~Stack();

    Stack<ITEM_TYPE>& operator=(const Stack<ITEM_TYPE>& RHS);
    ITEM_TYPE top();
    bool empty();
    bool one_left();
    void push(ITEM_TYPE item);
    ITEM_TYPE pop();

    template<typename T>
    friend ostream& operator<< (ostream& outs, const Stack<T>& printMe);

    Iterator begin() const;                   //Iterator to the head node
    Iterator end() const;                     //Iterator to NULL
    int size() const {return _size;}

private:
    node<ITEM_TYPE>* _top;
    int _size;
};

#include "MyStack.cpp"
#endif