#include "SQL.h"

SQL::SQL()
{
	if (hasPreviousSessions()) {
		loadPreviosSessions();
	}
}

SQL::~SQL()
{
	quit();
}

void SQL::run()
{
	bool quit = false;
	bool valid = true;
	string input;
	do {
		cout << ">> ";
		getInput(input, quit);
		if (quit) { 
			this->quit();
			break; 
		}
		valid = executeCommand(input);
		if (!valid) printHelp();
	} while (quit != true);
}

void SQL::run(const char* fileName)
{
	ifstream reader(fileName);
	string line;
	bool valid = true;
	int i = 1;
	while (!reader.eof()) {
		getline(reader, line);
		if (line.size() != 0 && line.at(0) != '/') {
			cout <<i<< ". >> " << line << endl;
			valid = executeCommand(line);
			if (!valid) printHelp();
			cout << endl;
			i++;
		}
		else {
			cout << line << endl;
		}
	}
}

void SQL::quit()
{
	saveTables();
	removeAllSelect();
}

void SQL::printHelp()
{
	cout << "INVALID COMMAND" << endl;
}

void SQL::getInput(string& input, bool& quit)
{
	getline(cin, input);
	if (input == "quit") quit = true;
}

bool SQL::hasPreviousSessions()
{
	cout << "Checking for previous sessions..." << endl;
	ifstream in("Data\\sql.txt");
	if (in.fail()) {
		cout << "Found no previos sessions" << endl;
		ofstream out;
		out.open("Data\\sql.txt");
		out.close();
		return false;
	}
	in.seekg(0, in.end);
	int length = in.tellg();
	if (length > 0)return true;
	in.seekg(0, in.beg);
	in.close();
	return false;
}

void SQL::loadPreviosSessions()
{
	cout << "Previous sessions found" << endl;
	cout << "Loading previous sessions..." << endl;
	ifstream in("Data\\sql.txt");
	int tablesCount = 0;
	string tableName;
	while (!in.eof()) {
		getline(in, tableName);
		if (in.eof())break;
		tables.insert(tableName,Table(tableName,getTableFields(tableName)));
		cout << "Re-indexing " << tableName << " table..." << endl;
		tables[tableName].reIndex();
		tablesCount++;
	}
	cout << "Number of tables found: " << tablesCount << endl;
	in.close();
}

bool SQL::executeCommand(cstring command)
{
	bool valid = true;
	Parser parse(command);
	MMap<string, string> tree = parse.parse_tree();
	valid = parse.isValid();
	if (!valid) return false;

	switch (parse.commandType())
	{
	case Keyword::CREATE:
		makeTable(tree["TABLE_NAME"][0],tree["FIELD_NAME"]);
		break;
	case Keyword::INSERT:
		insertTable(tree["TABLE_NAME"][0], tree["VALUES"]);
		break;
	case Keyword::SELECT:
		selectTable(tree["TABLE_NAME"][0], 
					tree["FIELD_NAME"], tree["CONDITION"]);
		break;
	default:
		break;
	}

	return true;
}

vector<string> SQL::getTableFields(cstring tableName)
{
	vector<string> fields;
	ifstream in("Data\\" + tableName + "_fields.txt");
	if (in.fail())return fields;
	string field;
	while (!in.eof()) {
		getline(in, field);
		if (in.eof())break;
		fields += field;
	}
	return fields;
}

void SQL::makeTable(cstring tableName,cvstring fields)
{
	if (!tables.contains(tableName)) {
		tables.insert(tableName, Table(tableName, fields));
	}
}

void SQL::insertTable(cstring tableName,cvstring value)
{
	if (!tables.contains(tableName)) {
		cout << tableName << " table has not been created" << endl;
		return;
	}
	tables[tableName].insert(value);
}

void SQL::selectTable(cstring tableName,cvstring fields,cvstring condition)
{
	if (condition.size() == 0) {
		if (fields[0] == "*") {
			tables[tableName].selectAll();
		}
		else {
			tables[tableName].select(fields);
		}
	}
	else {
		if (fields[0] == "*") {
			tables[tableName].selectCondition(condition);
		}
		else {
			tables[tableName].selectFieldAndCon(fields, condition);
		}
	}

}

void SQL::saveTables()
{
	Map<string, Table>::Iterator it;
	ofstream out("Data\\sql.txt");
	for (it = tables.begin(); it != tables.end(); it++) {
		out << (*it).key << endl;
	}
	out.close();
}

void SQL::removeAllSelect() {
	bool valid = true;
	int i = 0;
	while (valid) {
		string fileName = "Data\\select" + to_string(i);
		ifstream in(string(fileName + "_fields.txt"));
		if (!in.is_open()) {
			valid = false;
			break;
		}
		in.close();
		remove(string(fileName + "_fields.txt").c_str());
		remove(string(fileName + "_tbl.bin").c_str());
		i++;
	}
}