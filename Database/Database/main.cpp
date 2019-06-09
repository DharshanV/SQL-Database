#include <iostream>
#include "SQL.h"
#include "random.h"
using namespace std;
//TODO: work on STokenzier and change to detect quotes, 
//have a bool to keep track if i am inside quotes, if i am then take in both

//void getUpper(BPlusTree<int>& tree,int find, bool equal) {
//	cout << tree << endl;
//	BPlusTree<int>::Iterator start = tree.find(find);
//	if (equal == false) {
//		cout << "age > " << find << endl;
//		if (!start.is_null()) {
//			if (*start == find) {
//				start++;
//			}
//		}
//	}
//	else {
//		cout << "age >= " << find << endl;
//	}
//	while (start != tree.end()) {
//		cout << *start << endl;
//		start++;
//	}
//	cout << "========================" << endl;
//}
//
//void getLower(BPlusTree<int>& tree, int find, bool equal) {
//	cout << tree << endl;
//	BPlusTree<int>::Iterator start = tree.begin();
//	BPlusTree<int>::Iterator end = tree.find(find);
//	if (equal == false) {
//		cout << "age < " << find << endl;
//	}
//	else {
//		cout << "age <= " << find << endl;
//		if (!end.is_null()) {
//			if (*end == find) {
//				end++;
//			}
//		}
//	}
//	while (start != end) {
//		cout << *start << endl;
//		start++;
//	}
//	cout << "========================" << endl;
//}
//
//void greaterTest(BPlusTree<int>& tree) {
//	getUpper(tree, 1, false);
//	getUpper(tree, 3, false);
//	getUpper(tree, 5, false);
//	getUpper(tree, 8, false);
//	getUpper(tree, 23, false);
//	getUpper(tree, 25, false);
//	getUpper(tree, 28, false);
//	getUpper(tree, 30, false);
//	getUpper(tree, 33, false);
//	getUpper(tree, 38, false);
//}
//
//void greaterTestEqual(BPlusTree<int>& tree) {
//	getUpper(tree, 1, true);
//	getUpper(tree, 3, true);
//	getUpper(tree, 5, true);
//	getUpper(tree, 8, true);
//	getUpper(tree, 23, true);
//	getUpper(tree, 25, true);
//	getUpper(tree, 28, true);
//	getUpper(tree, 33, true);
//	getUpper(tree, 38, true);
//}
//
//void lowerTest(BPlusTree<int>& tree) {
//	getLower(tree, 1, false);
//	getLower(tree, 3, false);
//	getLower(tree, 5, false);
//	getLower(tree, 8, false);
//	getLower(tree, 23, false);
//	getLower(tree, 25, false);
//	getLower(tree, 28, false);
//	getLower(tree, 33, false);
//	getLower(tree, 38, false);
//}
//
//void lowerTestEqual(BPlusTree<int>& tree) {
//	getLower(tree, 1, true);
//	getLower(tree, 3, true);
//	getLower(tree, 5, true);
//	getLower(tree, 8, true);
//	getLower(tree, 23, true);
//	getLower(tree, 25, true);
//	getLower(tree, 28, true);
//	getLower(tree, 33, true);
//	getLower(tree, 38, true);
//}

int main() {
	//BPlusTree<int> tree;
	//for (int i = 3; i < 37; i+=5) {
	//	tree.insert(i);
	//}
	////>
	//greaterTest(tree);
	////>=
	////greaterTestEqual(tree);
	////lowerTest(tree);
	////lowerTestEqual(tree);
	//return 0;
	SQL sql;
	sql.run("inputs.txt");
	return 0;
}