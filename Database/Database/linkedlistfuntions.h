#ifndef LINKEDLISTFUNTIONS_H
#define LINKEDLISTFUNTIONS_H
#include <iostream>
#include <assert.h>

using namespace std;

//Node data type to hold the item data, and pointer to
//the next node
template <class T>
struct node {
    node(const T& item = T(), node<T>* next = NULL)
        :_item(item), _next(next) {}
    T _item;
    node<T>* _next;
    friend ostream& operator << (ostream& outs, const node<T>& print_me) {
        outs << "[" << print_me._item << "]->";
        return outs;
    }
};

//initializes head to NULL
template <class T>
node<T>* init_head(node<T>* &head);

//deletes all the nodes in the list
template<class T>
void delete_all(node<T>*&head);

//true if head is NULL, false otherwise.
template <class T>
bool empty(const node<T>* head);

//makes a copy of the list, returns a pointer to the last node:
template <class T>
node<T>* copy_list(const node<T>* head, node<T>* & cpy);

//insert at the beginning of the list:
template <class T>
node<T>* insert_head(node<T>* &head, T item);

//insert_after: if after is NULL, inserts at head
template <class T>
node<T>* insert_after(node<T>* &head, node<T>* after, const T& item);

//delete the node at the head of the list, reuturn the item:
template <class T>
T delete_head(node<T>* &head);

//print the list and return outs
template<class T>
ostream& print_list(const node<T>* head, ostream& outs = cout);

template <class T>
node<T>* init_head(node<T>* &head) {
    head = NULL;
    return head;
}

template<class T>
void delete_all(node<T>*& head) {
    node<T>* temp = head;
    while(temp != NULL){
        head = head->_next;
        delete temp;
        temp = head;
    }
    head = NULL;
}

template <class T>
bool empty(const node<T>* head) {
    if (head == NULL) {
        return true;
    }
    return false;
}

template<class T>
node<T>* copy_list(const node<T>* head, node<T>*& cpy)
{
    if(!empty(cpy)){
        delete_all(cpy);
    }

    const node<T>* temp = head;
    node<T>* cpyTail = cpy;
    while(temp != NULL){
        if(cpy == NULL){
            cpyTail = insert_head(cpy,temp->_item);
        }
        else{
            cpyTail = insert_after(cpy,cpyTail,temp->_item);
        }
        temp = temp->_next;
    }
    return cpyTail;
}

template<class T>
node<T>* insert_head(node<T>*& head, T item)
{
    node<T>* temp = new node<T>(item);
    if (head == NULL) {
        head = temp;
        temp->_next = NULL;
    }
    else {
        temp->_next = head;
        head = temp;
    }
    return temp;
}

template<class T>
node<T>* insert_after(node<T>*& head, node<T>* after, const T& item)
{
    node<T>* temp;
    if(head == NULL){
        temp = insert_head(head,item);
    }
    else{
        temp = new node<T>(item);
        after->_next = temp;
        after = temp;
    }
    return temp;
}

template<class T>
T delete_head(node<T>*& head)
{
    assert(empty(head) != true);
    node<T>* temp = head;
    head = head->_next;
    T itemTemp = temp->_item;
    delete temp;
    return itemTemp;
}

template<class T>
ostream& print_list(const node<T>* head, ostream& outs)
{
    if(empty(head)){
        outs<<"|||"<<endl;
        return outs;
    }
    const node<T>* temp = head;
    while (temp != NULL) {
        cout<<*temp;
        temp = temp->_next;
    }
    outs<<"|||"<<endl;
    return outs;
}

#endif // !LINKEDLISTFUNTIONS_H
