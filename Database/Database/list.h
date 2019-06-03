#ifndef LIST_H
#define LIST_H
#include "linkedlistfuntions.h"

template<class T>
class List{
public:
    List();
    List(const List<T>& other);
    List<T>& operator = (const List<T>& other);
    ~List();

    friend ostream& operator<<(ostream& out,const List<T>& other){
        print_list(other._head,out);
        return out;
    }

    //deletes all the nodes in the list
    void deleteAll();

    //retruns true if list is empty
    bool empty();

    //inserts a node at the head of the list
    void insert(T item);

    //deletes the node at the head,
    //and returns the value.
    T deleteHead();

    //returns the value at head.
    T headValue();
private:
    node<T>* _head;
};

template<class T>
List<T>::List()
{
    init_head(_head);
}

template<class T>
List<T>::List(const List<T>& other)
{
    init_head(_head);
    copy_list(other._head,_head);
}

template<class T>
List<T>& List<T>::operator =(const List<T>& other)
{
    copy_list(other._head,_head);
    return *this;
}

template<class T>
List<T>::~List()
{
    deleteAll();
}

template<class T>
void List<T>::deleteAll()
{
    delete_all(_head);
}

template<class T>
bool List<T>::empty()
{
    if (_head == NULL) {
        return true;
    }
    return false;
}

template<class T>
void List<T>::insert(T item)
{
    insert_head(_head,item);
}

template<class T>
T List<T>::deleteHead()
{
    return delete_head(_head);
}

template<class T>
T List<T>::headValue()
{
    assert(_head != NULL);
    return _head->_item;
}

#endif // LIST_H
