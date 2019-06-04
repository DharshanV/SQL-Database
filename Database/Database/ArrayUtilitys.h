#ifndef ARRAY_UTILITYS_H
#define ARRAY_UTILITYS_H
#include <iostream>
#include <vector>
#include <assert.h>
using namespace std;
template <class T>
T maximal(const T& a, const T& b);                      //return the larger of the two items

template <class T>
void swap_(T& a, T& b);  //swap the two items

template <class T>
int index_of_maximal(T data[], int n);                 //return index of the largest item in data

template <class T>
void ordered_insert(T data[], int& n,const T& entry);        //insert entry into the sorted array
														//data with length n
template <class T>
int first_ge(const T data[], int n, const T& entry);   //return the first element in data that is
														//not less than entry
template <class T>
void attach_item(T data[], int& n, const T& entry);    //append entry to the right of data

template <class T>
void insert_item(T data[], int i, int& n, T entry);    //insert entry at index i in data

template <class T>
void detach_item(T data[], int& n, T& entry);          //remove the last element in data and place
														//it in entry
template <class T>
void delete_item(T data[], int i, int& n, T& entry);   //delete item at index i and place it in entry

template <class T>
void merge(T data1[], int& n1, T data2[], int& n2);   //append data2 to the right of data1

template <class T>
void split(T data1[], int& n1, T data2[], int& n2);   //move n/2 elements from the right of data1
														//and move to data2
template <class T>
void copy_array(T dest[], const T src[],
	int& dest_size, int src_size);              //copy src[] into dest[]

template <class T>
void print_array(const T data[], int n, int pos = 0);  //print array data

template <class T>
bool is_gt(const T data[], int n, const T& item);       //item > all data[i]

template <class T>
bool is_le(const T data[], int n, const T& item);       //item <= all data[i]

//-------------- Vector Extra operators: ---------------------

template <typename T>
ostream& operator <<(ostream& outs, const vector<T>& list); //print vector list

template <typename T>
vector<T>& operator +=(vector<T>& list, const T& addme); //list.push_back addme

template<class T>
T maximal(const T& a, const T& b)
{
	return (a > b) ? a : b;
}

template<class T>
void swap_(T& a, T& b)
{
	T temp = a;
	a = b;
	b = temp;
}

template<class T>
int index_of_maximal(T data[], int n)
{
	if (n <= 0) return -1;
	int index = 0;
	for (int i = index + 1; i < n; i++) {
		if (data[i] > data[index]) {
			index = i;
		}
	}
	return index;
}

template<class T>
void ordered_insert(T data[], int& n,const T& entry)
{
    int index = n-1;
	if (index < 0) {
		data[0] = entry;
	}
	else {
		while (index >= 0 && data[index] > entry) {
			T temp;
			temp = data[index];
			data[index + 1] = temp;
			index--;
		}
		data[index + 1] = entry;
	}
	n++;
}

template<class T>
int first_ge(const T data[], int n, const T& entry)
{
	for (int i = 0; i < n; i++) {
        if (!(data[i] < entry)) {
            return i;
		}
	}
    return n;
}

template<class T>
void attach_item(T data[], int& n, const T& entry)
{
	data[n] = entry;
	n++;
}

template<class T>
void insert_item(T data[], int i, int& n, T entry)
{
    assert(!(n <= 0 || i < 0 || i > n));
	for (int j = n-1; j>=i; j--) {
        data[j+1] = data[j];
	}
    data[i] = entry;
    n++;
}

template<class T>
void detach_item(T data[], int& n, T& entry)
{
	assert(!(n <= 0));
	entry = data[n - 1];
	n--;
}

template<class T>
void delete_item(T data[], int i, int& n, T& entry)
{
	//assert(!(n <= 0 || i < 0 || i >= n));
	entry = data[i];
	for (int j = i+1; j < n; j++) {
		data[j - 1] = data[j];
	}
	n--;
}

template<class T>
void merge(T data1[], int& n1, T data2[], int& n2)
{
	for (int i = 0; i < n2; i++) {
		data1[n1 + i] = data2[i];
	}
	n1 += n2;
	n2 = 0;
}

template<class T>
void split(T data1[], int& n1, T data2[], int& n2)
{
	int mid = n1 / 2;
	n2 = mid;
	if (n1 % 2 == 0) {
		for (int i = 0; i < mid; i++) {
			data2[i] = data1[mid + i];
			n1--;
		}
	}
	else {
		for (int i = 0; i < mid; i++) {
			data2[i] = data1[mid + 1 + i];
			n1--;
		}
	}
}

template<class T>
void copy_array(T dest[], const T src[], int& dest_size, int src_size)
{
	for (int i = 0; i < src_size; i++) {
		dest[i] = src[i];
	}
	dest_size = src_size;
}

template<class T>
void print_array(const T data[], int n, int pos)
{
	if (n <= 0 || pos >= n) return; 
	cout << "[";
	for (int i = pos; i < n; i++) {
		cout << data[i];
		if (i + 1 < n) {
			cout << ",";
		}
	}
	cout << "]" << endl;
}

template<class T>
bool is_gt(const T data[], int n, const T& item)
{
	assert(!(n <= 0));
	for (int i = 0; i < n; i++) {
		if (!(item > data[i])) {
			return false;
		}
	}
	return true;
}

template<class T>
bool is_le(const T data[], int n, const T& item)
{
	assert(!(n <= 0));
	bool valid = false;
	for (int i = 0; i < n; i++) {
		if (item <= data[i]) {
			valid = true;
		}
		else {
			valid = false;
		}
	}
	return valid;
}

template<typename T>
ostream& operator<<(ostream & outs, const vector<T>& list)
{
	outs << "[";
	for (int i = 0; i < list.size();i++) {
		outs << list.at(i);
		if (i + 1 != list.size()) {
			outs << ",";
		}
	}
	outs << "]";
	return outs;
}

template<typename T>
vector<T>& operator+=(vector<T>& list, const T& addme)
{
	list.push_back(addme);
	return list;
}

template<typename T>
vector<T>& operator+=(vector<T>& list, const vector<T>& addme)
{
	for (int i = 0; i < addme.size(); i++) {
		list.push_back(addme[i]);
	}
	return list;
}

#endif // !ARRAY_UTILITYS_H
