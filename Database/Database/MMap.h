#ifndef MMAP_H
#define MMAP_H
#include "BPlusTree.h"
#include "mmpair.h"

template <class K, typename V>
class MMap
{
public:
	typedef BPlusTree<MMPair<K, V> > map_base;
	class Iterator {
	public:
		friend class MMap;
		Iterator(typename map_base::Iterator it = NULL) {
			_it = it;
		}
		bool isNull() {
			return _it.is_null();
		}
		Iterator operator ++(int unused) {
			return _it++;
		}
		Iterator operator ++() {
			return ++_it;
		}

		MMPair<K, V> operator *() {
			return *_it;
		}
		friend bool operator ==(const Iterator& lhs, const Iterator& rhs) {
			return lhs._it == rhs._it;
		}
		friend bool operator !=(const Iterator& lhs, const Iterator& rhs) {
			return lhs._it != rhs._it;
		}
	private:
		class map_base::Iterator _it;
	};
	MMap();

	//  Capacity
	int size() const;
	bool empty() const;
	int count(const K& key);

	//  Element Access
	const vector<V>& operator[](const K& key) const;
	vector<V>& operator[](const K& key);

	//  Modifiers
	void insert(const K& k, const V& v);
	void erase(const K& key);
	void clear();

	//  Operations:
	bool contains(const K& key) const;
	vector<V>& get(const K& key);

	Iterator begin() {
		return mmap.begin();
	}
	Iterator end() {
		return mmap.end();
	}

	Iterator find(const K& key);

	// I have not writtent hese yet, but we will need them:
	//    int count(const K& key);
	//    lower_bound
	//    upper_bound
	//    equal_range:

	bool is_valid();

	friend ostream& operator<<(ostream& outs, const MMap<K, V>& print_me) {
		outs << print_me.mmap;
		return outs;
	}

private:
	BPlusTree<MMPair<K, V> > mmap;
};

template<typename K, typename V>
MMap<K, V>::MMap()
{
}

template<typename K, typename V>
int MMap<K, V>::size() const
{
	return mmap.size();
}

template<typename K, typename V>
bool MMap<K, V>::empty() const
{
	return mmap.empty();
}

template<typename K, typename V>
const vector<V>& MMap<K, V>::operator[](const K& key) const
{
	return get(key);
}

template<typename K, typename V>
vector<V>& MMap<K, V>::operator[](const K& key)
{
	return get(key);
}

template<typename K, typename V>
void MMap<K, V>::insert(const K& k, const V& v)
{
	mmap.insert(MMPair<K, V>(k, v));
}

template<typename K, typename V>
void MMap<K, V>::erase(const K & key)
{
	mmap.remove(MMPair<K, V>(key, V()));
}

template<typename K, typename V>
void MMap<K, V>::clear()
{
	mmap.clear_tree();
}

template<typename K, typename V>
bool MMap<K, V>::contains(const K& key) const
{
	return mmap.contains(MMPair<K, V>(key, V()));
}

template<typename K, typename V>
vector<V>& MMap<K, V>::get(const K& key)
{
	return mmap.get(MMPair<K, V>(key, V())).value;
}

template<typename K, typename V>
int MMap<K, V>::count(const K& key)
{
	return get(key).size();
}

template<class K, typename V>
typename MMap<K,V>::Iterator MMap<K, V>::find(const K& key)
{
	typename map_base::Iterator it;
	it = mmap.find(key);
	return Iterator(it);
}

template<typename K, typename V>
bool MMap<K, V>::is_valid()
{
	return mmap.is_valid();
}
#endif // !MMAP_H
