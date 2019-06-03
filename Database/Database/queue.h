#ifndef QUEUE_H
#define QUEUE_H
#include "linkedlistfuntions.h"

template <class T>
class Queue{
public:
    Queue():_head(NULL), _tail(NULL){}
    ~Queue();
    Queue(const Queue<T>& other);
    Queue<T> &operator =(const Queue& rhs);

    //Pushes an item at the tail of the linked list.
    void push(T item);

    //Pops a value at the head of the list, and
    //returns that poped value.
    T pop();

    //returns true if list is empty.
    bool empty();

    //returns the value at the head of the list
    T front();

    //overrides the << operator for Queue object
    friend ostream& operator <<(ostream& outs, const Queue<T>& q){
        return print_list(q._head, outs);
    }

	Queue& operator += (const T& value) {
		push(value);
		return *this;
	}

private:
    node<T>* _head;
    node<T>* _tail;
};

template<class T>
Queue<T>::~Queue()
{
    delete_all(_head);
}

template<class T>
Queue<T>::Queue(const Queue<T>& other)
{
    this->_head = NULL;
    this->_tail = NULL;
    _tail = copy_list(other._head,_head);
}

template<class T>
Queue<T>& Queue<T>::operator =(const Queue& rhs)
{
    _tail = copy_list(rhs._head,_head);
    return *this;
}

template<class T>
void Queue<T>::push(T item)
{
    _tail = insert_after(_head,_tail,item);
}

template<class T>
T Queue<T>::pop()
{
    return delete_head(_head);
}

template<class T>
bool Queue<T>::empty()
{
    if (_head == NULL) {
        return true;
    }
    return false;
}

template<class T>
T Queue<T>::front()
{
    if(_head != NULL){
        return  _head->_item;
    }
}

#endif // QUEUE_H
