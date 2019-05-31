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

	table.selectAll();

	parser = Parser("select first,major from student");
	tree = parser.parse_tree();
	table.select(tree["FIELD_NAME"]);
}

void tableTest2() {
	ifstream in("inputs2.txt");
	string line;
	while (!in.eof()) {
		getline(in, line);
		Parser parser(line);
		MMap<string, string> tree = parser.parse_tree();
		Keyword type = parser.commandType();
		if (type == CREATE) {
			cout << "CREATING NEW TABLE " << line << endl;
			Table table(tree["TABLE_NAME"][0], tree["FIELD_NAME"]);
		}
		else if (type = INSERT) {
			cout << "INSERTING " << line << endl;
			Table table(tree["TABLE_NAME"][0]);
			table.insert(tree["VALUES"]);
			table.selectAll();
		}
		cout << "==============================" << endl;
	}
}

void tableTest3() {
	ifstream in("inputs2.txt");
	string line;
	Table table;
	while (!in.eof()) {
		getline(in, line);
		Parser parser(line);
		MMap<string, string> tree = parser.parse_tree();
		Keyword type = parser.commandType();
		if (type == CREATE) {
			cout << "CREATING NEW TABLE " << line << endl;
			table = Table(tree["TABLE_NAME"][0], tree["FIELD_NAME"]);
		}
		else if (type == INSERT) {
			cout << "INSERTING " << line << endl;
			table.insert(tree["VALUES"]);
		}
		else if (type == SELECT) {
			cout << "SELECTING " << line << endl;
			table.select(tree["FIELD_NAME"]);
		}
		cout << "==============================" << endl;
	}
}

void tableTest4() {
	ifstream in("inputs3.txt");
	string line;
	Table table;
	while (!in.eof()) {
		getline(in, line);
		Parser parser(line);
		MMap<string, string> tree = parser.parse_tree();
		Keyword type = parser.commandType();
		if (type == CREATE) {
			cout << "CREATING NEW TABLE " << line << endl;
			table = Table(tree["TABLE_NAME"][0], tree["FIELD_NAME"]);
		}
		else if (type == INSERT) {
			cout << "INSERTING " << line << endl;
			table.insert(tree["VALUES"]);
		}
		else if (type == SELECT) {
			cout << "SELECTING " << line << endl;
			if (tree.contains("CONDITION")) {
				table.selectCondition(tree["CONDITION"]);
			}
			else {
				table.select(tree["FIELD_NAME"]);
			}
		}
		cout << "==============================" << endl;
	}
}

int main() {
	tableTest4();
	return 0;
}