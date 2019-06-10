#ifndef BTREE_H
#define BTREE_H
#include <iostream>
#include "ArrayUtilitys.h"
#include <iomanip>
using namespace std;

template <class T>
class BPlusTree
{
public:
    class Iterator{
    public:
        friend class BPlusTree;
        Iterator(BPlusTree<T>* _it=NULL, int _key_ptr = 0)
			:it(_it), key_ptr(_key_ptr) {
		}

        T operator *(){
            assert(key_ptr<it->data_count);
            return it->data[key_ptr];
        }
        //arrow op returns the address of the key

        Iterator operator++(int un_used){
            Iterator hold(it,key_ptr);
            if(key_ptr + 1 == it->data_count){
                it = it->next;
                key_ptr = 0;
            }
            else{
                key_ptr++;
            }
            return hold;
        }

        Iterator& operator++(){
            if(key_ptr + 1 == it->data_count){
                it = it->next;
                key_ptr = 0;
            }
            else{
                key_ptr++;
            }
			return *this;
        }

        friend bool operator ==(const Iterator& lhs, const Iterator& rhs){
            return (lhs.it == rhs.it && lhs.key_ptr == rhs.key_ptr);
        }

        friend bool operator !=(const Iterator& lhs, const Iterator& rhs){
			if (!(lhs.it != rhs.it)) {
				if (!(lhs.key_ptr < rhs.key_ptr)) return false;
				else return true;
			}
			else return true;
        }

        void print_Iterator(){

        }
        bool is_null(){return !it;}
    private:
        BPlusTree<T>* it;
        int key_ptr;
    };

    BPlusTree(bool dups = false);
    //big three:
    BPlusTree(const BPlusTree<T>& other);
    ~BPlusTree();
    BPlusTree<T>& operator =(const BPlusTree<T>& RHS);

    void insert(const T& entry);                //insert entry into the tree
    void remove(const T& entry);                //remove entry from the tree

    void clear_tree();                          //clear this object (delete all nodes etc.)
    void copy_tree(const BPlusTree<T>& other);  //copy other into this object
//    void copy_tree(const BPlusTree<T>& other, stack<BPlusTree<T>*>& s);
    bool contains(const T& entry) const;        //true if entry can be found in the array

    T& get(const T& entry);                     //return a reference to entry in the tree
    const T& get(const T& entry)const;          //return a reference to entry in the tree
    T& get_existing(const T& entry);            //return a reference to entry in the tree

    Iterator find(const T& entry);              //return an iterator to this key. NULL if not there.

    int size() const;                           //count the number of elements in the tree
    bool empty() const;                         //true if the tree is empty

    void print_tree(int level = 0,
                    ostream &outs=cout) const; //print a readable version of the tree
    friend ostream& operator<<(ostream& outs,
                               const BPlusTree<T>& print_me){
        print_me.print_tree(0, outs);
        return outs;
    }
    bool is_valid();

    Iterator begin(){
		BPlusTree<T>* ptr;
		if (data_count == 0) ptr = NULL;
		else ptr = get_smallest_node();
        return Iterator(ptr);
    }
    Iterator end(){
        return Iterator(NULL);
    }
private:
    static const int MINIMUM = 1;
    static const int MAXIMUM = 2 * MINIMUM;

    bool dups_ok;                       //true if duplicate keys may be inserted
    int data_count;                     //number of data elements
    T data[MAXIMUM + 1];                //holds the keys
    int child_count;                    //number of children
    BPlusTree* subset[MAXIMUM + 2];     //subtrees
    BPlusTree* next;
    bool is_leaf() const
        {return child_count==0;}        //true if this is a leaf node

    T* find_ptr(const T& entry);        //return a pointer to this key. NULL if not there.

    //insert element functions
    void loose_insert(const T& entry);  //allows MAXIMUM+1 data elements in the root
    void fix_excess(int i);             //fix excess of data elements in child i

    //remove element functions:
    void loose_remove(const T& entry);  //allows MINIMUM-1 data elements in the root

    BPlusTree<T>* fix_shortage(int i);  //fix shortage of data elements in child i
                                        // and return the smallest key in this subtree
    BPlusTree<T>* get_smallest_node();
    BPlusTree<T>* get_biggest_node();
    void get_smallest(T& entry);        //entry := leftmost leaf
    void get_biggest(T& entry);         //entry := rightmost leaf
    void remove_biggest(T& entry);      //remove the biggest child of this tree->entry
    void transfer_left(int i);          //transfer one element LEFT from child i
    void transfer_right(int i);         //transfer one element RIGHT from child i
	void rotate_left(int i);
	void rotate_right(int i);
    BPlusTree<T>* merge_with_next_subset(int i);    //merge subset i with subset i+1
private:
    bool isFound(int index, const T &entry) const;
    bool isInBetween(const T data[],int n,const T& min,const T& max);
};

template<typename T>
BPlusTree<T>::BPlusTree(bool dups)
{
	this->dups_ok = dups;
	data_count = 0;
	child_count = 0;
    next = NULL;
    for(int i=0;i<MAXIMUM+2;i++){
        subset[i] = NULL;
    }
}

template<typename T>
BPlusTree<T>::BPlusTree(const BPlusTree<T>& other)
{
	copy_tree(other);
}

template<typename T>
BPlusTree<T>::~BPlusTree()
{
	clear_tree();
}

template<typename T>
BPlusTree<T>& BPlusTree<T>::operator=(const BPlusTree<T>& RHS)
{
	if (&RHS == this)return *this;
	clear_tree();
	copy_tree(RHS);
	return *this;
}

/*
 *  call loose_insert
 *  grow the tree if you need to.
 *
 * loose_insert(entry):
 *  look for entry in data [ ]
 *  if found and no dupes, exit.
 *  four cases:
 *      1. found / leaf: call overloaded +
 *      2. found / !leaf call subset[i+1]->loose_insert and fix_excess(i+1)
 *      3. !found / leaf insert entry at position data[i]
 *      4. !found / !leaf call subset[i]->loose_insert and fix_excess(i)
*/
template<typename T>
void BPlusTree<T>::insert(const T& entry)
{
	loose_insert(entry);
	if (data_count == MAXIMUM + 1) {
		BPlusTree<T>* temp = new BPlusTree<T>(dups_ok);
		copy_array(temp->data, data, temp->data_count, data_count);
		copy_array(temp->subset, subset, temp->child_count, child_count);

		data_count = 0;
		child_count = 1;
		subset[0] = temp;
		fix_excess(0);
    }
}

template<typename T>
void BPlusTree<T>::remove(const T &entry)
{
/*
 * ---------------------------------------------------------------------------------
 * Same as BTree:
 * Loose_remove the entry from this tree.
 * Shrink if you have to
 * ---------------------------------------------------------------------------------
 * once you return from loose_remove, the root (this object) may have no data and
 * only a single subset:
 * now, the tree must shrink:
 *
 * point a temporary pointer (shrink_ptr) and point it to this root's only subset
 * copy all the data and subsets of this subset into the root (through shrink_ptr)
 * now, the root contains all the data and poiners of it's old child.
 * now, simply delete shrink_ptr, and the tree has shrunk by one level.
 * Note, the root node of the tree will always be the same, it's the
 * child node we delete
 */
    loose_remove(entry);
	if (data_count == 0 && child_count == 1) {
		BPlusTree<T>* shrink_ptr = subset[0];
		copy_array(data, shrink_ptr->data,
			data_count, shrink_ptr->data_count);
		copy_array(subset, shrink_ptr->subset, 
			child_count, shrink_ptr->child_count);
		shrink_ptr->child_count = 0;
		delete shrink_ptr;
	}
}

template<typename T>
void BPlusTree<T>::clear_tree()
{
	if (this->is_leaf()) {
		child_count = 0;
		return;
	}
	else {
		for (int i = child_count - 1; i >= 0; i--) {
			subset[i]->clear_tree();
			delete subset[i];
		}
		child_count = 0;
	}
	data_count = 0;
	for (int i = 0; i < child_count; i++) {
		subset[i] = NULL;
	}
	child_count = 0;
}

template<typename T>
void BPlusTree<T>::copy_tree(const BPlusTree<T>& other)
{
	copy_array(data, other.data, data_count, other.data_count);
	if (other.is_leaf()) {
		return;
	}
	else {
		child_count = other.child_count;
		for (int i = other.child_count - 1; i >= 0; i--) {
			subset[i] = new BPlusTree<T>(dups_ok);
            subset[i]->copy_tree(*other.subset[i]);
		}
		if (child_count < 2)return;
		for (int i = other.child_count - 1; i > 0; i--) {
			subset[i - 1]->get_biggest_node()->next = subset[i]->get_smallest_node();
		}
	}
}

template<typename T>
bool BPlusTree<T>::contains(const T& entry) const
{
    int index = first_ge(data,data_count,entry);
    bool found = isFound(index,entry);
    if(found){
        return true;
    }
    else{
        if(this->is_leaf()){
            return false;
        }
        else{
            return subset[index]->contains(entry);
        }
    }
}

template<typename T>
T& BPlusTree<T>::get(const T& entry)
{
    if(!contains(entry)){
        insert(entry);
    }
    return get_existing(entry);
}

template<typename T>
const T& BPlusTree<T>::get(const T& entry) const
{
    int index = first_ge(data,data_count,entry);
    bool found = isFound(index,entry);
    T temp = entry;
    if(is_leaf()){
        if(found){
            return data[index];
        }
        else {
            assert(found);
        }
    }
    if(found){
        return subset[index+1]->get(entry);
    }
    else{
        return subset[index]->get(entry);
    }
}

template<typename T>
T& BPlusTree<T>::get_existing(const T& entry)
{
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
    int index = first_ge(data,data_count,entry);
    bool found = isFound(index,entry);
    T temp = entry;
    if(is_leaf()){
        if(found){
            return data[index];
        }
        else {
            assert(found);
        }
    }
    if(found){
        return subset[index+1]->get_existing(entry);
    }
    else{
        return subset[index]->get_existing(entry);
    }
}

template<class T>
typename BPlusTree<T>::Iterator BPlusTree<T>::find(const T& entry)
{
    int index = first_ge(data,data_count,entry);
    bool found = isFound(index,entry);
    if(is_leaf()){
        if(found){
            return Iterator(this,index);
        }
        else {
			if (index == data_count) {
				return ++Iterator(this, index-1);
			}
			return Iterator(this, index);
        }
    }
    if(found){
        return subset[index+1]->find(entry);
    }
    else{
        return subset[index]->find(entry);
    }
}

template<typename T>
int BPlusTree<T>::size() const
{
	int total = 0;
	if (is_leaf()) {
		return data_count;
	}
	for (int i = child_count - 1; i >= 0; i--) {
		if (i != data_count) {
			total++;
		}
		total += subset[i]->size();
	}
	return total;
}

template<typename T>
bool BPlusTree<T>::empty() const
{
	return data_count == 0;
}

template<typename T>
bool BPlusTree<T>::is_valid()
{
	bool valid = false;
	if (this->is_leaf()) {
		return true;
	}
	for (int i = child_count - 1; i >= 0; i--) {
		valid = subset[i]->is_valid();
		if (valid == false)return false;
		T min;
		T max;

		if (i - 1 < 0) {
			min = INT_MIN;
			max = data[i];
		}
		else if (i >= data_count) {
			max = INT_MAX;
			min = data[i - 1];
		}
		else {
			min = data[i - 1];
			max = data[i];
		}
		valid = isInBetween(subset[i]->data,
			subset[i]->data_count, min, max);
		if (valid == false)return false;
	}
    return valid;
}

template<typename T>
void BPlusTree<T>::print_tree(int level, ostream& outs) const
{
//data[data_count-1] must be less than equal to 
//  every subset[child_count-1]->data[ ]

//every data[i] is greater than every subset[i]->data[ ]

//B+Tree: Every data[i] is equal to subset[i+1]->smallest

//Recursively validate every subset[i]
    if(this->is_leaf()){
        for(int i= this->data_count-1;i>=0;i--){
            cout<<setw(level*5);
            cout<<this->data[i]<<endl;
        }
    }
    else{
        for(int i=child_count-1;i>=0;i--){
            if(i != data_count){
                cout<<setw(level*5);
                cout<<data[i]<<endl;
            }
            subset[i]->print_tree(level+1,outs);
        }
    }
}

template<typename T>
bool BPlusTree<T>::isFound(int index, const T &entry) const
{
    if(index >= data_count || index<0) return false;
    if(data[index] == entry) return true;
    return false;
}

template<typename T>
bool BPlusTree<T>::isInBetween(const T data[],int n,const T& min,const T& max)
{
	for (int i = 0; i < n; i++) {
		if (!(min <= data[i] && !(max <= data[i]))) {
			return false;
		}
	}
    return true;
}

template<class T>
T* BPlusTree<T>::find_ptr(const T& entry)
{
    int index = first_ge(data,data_count,entry);
    bool found = isFound(index,entry);
    T temp = entry;
    if(is_leaf()){
        if(found){
            return &data[index];
        }
        else {
            return NULL;
        }
    }
    if(found){
        return subset[index+1]->find_ptr(entry);
    }
    else{
        return subset[index]->find_ptr(entry);
    }
}

template<typename T>
void BPlusTree<T>::loose_insert(const T& entry)
{
    int index = first_ge(data,data_count,entry);
    bool found = isFound(index,entry);
    if(found && dups_ok == false) return;

    if(found){
        if(is_leaf()){
            data[index] += entry;
        }
        else{
            subset[index+1]->loose_insert(entry);
            fix_excess(index+1);
        }
    }
    else{
        if(is_leaf()){
            ordered_insert(data,data_count,entry);
        }
        else{
            subset[index]->loose_insert(entry);
			fix_excess(index);
        }
    }
}

template<typename T>
void BPlusTree<T>::fix_excess(int i)
{
	if (subset[i]->data_count == MAXIMUM + 1) {
		BPlusTree<T>* maxTree = subset[i];
		//move the data across
		BPlusTree<T>* newTree = new BPlusTree<T>(dups_ok);
		insert_item(subset, i + 1, child_count, newTree);

		//Split data
		split(maxTree->data, maxTree->data_count,
			newTree->data, newTree->data_count);
		//Split pointers
		split(maxTree->subset, maxTree->child_count,
			newTree->subset, newTree->child_count);

		//Detach
		T temp;
		detach_item(maxTree->data,
			maxTree->data_count, temp);
		ordered_insert(data, data_count, temp);
        newTree->insert(temp);


        if(subset[i]->is_leaf()){
            newTree->next = subset[i]->next;
            subset[i]->next = newTree;
        }
    }
}

/*
 *  1. if this node is_leaf: just delete and return. if not found, then not on tree
 *  2. if found (and this node not is_leaf):
 *      if shortage, fix shortage
 *                   look for entry in this subset and replace it (data[i]) with smallest of subset[i+1]
 *                   else look for entry in the subset resulting from fix_shortage
 *                                     and replace it with that subset's smallest
 * 3. if not found (yet) call loose_remove of the subset[i] and fix shortage if needed.
*/
template<typename T>
void BPlusTree<T>::loose_remove(const T& entry)
{
    int index = first_ge(data,data_count,entry);
    bool found = isFound(index,entry);
    T temp = entry;
    if(is_leaf()){
        if(found){
            delete_item(data,index,data_count,temp);
        }
    }
    else if(!is_leaf()){
        if(found){
            subset[index+1]->loose_remove(entry);
			data[index] = subset[index + 1]->get_smallest_node()->data[0];
            fix_shortage(index+1);
			bool found = false;
			for (int i = 0; i < data_count; i++) {
				if (data[i] == entry) {
					found = true;
					break;
				}
			}
			if(found){
				data[index] = subset[index + 1]->get_smallest_node()->data[0];
			}
        }
        else{
            subset[index]->loose_remove(entry);
            fix_shortage(index);
        }
    }
}

/*
 *  1. transfer_right(i-1)
 *  2. transfer_left(i+1)
 *  3. merge_with_next_subset(i-1)
 *  4. merge_with_next_subset(i)
*/
template<class T>
BPlusTree<T>* BPlusTree<T>::fix_shortage(int i)
{
    if(subset[i]->data_count>=MINIMUM)return NULL;

    if(i + 1 < child_count && subset[i+1]->data_count > MINIMUM){
		transfer_left(i);
		return subset[i];
    }
    else if(i > 0 && subset[i-1]->data_count > MINIMUM){
		transfer_right(i);
		return subset[i];
    }
    else if(i+1<child_count && subset[i+1]->data_count>0){
        merge_with_next_subset(i);
    }
    else{
		merge_with_next_subset(i-1);
    }
	return NULL;
}

template<class T>
BPlusTree<T>* BPlusTree<T>::get_smallest_node()
{
    if(is_leaf()){
        return this;
    }
    return subset[0]->get_smallest_node();
}

template<class T>
BPlusTree<T>* BPlusTree<T>::get_biggest_node()
{
    if(is_leaf()){
        return this;
    }
    return subset[child_count-1]->get_biggest_node();
}

template<class T>
void BPlusTree<T>::get_smallest(T& entry)
{
	//replace entry with the left_most leaf of this tree
// Keep looking in the first subtree (recursive)
//  until you get to a leaf.
// Then, replace entry with data[0] of the leaf node
	entry = get_smallest_node()->data[0];
}

template<class T>
void BPlusTree<T>::get_biggest(T& entry)
{
	entry = get_smallest_node()->data[data_count - 1];
}

template<typename T>
void BPlusTree<T>::remove_biggest(T& entry)
{
    if(is_leaf()){
        detach_item(data,data_count,entry);
        return;
    }
    subset[child_count - 1]->remove_biggest(entry);
	if (subset[child_count - 1]->data_count < MINIMUM) {
		fix_shortage(child_count-1);
	}
}

template<class T>
void BPlusTree<T>::transfer_left(int i)
{
	/*
	    * --------------------------------------------------------------
     * transfer_left: from subset[i] to subset[i-1]
     *      non-leaf: same as BTree
     *      leaves  : rotate and leave and update data[i-1] with subset[i]->data[0]
     *                  leftmost key after you borrowed goes to replace data[i-1]
     * --------------------------------------------------------------
	*/
	if (subset[i]->is_leaf()) {
		rotate_left(i);
		data[i] = subset[i+1]->data[i];
	}
	else {
		rotate_left(i);
	}
}

template<class T>
void BPlusTree<T>::transfer_right(int i)
{
	/*
	 * --------------------------------------------------------------
     * transfer right from subset[i] to subset[i+1]
     *      non-leaf: same as BTree
     *      leaves  : rotate and leave a 'trace':
     *              data[i] is replaced with subset[i+1]->data[0]
     *              after you borrowed the key,
     * --------------------------------------------------------------
	*/
	if (subset[i]->is_leaf()) {
		rotate_right(i);
		subset[i]->data[0] = data[i-1];
	}
	else {
		rotate_right(i);
	}
}

template<typename T>
void BPlusTree<T>::rotate_left(int i)
{
    attach_item(subset[i]->data,subset[i]->data_count,data[i]);
    delete_item(subset[i+1]->data,
		MINIMUM-1,subset[i+1]->data_count,data[i]);
	if (!subset[i + 1]->is_leaf()) {
		BPlusTree<T>* temp;
		delete_item(subset[i + 1]->subset, 0,
			subset[i + 1]->child_count, temp);
		attach_item(subset[i]->subset, subset[i]->child_count, temp);
	}
}

template<typename T>
void BPlusTree<T>::rotate_right(int i)
{
    attach_item(subset[i]->data,subset[i]->data_count,data[i-1]);
    detach_item(subset[i-1]->data,subset[i-1]->data_count,data[i-1]);
	if (!subset[i - 1]->is_leaf()) {
		BPlusTree<T>* temp;
		detach_item(subset[i - 1]->subset,
			subset[i - 1]->child_count, temp);
		insert_item(subset[i]->subset,0, subset[i]->child_count, temp);
	}
}

template<class T>
BPlusTree<T>* BPlusTree<T>::merge_with_next_subset(int i)
{
	/*
		 *   1. remove data[i] from this object
	 *   2. if not a leaf, append it to child[i]->data:
	 *   3. Move all data items from subset[i+1]->data to right of subset[i]->data
	 *   4. Move all subset pointers from subset[i+1]->subset to
	 *          right of subset[i]->subset
	 *   5. delete subset[i+1] (store in a temp ptr)
	 *   6. if a leaf, point subset[i]->next to temp_ptr->next
	 *   6. delete temp ptr
	 *
	*/
	BPlusTree<T>* nextPtr = NULL;
	if (!subset[i]->is_leaf()) {
		BPlusTree<T>* temp;
		T item;
		delete_item(data, i, data_count, item);
		attach_item(subset[i]->data, subset[i]->data_count, item);
		merge(subset[i]->data, subset[i]->data_count,
			subset[i + 1]->data, subset[i + 1]->data_count);
		merge(subset[i]->subset, subset[i]->child_count,
			subset[i + 1]->subset, subset[i + 1]->child_count);
		delete_item(subset, i + 1, child_count, temp);
		delete temp;
	}
	else {
		BPlusTree<T>* temp;
		T item;
		delete_item(data, i, data_count, item);
		merge(subset[i]->data, subset[i]->data_count,
			subset[i + 1]->data, subset[i + 1]->data_count);
		merge(subset[i]->subset, subset[i]->child_count,
			subset[i + 1]->subset, subset[i + 1]->child_count);
		if (i + 1 > child_count)nextPtr = NULL;
		else nextPtr = subset[i + 1]->next;
		delete_item(subset, i + 1, child_count, temp);
		delete temp;
		subset[i]->next = nextPtr;
	}
	return NULL;
}

#endif // !BTREE_H
