#ifndef BPLUSTREE_H
#define BPLUSTREE_H

#include <iostream>
#include <iomanip>
#include <assert.h> 
#include "btree_array_funcs.h"
using namespace std;

template <class T>
class BPlusTree
{
public:
    class Iterator{
    public:
        friend class BPlusTree;
        Iterator(BPlusTree<T>* _it = NULL, int _key_ptr = 0):it(_it), key_ptr(_key_ptr){}

        T operator *(){return it->data[key_ptr];}

        Iterator operator++(int un_used){
            Iterator pointer = *this;
            if(key_ptr >= it->data_count -1){
                key_ptr = 0;
                it = it->next;
            }
            else{key_ptr++;}
            return pointer;
        }

        Iterator operator++(){
            assert(it);
            if (key_ptr < it->data_count - 1){
                key_ptr++;
            }
            else{
                it = it->next;
                key_ptr = 0;
            }
            return it;
        }

        friend bool operator ==(const Iterator& lhs, const Iterator& rhs){
            return (lhs.key_ptr == rhs.key_ptr && lhs.it == rhs.it);
        }

        friend bool operator !=(const Iterator& lhs, const Iterator& rhs){
            return (lhs.key_ptr != rhs.key_ptr || lhs.it != rhs.it);
        }

        void print_Iterator(){
            if (it){
                cout<<"iterator: ";
                print_array(it->data, it->data_count, key_ptr);
            }
            else{
                cout<<"iterator: NULL, key_ptr: "<<key_ptr<<endl;
            }
        }

        bool is_null(){return !it;}

        void info(){
            cout<<endl<<"Iterator info:"<<endl;
            cout<<"key_ptr: "<<key_ptr<<endl;
            cout<<"it: "<<*it<<endl;
        }

    private:
        BPlusTree<T>* it;
        int key_ptr;
    };

    BPlusTree(bool dups):dups_ok(dups), data_count(0), child_count(0), next(NULL){}

    BPlusTree(T *a, int size, bool dups):dups_ok(dups), data_count(0), child_count(0), next(NULL){
        for (int i = 0; i < size; i++){insert(a[i]);}
    }

    //big three:
    BPlusTree(const BPlusTree<T>& other){copy_tree(other);}
    ~BPlusTree(){clear_tree();}
    BPlusTree<T>& operator =(const BPlusTree<T>& RHS){
        if (this != &RHS){
            if (!this->empty()){
                this->clear_tree();
            }
            this->copy_tree(RHS);
        }
        return *this;
    }

    //copy other into this object
    void copy_tree(const BPlusTree<T>& other){
        BPlusTree* last = NULL;
        copy_tree(other, last);
    }

    void copy_tree(const BPlusTree<T>& other, BPlusTree<T>*& last_node){
        for (int i = 0; i < other.data_count; i++){
            data[i] = other.data[i];
        }
        if (other.is_leaf()){
            if (last_node != NULL){
                last_node->next = this;
            }
            last_node = this;
        }
        
        data_count = other.data_count;
        child_count = other.child_count;
        if (!other.is_leaf()){
            for (int i = 0; i < other.child_count; i++){
                subset[i] = new BPlusTree(dups_ok);
                subset[i]->copy_tree(*other.subset[i], last_node);
            }
        }
    }

    //insert entry into the tree
    void insert(const T& entry){
        //if (!contains(entry)){
            loose_insert(entry);
        //}
        if(data_count > MAXIMUM){
            //creating the new root
            BPlusTree* temp;
            temp = new BPlusTree(dups_ok);
            copy_array(temp->data, data, temp->data_count, data_count);
            copy_array(temp->subset, subset, temp->child_count, child_count);
            //clear original root
            data_count = 0;
            child_count = 1;
            //link original root to new root
            subset[0] = temp;
            fix_excess(0);
        }
    }
    
    //remove entry from the tree
    void remove(const T& entry){
        if(contains(entry)){
            loose_remove(entry);
            if(child_count == 1){
                BPlusTree* other;
                other = subset[0];
                copy_tree(*other);
                other->data_count = 0;
                other->child_count = 0;
                delete other;
            }
        }
    }

    //clear this object (delete all nodes etc.)
    void clear_tree(){
        if (!is_leaf()){
            for (int i = 0; i < child_count; i++) {
                subset[i]->clear_tree();
                delete subset[i];
            }
        }
        else{next = NULL;}
        data_count = 0; 
        child_count = 0; 
    }

    //true if entry can be found
    bool contains(const T& entry) const{
        int pointer;
        pointer = first_ge(data, data_count, entry);
        if (!is_leaf()){
            if (data[pointer] == entry){
                return subset[pointer + 1]->contains(entry);
            }
            else{
                return subset[pointer]->contains(entry);
            }
        }
        else{
            if (data[pointer] == entry){
               return true;
            }
            else{
                return false;
            }
        }
    }

    //return a reference to entry
    T& get(const T& entry){
        //If entry is not in the tree, add it to the tree
        //assert(contains(entry));
        const bool debug = false;
        if (!contains(entry)){
            insert(entry);
        }
        return get_existing(entry);
    }

    //return a reference to entry
    const T& get(const T& entry)const{
        //If entry is not in the tree, add it to the tree
        //assert(contains(entry));
        const bool debug = false;
        if (!contains(entry)){
            insert(entry);
        }
        return get_existing(entry);
    }

    //return a reference to entry
    T& get_existing(const T& entry){
           /*
 * ---------------------------------------------------------------
 * This routing explains plainly how the BPlusTree is organized.
 * i = first_ge
 *       |      found         |    !found         |
 * ------|--------------------|-------------------|
 * leaf  |  you're done       | it's not there    |
 *       |                    |                   |
 * ------|--------------------|-------------------|
 * !leaf | subset[i+1]->get() |  subset[i]->get() |
 *       | (inner node)       |                   |
 * ------|--------------------|-------------------|
 *       |                    |                   |
 *
 * ---------------------------------------------------------------
 *
 */
    //assert that entry is not in the tree.
    //assert(contains(entry));

        const bool debug = false;
        int i = first_ge(data, data_count, entry);
        bool found = (i<data_count && data[i]==entry);
        if (is_leaf()){
            if (found){
                return data[i];
            }
            else{
                if (debug) cout<<"get_existing was called with nonexistent entry"<<endl;
                assert(false);
            }
        }
        if (found){ //inner node
            return subset[i+1]->get(entry);
        }
        //or just return true?
        else{ //not found yet...
            return subset[i]->get(entry);
        }
    }

    //return an iterator to this key. NULL if not there.
    Iterator find(const T& key){
        if (data_count == 0){
            return NULL;
        }
        
        int pointer;
        pointer = first_ge(data, data_count, key);
        if (!is_leaf()){
            if (data[pointer] == key){
                return subset[pointer + 1]->find(key);
            }
            else{
                return subset[pointer]->find(key);
            }
        }
        else{
            if (data[pointer] == key){
               return Iterator(this, pointer);
            }
            else{
                return Iterator();
            }
        }
    }

    //return first that goes NOT BEFORE key entry or next if does not exist: >= entry
    Iterator lower_bound(const T& key){
        for(Iterator it = begin(); it!= end(); it++){
            if (*it >= key){
                return it;
            }
        }
        return Iterator(NULL);

        // if (contains(key)){
        //     return find(key);
        // }
        // else{
        //     int pointer;
        //     pointer = first_ge(data, data_count, key);
        //     if (!is_leaf()){
        //         return subset[pointer]->lower_bound(key);
        //     }
        //     else{
        //         if (data[pointer] < key){
        //             return Iterator(NULL);
        //         }
        //         else{return Iterator(this, pointer);}
        //     } 
        // }
    }

    //return first that goes AFTER key exist or not, the next entry  >entry
    Iterator upper_bound(const T& key){
        for(Iterator it = begin(); it!= end(); it++){
            if (*it > key){
                return it;
            }
        }
        return Iterator(NULL);

        // if (contains(key)){
        //     return ++find(key);
        // }
        // else{
        //     int pointer;
        //     pointer = first_ge(data, data_count, key);

        //     if (!is_leaf()){
        //         return subset[pointer]->upper_bound(key);
        //     }
        //     else{
        //         if (data[pointer] <= key){
        //             return Iterator(NULL);
        //         }
        //         else{return Iterator(this, pointer);}
        //     }
        // } 
    }

    //count the number of elements
    int size() const{
        int tempSize = data_count;
        if (!is_leaf()){
            for (int i = 0; i < child_count; i++){
                tempSize += subset[i]->size();
            }
        }
        return tempSize;
    }

    //true if the tree is empty
    bool empty() const{return size() == 0;}   

    void print_tree(int level = 0,ostream &outs=cout) const{
        if(!is_leaf()){
            subset[child_count - 1]->print_tree(level + 1);
        }
        for (int i = data_count - 1; i >= 0; i--){
            outs << setw(4 * level) << "" << "[" << data[i] << "]" << endl;
            if(!is_leaf()){
                subset[i]->print_tree(level + 1);
            }
        }
    }

    friend ostream& operator<<(ostream& outs,const BPlusTree<T>& print_me){
        print_me.print_tree(0, outs);
        return outs;
    }

    bool is_valid(){return true;}

    string in_order(){
        string in = "";
        if(!is_leaf()){
            in += subset[0]->in_order();
        }
        for (int i = 0; i < data_count; i++){
            in += to_string(data[i]) + "|";
            if(!is_leaf()){
                in += subset[i + 1]->in_order();
            }
        }
        return in;
    }

    string pre_order(){
        string pre = "";
        for (int i = 0; i < data_count; i++){
            pre += to_string(data[i]) + "|";
            if(!is_leaf()){
                pre += subset[i]->pre_order();
            }
        }
        if(!is_leaf()){
            pre += subset[data_count]->pre_order();
        }
        return pre;
    }

    string post_order(){
        string post = "";
        if(!is_leaf()){
            post += subset[0]->post_order();
        }
        for (int i = 0; i < data_count; i++){
            if(!is_leaf()){
                post += subset[i + 1]->post_order();
            }
            post += to_string(data[i]) + "|";
        }
        return post;
    }

    Iterator begin(){
        return Iterator(get_smallest_node());
    }

    Iterator end(){
        return Iterator(NULL);
    }

    ostream& list_keys(Iterator from = NULL, Iterator to = NULL){
        if (from == NULL) from = begin();
        if (to == NULL) to = end();
        for (Iterator it = from; it != to; it++)
            cout<<*it<<" ";
        return cout;
    }

private:
    static const int MINIMUM = 1;
    static const int MAXIMUM = 2 * MINIMUM;

    bool dups_ok = true;                 //true if duplicate keys are allowed
    int data_count;                      //number of data elements
    T data[MAXIMUM + 1];                 //holds the keys
    int child_count;                     //number of children
    BPlusTree* subset[MAXIMUM + 2];      //subtrees
    BPlusTree* next;

    //true if this is a leaf node
    bool is_leaf() const{return child_count == 0;}             

    //return a pointer to this key. NULL if not there.
    T* find_ptr(const T& entry){
        int pointer;
        pointer = first_ge(data, data_count, entry);
        if (!is_leaf()){
            if (data[pointer] == entry){
                return subset[pointer + 1]->find_ptr(entry);
            }
            else{
                return subset[pointer]->find_ptr(entry);
            }
        }
        else{
            if (data[pointer] == entry){
               return *this;
            }
            else{
                return NULL;
            }
        }
    }         

    //insert element functions. allows MAXIMUM+1 data elements in the root
    void loose_insert(const T& entry){
    /*
       int i = first_ge(data, data_count, entry);
       bool found = (i<data_count && data[i] == entry);

       three cases:
         found
         a. found/leaf: deal with duplicates: call +
         b. found/no leaf: subset[i+1]->loose_insert(entry)
                           fix_excess(i+1) if there is a need

         ! found:
         c. !found / leaf : insert entry in data at position i
         c. !found / !leaf: subset[i]->loose_insert(entry)
                            fix_excess(i) if there is a need

            |   found          |   !found         |
      ------|------------------|------------------|-------
      leaf  |a. dups? +        | c: insert entry  |
            |                  |    at data[i]    |
      ------|------------------|------------------|-------
            | b.               | d.               |
            |subset[i+1]->     | subset[i]->      |
      !leaf | loose_insert()   |  loose_insert()  |
            |fix_excess(i+1)   | fix_excess(i)    |
            |                  |                  |
      ------|------------------|------------------|-------
    */
    int i = first_ge(data, data_count, entry);
    bool found = (data[i] == entry && i<data_count);

    //cout << "in loose insert" << endl;
    if (found){
        //cout << "found" << endl;
        if (dups_ok)
        {
            //cout << "dups ok" << endl;
            if (is_leaf()){
                data[i] = data[i] + entry;
            }
            else{
                subset[i + 1]->loose_insert(entry);
                if (subset[i + 1]->data_count > MAXIMUM){
                    fix_excess(i); 
                }
            }
        } 
        else{
            //cout << "dups not ok" << endl;
            if (is_leaf()){
                data[i] = entry;
            }
            else{
                subset[i + 1]->loose_insert(entry);
                if (subset[i + 1]->data_count > MAXIMUM){
                    fix_excess(i); 
                }
            }
        }
    }
    else{
        //cout << "not found" << endl;
        if(is_leaf()){
            ordered_insert(data, data_count, entry);
        } 
        else{
            subset[i]->loose_insert(entry);
            if(subset[i]->data_count > MAXIMUM){
                fix_excess(i);
            }
        }
    }
    } 

    //fix excess in child i
    void fix_excess(int i){
        BPlusTree* temp = new BPlusTree(dups_ok);
        T pushUp;
        //splitting the excess array into two arrays
        split(subset[i]->data, subset[i]->data_count, temp->data, temp->data_count);
        if (!subset[i]->is_leaf()){
            split(subset[i]->subset, subset[i]->child_count, temp->subset, temp->child_count);
        }
        //move the last index of the original array
        detach_item(subset[i]->data, subset[i]->data_count, pushUp);
        insert_item(data, i, data_count, pushUp);
        insert_item(subset, i + 1, child_count, temp);
        if (subset[i]->is_leaf()){
            insert_item(subset[i+1]->data, 0, subset[i+1]->data_count, pushUp);
            
            temp->next = subset[i]->next;
            subset[i]->next = temp;
        }
    }          

    //remove element functions: allows MINIMUM-1 data elements in the root
    void loose_remove(const T& entry){
        if (data[first_ge(data, data_count, entry)] != entry){
            subset[first_ge(data, data_count, entry)]->loose_remove(entry);
            for (int i = 0; i < child_count; i++){
                if (subset[i]->data_count < MINIMUM){
                    fix_shortage(i);
                } 
            }
        }
        else{
            T temp;
            if (is_leaf()){
                delete_item(data, first_ge(data, data_count, entry), data_count, temp);
            }
            else{
                subset[first_ge(data, data_count, entry)]->remove_biggest(temp);
                data[first_ge(data, data_count, entry)] = temp;
                if (subset[first_ge(data, data_count, entry) - 1]->data_count < MINIMUM){
                    fix_shortage(first_ge(data, data_count, entry) - 1);
                } 
            }
        }
    }

    //fix shortage in child i and return the smallest key in this subtree
    BPlusTree<T>* fix_shortage(int i){
        if (i < child_count - 1 && subset[i + 1]->data_count > MINIMUM){
            //cout << endl << "ROTATE LEFT" << endl;
            rotate_left(i);
        }
        else if (i > 0 && subset[i - 1]->data_count > MINIMUM){
            //cout << endl << "ROTATE RIGHT" << endl;
            rotate_right(i);
        }
        else if (subset[i + 1] && i < child_count - 1){
            //cout << endl << "MERGE RIGHT" << endl;
            merge_with_next_subset(i);
        }
        else{
            //cout << endl << "MERGE LEFT" << endl;
            merge_with_next_subset(i - 1);
        }
        return NULL;
    }

    BPlusTree<T>* get_smallest_node(){
        if (!is_leaf())
        {
            return subset[0]->get_smallest_node();
        }
        else{return this;}
    }

    //entry := leftmost leaf
    void get_smallest(T& entry){
        if (!is_leaf()){
            subset[0]->get_smallest;
        }
        else{entry = data[0];}
    }

    //entry := rightmost leaf
    void get_biggest(T& entry){
        if (!is_leaf()){
            subset[child_count - 1]->get_smallest;
        }
        else{entry = data[data_count - 1];}
    }

    //remove the biggest child of tree->entry
    void remove_biggest(T& entry){
        if (!is_leaf()){
            subset[child_count - 1]->remove_biggest(entry);
            if (subset[child_count - 1]->data_count < MINIMUM){
                fix_shortage(child_count - 1);
            } 
        }
        else{
            delete_item(data, data_count - 1, data_count, entry);
        }
    }

    //transfer one element LEFT from child i
    void rotate_left(int i){
        //move the parent to the back of child i
        attach_item(subset[i]->data, subset[i]->data_count, data[i]);
        //move left child of right sibling
        if (!subset[i]->is_leaf()){
            BPlusTree* temp;
            delete_item(subset[i + 1]->subset, 0, subset[i + 1]->child_count, temp);
            attach_item(subset[i]->subset, subset[i]->child_count, temp);
        }
        //move leftmost right sibling to the root
        T root;
        delete_item(subset[i + 1]->data, 0, subset[i + 1]->data_count, root);
        data[i] = root;
    }

    //transfer one element RIGHT from child i
    void rotate_right(int i){
        //move the parent to the front of child i
        insert_item(subset[i]->data, 0, subset[i]->data_count, data[i - 1]);
        //move right child of left sibling
        if (!subset[i]->is_leaf()){
            BPlusTree* temp;
            detach_item(subset[i - 1]->subset, subset[i - 1]->child_count, temp);
            insert_item(subset[i]->subset, 0, subset[i]->child_count, temp);
        }
        //move rightmost left sibling to the root
        T root;
        detach_item(subset[i - 1]->data, subset[i - 1]->data_count, root);
        data[i - 1] = root;
    }

    //merge subset i with  i+1
    BPlusTree<T> *merge_with_next_subset(int i){
        //merge the grandchildren     
        merge(subset[i]->subset, subset[i]->child_count, subset[i + 1]->subset, subset[i + 1]->child_count);
        //merge the parent
        T temp;
        delete_item(data, i, data_count, temp);
        attach_item(subset[i]->data, subset[i]->data_count, temp);
        merge(subset[i]->data, subset[i]->data_count, subset[i + 1]->data, subset[i + 1]->data_count);
        //
        BPlusTree* temp1;
        delete_item(subset, i + 1, child_count, temp1);
    }

};

#endif

