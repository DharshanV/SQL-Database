#include <iostream>
#include "testFunctions.h"
#include "Table.h"
using namespace std;

int main() {
	string testInputs = "create table student fields last, first, major";
	Parser parser(testInputs);
	MMap<string, string> tree = parser.parse_tree();
	Table table(tree["TABLE_NAME"][0],tree["FIELD_NAME"]);
	parser = Parser("insert into student values vishwanatha, dharshan, CS");
	tree = parser.parse_tree();
	table.insert(tree["VALUES"]);

	parser = Parser("insert into student values seb, vin, math");
	tree = parser.parse_tree();
	table.insert(tree["VALUES"]);
	return 0;
}