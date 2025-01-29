#ifndef MULTIMAP_H
#define MULTIMAP_H

#include <iostream>
#include "bplustree.h"

using namespace std;

template <typename K, typename V>
struct MPair{
    K key;
    vector<V> value_list;

    //--------------------------------------------------------------------------------
    /*      MPair CTORs:
     *  must have these CTORs:
     *  - default CTOR / CTOR with a key and no value: this will create an empty vector
     *  - CTOR with a key AND a value: pushes the value into the value_list
     *  _ CTOR with a key and a vector of values (to replace value_list)
     */
    //--------------------------------------------------------------------------------
    MPair(const K& k=K()){key = k;}

    MPair(const K& k, const V& v):key(k), value_list(){value_list.push_back(v);}

    MPair(const K& k, const vector<V>& vlist){
        key = k;
        value_list = vlist;
    }
    //--------------------------------------------------------------------------------
    //You'll need to overlod << for your vector:
    friend ostream& operator <<(ostream& outs, const MPair<K, V>& print_me){
        outs << "Key:" << print_me.key << " | Value: ";
        for (int i = 0; i < print_me.value_list.size(); i++){
            cout << print_me.value_list[i] << ", ";
        }
        return outs;
    }
    
    friend bool operator ==(const MPair<K, V>& lhs, const MPair<K, V>& rhs){
        return (lhs.key == rhs.key);
    }

    friend bool operator < (const MPair<K, V>& lhs, const MPair<K, V>& rhs){
        return lhs.key < rhs.key;
    }

    friend bool operator <= (const MPair<K, V>& lhs, const MPair<K, V>& rhs){
        return lhs.key <= rhs.key;
    }

    friend bool operator > (const MPair<K, V>& lhs, const MPair<K, V>& rhs){
        return lhs.key > rhs.key;
    }

    friend bool operator >= (const MPair<K, V>& lhs, const MPair<K, V>& rhs){
        return lhs.key >= rhs.key;
    }

    friend MPair<K, V> operator + (const MPair<K, V>& lhs, const MPair<K, V>& rhs){
        MPair<K,V> a(lhs.key, lhs.value_list);
        for (int i = 0; i < rhs.value_list.size(); i++){
            a.value_list.push_back(rhs.value_list.at(i));
        }
        return a;
    }
};

template <typename K, typename V>
class MMap
{
public:
    typedef BPlusTree<MPair<K, V> > map_base;
    class Iterator{
    public:
        friend class MMap;
        Iterator(typename map_base::Iterator it = nullptr) : _it(it) {}
        Iterator operator ++(int unused){return _it++;}
        Iterator operator ++(){return ++_it;}
        MPair<K, V> operator *(){return *_it;}
        friend bool operator ==(const Iterator& lhs, const Iterator& rhs){
            return lhs._it == rhs._it;
        }

        friend bool operator !=(const Iterator& lhs, const Iterator& rhs){
            return lhs._it != rhs._it;
        }

    private:
        typename map_base::Iterator _it;
    };

    MMap():mmap(true){}

//  Iterators
    Iterator begin(){return mmap.begin();}
    Iterator end(){return mmap.end();}

//  Capacity
    int size() const{return mmap.size();}
    bool empty() const{return mmap.empty();}

//  Element Access
    const vector<V>& operator[](const K& key) const{
        return mmap.get((key)).value_list;
    }

    vector<V>& operator[](const K& key){
        return mmap.get((key)).value_list;
    }
    // const vector<V>& operator[](const K& key) const;
    // vector<V>& operator[](const K& key);
    //^ canvas only had these two where did you come up with the two below?

    vector<V>& at(const K& key){
        return mmap.get(MPair<K,V>(key)).value_list;
    }

//  Modifiers
    void insert(const K& k, const V& v){
        mmap.insert(MPair<K,V>(k,v));
    }

    void insert(const K& k) {
        mmap.insert(MPair<K, V>(k));
    }

    void erase(const K& key){
        mmap.remove(key);
    }

    void clear(){
        mmap.clear_tree();
    }

//  Operations:
    bool contains(const K& key) const{return mmap.contains(key);}

    vector<V> &get(const K& key){
        return mmap.get((key)).value_list;
    }

    Iterator find(const K& key){
        return Iterator(mmap.find((key)));
    }

    bool contains(const MPair<K, V>& target) const{
        return mmap.contains((target));
    }

    int count(const K& key){return mmap.size();}

    Iterator lower_bound(const K& key){
        return Iterator(mmap.lower_bound(key));
    }

    Iterator upper_bound(const K& key){
        return Iterator(mmap.upper_bound(key));
    }

    //equal_range:

    bool is_valid(){return mmap.is_valid();}

    friend ostream& operator<<(ostream& outs, const MMap<K, V>& print_me){
        outs<<print_me.mmap<<endl;
        return outs;
    }

    void print_lookup(){
        for (auto it = begin(); it != end(); ++it) {
            const MPair<K, V>& pair = *it;
            cout << setw(12) << pair.key << ": ";
            for (const auto& value : pair.value_list) {
                cout << value << "|";
            }
            cout << endl;
        }
    }

private:
    BPlusTree<MPair<K, V> > mmap;
};

#endif