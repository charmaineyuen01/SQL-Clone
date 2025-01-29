#ifndef MYQUEUE_CPP
#define MYQUEUE_CPP

#include "MyQueue.h"

//big 3
template <typename T>
Queue<T>::Queue(){
    _size = 0;
    _front = nullptr;
    _rear = nullptr;
}

template <typename T>
Queue<T>::Queue(const Queue<T>& copyMe){
    _front = nullptr;
    _rear = _copy_list(_front, copyMe._front);
    _size = copyMe._size;
}

template <typename T>
Queue<T>::~Queue(){
    _clear_list(_front);
}
//end of big 3

template <typename T>
Queue<T>& Queue<T>::operator=(const Queue<T>& RHS){
    _size = RHS._size;
    _rear = _copy_list(_front, RHS._front);
    return *this;
}

template <typename T>
bool Queue<T>::empty(){
    return _front == nullptr;
}

template <typename T>
T Queue<T>::front(){
    return _front -> _item;
}

template <typename T>
T Queue<T>::back(){
    return _rear -> _item;
}

template <typename T>
void Queue<T>::push(T item){
    _size++;
    if (_front == nullptr) {
        _front = _rear = _insert_head(_front, item);
    } else {
        _rear = _insert_after(_front, _rear, item);
    }
}

template <typename T>
T Queue<T>::pop(){
    assert(_front != nullptr);
    _size--;
    return _delete_node(_front, _front);
}

template <typename T>
typename Queue<T>::Iterator Queue<T>::begin() const{
    return Iterator(_front);
}

template <typename T>
typename Queue<T>::Iterator Queue<T>::end() const{
    return Iterator(nullptr);
}

template <typename TT>
ostream& operator<< (ostream& outs, const Queue<TT>& printMe){
    node<TT>* walker = printMe._front;
    outs << "Head-> ";
    while (walker != nullptr)
    {
        outs << "<-[" << walker -> _item << "]-> ";
        walker = walker -> _next;
    }
    outs << "|||" << endl;
    return outs;
}

#endif