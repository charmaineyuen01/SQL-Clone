#ifndef MYSTACK_CPP
#define MYSTACK_CPP

#include "MyStack.h"

//big three
template <typename ITEM_TYPE>
Stack<ITEM_TYPE>::Stack(){
    _top = nullptr;
    _size = 0;
}

template <typename ITEM_TYPE>
Stack<ITEM_TYPE>::Stack(const Stack<ITEM_TYPE>& copyMe){
    _top = _copy_list(copyMe._top);
    _size = copyMe._size;
}

template <typename ITEM_TYPE>
Stack<ITEM_TYPE>::~Stack(){
    _clear_list(_top);
}
//end of big three

template <typename ITEM_TYPE>
Stack<ITEM_TYPE>& Stack<ITEM_TYPE>::operator=(const Stack<ITEM_TYPE>& RHS){
    _size = RHS._size;
    _copy_list(_top, RHS._top);
    return *this;
}

//need to check starting here
template <typename ITEM_TYPE>
ITEM_TYPE Stack<ITEM_TYPE>::top(){
    return _top -> _item;
}

template <typename ITEM_TYPE>
bool Stack<ITEM_TYPE>::empty(){
    return _top == nullptr;
}

template <typename ITEM_TYPE>
bool Stack<ITEM_TYPE>::one_left(){
    return _top ->_next == nullptr;
}

template <typename ITEM_TYPE>
void Stack<ITEM_TYPE>::push(ITEM_TYPE item){
    _size++;
    _top = _insert_head(_top, item);
}

template <typename ITEM_TYPE>
ITEM_TYPE Stack<ITEM_TYPE>::pop(){
    if(_top == nullptr){throw 104;}
    //assert(_top != nullptr);
    _size--;
    return _delete_node(_top, _top);
}

template<typename T>
ostream& operator<< (ostream& outs, const Stack<T>& printMe){
    node<T>* walker = printMe._top;
    outs << "Head-> ";
    while (walker != nullptr)
    {
        outs << "<-[" << walker -> _item << "]-> ";
        walker = walker -> _next;
    }
    outs << "|||" << endl;
    return outs;
}

template <typename ITEM_TYPE>
typename Stack<ITEM_TYPE>::Iterator Stack<ITEM_TYPE>::begin() const{
    return Iterator(_top);
}

template <typename ITEM_TYPE>
typename Stack<ITEM_TYPE>::Iterator Stack<ITEM_TYPE>::end() const{
    return Iterator(nullptr);
}

#endif