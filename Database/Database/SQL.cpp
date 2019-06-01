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
		if (quit)this->quit();
		valid = executeCommand(input);
		if (!valid) printHelp();
	} while (quit != true);
}

void SQL::run(const char* fileName)
{
	ifstream reader(fileName);
	string line;
	bool valid = true;
	while (!reader.eof()) {
		getline(reader, line);
		cout << ">> " << line << endl;
		valid = executeCommand(line);
	}
}

void SQL::quit()
{
	saveTables();
}

void SQL::printHelp()
{

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

bool SQL::executeCommand(const string& command)
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

		break;
	default:
		break;
	}

	return true;
}

vector<string> SQL::getTableFields(const string& tableName)
{
	vector<string> fields;
	ifstream in("Data\\" + tableName + "_fields.txt");
	string field;
	while (!in.eof()) {
		getline(in, field);
		if (in.eof())break;
		fields += field;
	}
	return fields;
}

void SQL::makeTable(const string& tableName,const vector<string>& fields)
{
	if (!tables.contains(tableName)) {
		tables.insert(tableName, Table(tableName, fields));
	}
}

void SQL::insertTable(const string& tableName, const vector<string>& value)
{
	if (!tables.contains(tableName)) {
		cout << tableName << " table has not been created" << endl;
		return;
	}
	tables[tableName].insert(value);
	tables[tableName].selectAll();
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