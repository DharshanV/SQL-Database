#include <iostream>
#include "testFunctions.h"
#include "Table.h"
using namespace std;

void tableTest1() {
	string testInputs = "create table student fields last, first, major";
	Parser parser(testInputs);
	MMap<string, string> tree = parser.parse_tree();
	Table table(tree["TABLE_NAME"][0], tree["FIELD_NAME"]);
	parser = Parser("insert into student values vishwanatha, dharshan, CS");
	tree = parser.parse_tree();
	table.insert(tree["VALUES"]);

	parser = Parser("insert into student values seb, vin, math");
	tree = parser.parse_tree();
	table.insert(tree["VALUES"]);
}

int main() {
	ifstream in("inputs2.txt");
	string line;
	while (!in.eof()) {
		getline(in, line);
		if (in.eof()) break;
		Parser parser(line);
		MMap<string, string> tree = parser.parse_tree();
		Keyword type = parser.commandType();
		if (type == CREATE) {
			Table table(tree["TABLE_NAME"][0], tree["FIELD_NAME"]);
		}
		else if (type = INSERT) {
			Table table(tree["TABLE_NAME"][0]);
			table.insert(tree["VALUES"]);
		}
	}
	return 0;
}