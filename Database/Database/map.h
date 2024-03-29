#ifndef MAP_H
#define MAP_H
#include "BPlusTree.h"
#include "pair.h"
template<class K, class V>
class Map
{
public:
	typedef BPlusTree<Pair<K, V> > map_base;
	class Iterator {
	public:
		friend class Map;
		Iterator(typename map_base::Iterator it = NULL) {
			_it = it;
		}
		Iterator operator ++(int unused) {
			return _it++;
		}
		Iterator operator ++() {
			return _it++;
		}

		Pair<K, V> operator *() {
			return *_it;
		}
		friend bool operator ==(const Iterator& lhs, const Iterator& rhs) {
			return lhs._it == rhs._it;
		}
		friend bool operator !=(const Iterator& lhs, const Iterator& rhs) {
			return lhs._it != rhs._it;
		}
	private:
		typename map_base::Iterator _it;
	};

	Map();

	Iterator begin() {
		return map.begin();
	}
	Iterator end() {
		return map.end();
	}

	//  Capacity
	int size() const;
	bool empty() const;

	//  Element Access
	V& operator[](const K& key);
	const V& operator[](const K& key) const;
	V& at(const K& key);
	const V& at(const K& key) const;


	//  Modifiers
	void insert(const K& k, const V& v);
	void erase(const K& key);
	void clear();
	V get(const K& key);

	//  Operations:
	Iterator find(const K& key);

	bool contains(const Pair<K, V>& target);

	bool is_valid() { return map.is_valid(); }

	friend ostream& operator<<(ostream& outs, const Map<K, V>& print_me) {
		outs << print_me.map;
		return outs;
	}
private:
	int key_count;
	BPlusTree<Pair<K, V> > map;
};

template<class K, class V>
Map<K, V>::Map()
{
	key_count = 0;
}

template<class K, class V>
int Map<K, V>::size() const
{
	return map.size();
}

template<class K, class V>
bool Map<K, V>::empty() const
{
	return map.empty();
}

template<class K, class V>
V& Map<K, V>::operator[](const K& key)
{
	return map.get(Pair<K, V>(key, V())).value;
}

template<class K, class V>
const V& Map<K, V>::operator[](const K& key) const
{
	return map.get(Pair<K, V>(key, V())).value;
}

template<class K, class V>
V& Map<K, V>::at(const K& key)
{
	return map.get(Pair<K, V>(key, V())).value;
}

template<class K, class V>
const V& Map<K, V>::at(const K& key) const
{
	return at(key);
}

template<class K, class V>
void Map<K, V>::insert(const K& k, const V& v)
{
	map.insert(Pair<K, V>(k, v));
}

template<class K, class V>
void Map<K, V>::erase(const K& key)
{
	map.remove(Pair<K, V>(key, V()));
}

template<class K, class V>
void Map<K, V>::clear()
{
	map.clear_tree();
}

template<class K, class V>
V Map<K, V>::get(const K& key)
{
	return map.get(Pair<K, V>(key, V())).value;
}

template<class K, class V>
typename Map<K,V>::Iterator Map<K, V>::find(const K& key)
{
	return Iterator(at(key));
}

template<class K, class V>
bool Map<K, V>::contains(const Pair<K, V>& target)
{
	return map.contains(target);
}

#endif // MAP_H
