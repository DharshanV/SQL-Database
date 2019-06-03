#ifndef STACK_H
#define STACK_H
#include "linkedlistfuntions.h"
#include "list.h"

template <class T>
class Stack{
public:
    Stack(){}
    ~Stack(){}
    Stack(const Stack<T>& other);
    Stack<T>& operator =(const Stack<T>& rhs);

    void push(T item);
    T pop();
    T top();
    bool empty();
	Stack& operator += (const T& value) {
		push(value);
		return *this;
	}
    friend ostream& operator << (ostream& outs, const Stack& s){
        outs<<s.list;
        return outs;
    }
private:
    List<T> list;
};

template<class T>
Stack<T>::Stack(const Stack<T>& other)
{
    list = other.list;
}

template<class T>
Stack<T>& Stack<T>::operator =(const Stack<T>& rhs)
{
    list = rhs.list;
    return *this;
}

template<class T>
void Stack<T>::push(T item)
{
    list.insert(item);
}

template<class T>
T Stack<T>::pop()
{
    return list.deleteHead();
}

template<class T>
T Stack<T>::top()
{
    return list.headValue();
}

template<class T>
bool Stack<T>::empty()
{
    return list.empty();
}


#endif // STACK_H
