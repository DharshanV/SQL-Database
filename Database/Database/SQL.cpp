#include "SQL.h"

SQL::SQL()
{
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
		if (reader.eof())break;
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

		break;
	case Keyword::SELECT:

		break;
	default:
		break;
	}

	return true;
}

void SQL::makeTable(const string& tableName,const vector<string>& fields)
{
	if (!tables.contains(tableName)) {
		tables.insert(tableName, Table(tableName, fields));
	}
}

void SQL::saveTables()
{
	Map<string, Table>::Iterator it;
	for (it = tables.begin(); it != tables.end(); it++) {

	}
}
