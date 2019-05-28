#ifndef RANDOM_H
#define RANDOM_H
#include <iostream>
#include <ctime>
using namespace std;

template<typename T>
class Random{
public:
    Random();
    int next(int low,int high);
    void fill(T a[],unsigned int size,int low,int high);
private:
    static bool firstRun;
};


template<typename T>
bool Random<T>::firstRun = true;

template<typename T>
Random<T>::Random()
{
    if(firstRun) srand(time(NULL));
    firstRun = false;

}

template<typename T>
int Random<T>::next(int low, int high)
{
    return (rand()%(high-low + 1)) + low;
}

template<typename T>
void Random<T>::fill(T a[], unsigned int size,int low,int high)
{
    for(int i=0;i<size;i++){
        a[i] = next(low,high);
    }
}

#endif // RANDOM_H

//rand()%(lo - hi + 1) + lo
