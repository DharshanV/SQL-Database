#ifndef MMMMPair_H
#define MMMMPair_H
#include <iostream>
#include <vector>
using namespace std;
template <typename K, typename V>
struct MMPair {
    K key;
    vector<V> value;

	MMPair(const K& k = K(), const V& v = V()) {
        key = k;
	}
    friend ostream& operator <<(ostream& outs, const MMPair<K, V>& print_me) {
        cout << "[K:" << print_me.key <<
                ",V:" << print_me.value << "]";
        return outs;
    }
    friend bool operator ==(const MMPair<K, V>& lhs, const MMPair<K, V>& rhs) {
        return lhs.key == rhs.key;
    }
    friend bool operator < (const MMPair<K, V>& lhs, const MMPair<K, V>& rhs) {
        return lhs.key < rhs.key;
    }
    friend bool operator > (const MMPair<K, V>& lhs, const MMPair<K, V>& rhs) {
        return lhs.key > rhs.key;
    }
    friend bool operator <= (const MMPair<K, V>& lhs, const MMPair<K, V>& rhs) {
        return lhs.key <= rhs.key;
    }
	MMPair<K, V>& operator += (const MMPair<K, V>& other) {
		for (int i = 0; i < other.value.size(); i++) {
			value += other.value[i];
		}
		return *this;
	}
	MMPair<K, V>& operator = (int other) {
		key = other;
		return *this;
	}
};
#endif // MMMMPair_H
