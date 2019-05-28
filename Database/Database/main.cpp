#include <iostream>
#include "testFunctions.h"
#include "Table.h"
using namespace std;

int main() {
	string testInputs = "create table student fields last, first, major";
	Parser parser(testInputs);
	MMap<string, string> tree = parser.parse_tree();
	//cout << tree << endl;
	Table table(tree["TABLE_NAME"][0],tree["FIELD_NAME"]);
	return 0;
}