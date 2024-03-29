#ifndef TEST_FUNCTIONS_H
#define TEST_FUNCTIONS_H
#include <vector>
#include "STokenizer.h"
#include "MMap.h"
#include "map.h"
#include "Parser.h"
#include "Record.h"
#include "Table.h"
void parserTest1() {
	string testInputs[] = { "select * from student",
						"select lname from student",
						"select lname,fname from student",
						"select dhausds from fuusahds student" };
	int size = (sizeof(testInputs) / sizeof(*testInputs));
	for (int i = 0; i < size; i++) {
		cout << "==============" << endl;
		cout << "Input: " << testInputs[i] << endl << endl;
		Parser parser(testInputs[i]);
		MMap<string, string> map = parser.parse_tree();
		cout << map;
	}
	cout << "==============" << endl;
}

void parserTest2() {
	string testInputs[] = { "select * from student where age > 9",
					"select lname,fname from student where age < 10",
					"select lname from student where age >= 15" };
	int size = (sizeof(testInputs) / sizeof(*testInputs));
	for (int i = 0; i < size; i++) {
		cout << "==============" << endl;
		cout << "Input: " << testInputs[i] << endl << endl;
		Parser parser(testInputs[i]);
		MMap<string, string> map = parser.parse_tree();
		cout << map;
	}
	cout << "==============" << endl;
}

void parserTest3() {
	string testInputs[] =
	{ "select lname from student where fname >= alex and age <= 10",
		"select fname,lname from student where age = 10 and fname > jake",
		"select * from student where age >= 0 or fname < apple" };
	int size = (sizeof(testInputs) / sizeof(*testInputs));
	for (int i = 0; i < size; i++) {
		cout << "==============" << endl;
		cout << "Input: " << testInputs[i] << endl << endl;
		Parser parser(testInputs[i]);
		MMap<string, string> map = parser.parse_tree();
		cout << map;
	}
	cout << "==============" << endl;
}

void parserTest4() {
	string testInputs[] =
	{ "select * from student where name >= \"alex bob\"",
		"select lname,fname from student where name >= \"alex bob\" and age <= 10" };
	int size = (sizeof(testInputs) / sizeof(*testInputs));
	for (int i = 0; i < size; i++) {
		cout << "==============" << endl;
		cout << "Input: " << testInputs[i] << endl << endl;
		Parser parser(testInputs[i]);
		MMap<string, string> map = parser.parse_tree();
		cout << map;
	}
	cout << "==============" << endl;
}

void insertTest1() {
	string testInputs[] =
	{ "insert into student values lname",
		"insert into student values lname,fname,10",
		"insert into student TRASH lname,,10",
		"insert into student values lname fname 10" };
	int size = (sizeof(testInputs) / sizeof(*testInputs));
	for (int i = 0; i < size; i++) {
		cout << "==============" << endl;
		cout << "Input: " << testInputs[i] << endl << endl;
		Parser parser(testInputs[i]);
		MMap<string, string> map = parser.parse_tree();
		cout << map;
	}
	cout << "==============" << endl;
}

void insertTest2() {
	string testInputs[] =
	{ "insert into student values Yang, Bo, \"Computer Science\"",
		"insert into student values Davis, John, Math",
		"insert into student values Johnson, \"Mary Ann\", \"Biology\"",
		"insert into employee values Jackson, David, Finance, 2016",
		"insert into employee values Davidson, \"Mary Alice\", complaints, 2012",
		"insert into employee values \"Mary Alice\", complaints, 2012" };
	int size = (sizeof(testInputs) / sizeof(*testInputs));
	for (int i = 0; i < size; i++) {
		cout << "==============" << endl;
		cout << "Input: " << testInputs[i] << endl << endl;
		Parser parser(testInputs[i]);
		MMap<string, string> map = parser.parse_tree();
		cout << map;
	}
	cout << "==============" << endl;
}

void createTest1() {
	string testInputs[] =
	{ "create table student fields last, first, major",
		"create table employee fields lname, fname, dept, year_hired" };
	int size = (sizeof(testInputs) / sizeof(*testInputs));
	for (int i = 0; i < size; i++) {
		cout << "==============" << endl;
		cout << "Input: " << testInputs[i] << endl << endl;
		Parser parser(testInputs[i]);
		MMap<string, string> map = parser.parse_tree();
		cout << map;
	}
	cout << "==============" << endl;
}

void createTest2() {
	string testInputs[] =
	{ "make table student fields last, first, major",
		"make table employee fields lname, fname, dept, year_hired" };
	int size = (sizeof(testInputs) / sizeof(*testInputs));
	for (int i = 0; i < size; i++) {
		cout << "==============" << endl;
		cout << "Input: " << testInputs[i] << endl << endl;
		Parser parser(testInputs[i]);
		MMap<string, string> map = parser.parse_tree();
		cout << map;
	}
	cout << "==============" << endl;
}

void recordTest() {
	vector<string> data[] = { {"john","smith","cs","24"},
							{"a","b","Math","18"} };
	Record list[] = { data[0],data[1] };
	//print_list(list, 2);
	save_list(list, 2);
	return;
}

void selectTests(int index) {
	void(*funs[])() = { parserTest1,parserTest2,parserTest3,parserTest4 };
	int size = (sizeof(funs) / sizeof(*funs));
	if (index == 0) {
		for (int i = 0; i < size; i++) {
			funs[i]();
		}
		return;
	}
	funs[index - 1]();
}

void insertTests(int index) {
	void(*funs[])() = { insertTest1,insertTest2 };
	int size = (sizeof(funs) / sizeof(*funs));
	if (index == 0) {
		for (int i = 0; i < size; i++) {
			funs[i]();
		}
		return;
	}
	funs[index - 1]();
}

void createTests(int index) {
	void(*funs[])() = { createTest1,createTest2 };
	int size = (sizeof(funs) / sizeof(*funs));
	if (index == 0) {
		for (int i = 0; i < size; i++) {
			funs[i]();
		}
		return;
	}
	funs[index - 1]();
}

void inputTest() {
	string input;
	cout << ">> ";
	getline(cin, input);
	cout << "Input: " << input << endl << endl;
	Parser parser(input);
	MMap<string, string> map = parser.parse_tree();
	cout << map;
}

void fileTest() {
	ifstream in("inputs.txt");
	if (in.fail())cout << "failed to open" << endl;
	string line;
	int i = 1;
	while (!in.eof()) {
		getline(in, line);
		cout << i << ". " << line << endl;
		Parser parser(line);
		MMap<string, string> map = parser.parse_tree();
		cout << "==========" << endl;
		cout << map;
		cout << "==========" << endl;
		i++;
	}
}

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


void parserTest() {
	selectTests(0);
	insertTests(0);
	createTests(0);
	fileTest();
}
#endif // !TEST_FUNCTIONS_H
