#ifndef SQL_H
#define SQL_H
#include "Table.h"
#include "STokenizer.h"
#include "Parser.h"
#include "Record.h"

using namespace std;
class SQL
{
public:
	SQL();
	~SQL();
	void run();
	void run(const char* fileName);
	void quit();
private:
	void makeTable(const string& tableName, const vector<string>& fields);
	void insertTable(const string& tableName, const vector<string>& value);
	void getInput(string& input,bool& quit);
	void printHelp();
	void saveTables();
	void loadPreviosSessions();
	bool hasPreviousSessions();
	bool executeCommand(const string& command);
	vector<string> getTableFields(const string& tableName);
private:
	Map<string, Table> tables;
};

#endif // !SQL_H