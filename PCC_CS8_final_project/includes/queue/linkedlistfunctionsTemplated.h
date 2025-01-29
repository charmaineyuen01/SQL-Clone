#ifndef LINKEDLISTFUNCTIONSTEMPLATED_H
#define LINKEDLISTFUNCTIONSTEMPLATED_H

#include "node.h"
#include <iostream>

//Linked List General Functions:
template <typename T>
void _print_list(node<T>* head){
    node<T>* walker = head;
    cout << "Head-> ";
    while (walker != nullptr)
    {
        cout << "<-[" << walker -> _item << "]-> ";
        walker = walker -> _next;
    }
    cout << "|||" << endl;
}

//recursive fun! :)
template <typename T>
void _print_list_backwards(node<T> *head){}

//return ptr to key or NULL
template <typename T>
node<T>* _search_list(node<T>* head, T key){
    node<T>* walker = head;
    while (walker != nullptr && walker -> _item != key)
    {
        walker = walker -> _next;
    }
    return walker;
}

template <typename T>
node<T>* _insert_head(node<T> *&head, T insert_this){
    node<T>* n = new node<T>;
    n -> _item = insert_this;
    if (head != nullptr){
        head -> _prev = n;
        n -> _next = head;
        head = n;
    }
    head = n;
    return head;
}

//insert after ptr: insert head if marker null
template <typename T>
node<T>* _insert_after(node<T>*& head, node<T> *after_this, T insert_this){
    if (head == nullptr)
    {
        return _insert_head(head, insert_this);
    }
    else if (after_this -> _next == nullptr){
        node<T>* n = new node<T>;
        n -> _item = insert_this;
        after_this -> _next = n;
        n -> _prev = after_this;
        n -> _next = nullptr;
        return n;
    }
    else{
        node<T>* n = new node<T>;
        n -> _item = insert_this;
        //forward linking
        n -> _next = after_this -> _next;
        after_this -> _next = n;
        //backward linking
        n -> _prev = after_this;
        _following_node(n) -> _prev = n; 
        return n;
    }
}

//insert before ptr: insert head if marker null
template <typename T>
node<T>* _insert_before(node<T>*& head, node<T>* before_this, T insert_this){   
    if (head == nullptr || before_this == head)
    {
        return _insert_head(head, insert_this);
    }
    else{
        node<T>* n = new node<T>;
        n -> _item = insert_this;
        //backward linking
        n -> _prev = before_this -> _prev;
        before_this -> _prev = n;
        //forward linking
        n -> _next = before_this;
        _previous_node(n) -> _next = n;
        return n;
    }
}

//ptr to previous node
template <typename T>
node<T>* _previous_node(node<T>* prev_to_this){
    return prev_to_this -> _prev;
}

//ptr to following node
template <typename T>
node<T>* _following_node(node<T>* next_to_this){
    return next_to_this -> _next;
}

//delete, return item
template <typename T>
T _delete_node(node<T>*& head, node<T>* delete_this){
        //delete head
    if (delete_this == head)
    {   
        if(delete_this -> _next != nullptr){
            _following_node(delete_this) -> _prev = nullptr;
            head = delete_this -> _next;
        }
        else{head = nullptr;}
    }
    //delete tail
    else if(delete_this -> _next == nullptr){
        _previous_node(delete_this) -> _next = nullptr;
    }
    else{
        //forward linking
        _previous_node(delete_this) -> _next = delete_this -> _next;
        //backward linking
        _following_node(delete_this) -> _prev = delete_this -> _prev;
    }
    delete_this -> _next = nullptr;
    delete_this -> _prev = nullptr;
    T item = delete_this -> _item;
    delete delete_this;
    return item;
}

//duplicate the list...
template <typename T>
node<T>* _copy_list(node<T>* head){
    node<T>* dest = nullptr;
    node<T>* newTail = _copy_list(dest, head);
    return dest;
}

//duplicate the list, return pointer to last node in dest... 
//use this function in your queue big three 
template <typename T>
node<T> *_copy_list(node<T> *&dest, node<T> *src){
    _clear_list(dest);
    node<T>* destWalker = dest;
    node<T>* walker = src;

    while(walker != nullptr){
        destWalker = _insert_after(dest, destWalker, walker ->_item);
        walker = walker->_next;
    }
    return destWalker;
}

//delete all the nodes
template <typename T>
void _clear_list(node<T>*& head){
    while (head) {
        _delete_node(head, head);
    }
}

//_item at this position
template <typename T>
T& _at(node<T>* head, int pos){
    node<T>* walker = head;
    for (int i = 0; i < pos - 1; i++)
    {
        walker = walker -> _next;
    }
    return walker -> _item;
}

#endif