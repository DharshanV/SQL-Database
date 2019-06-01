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
	void printHelp();
	void getInput(string& input,bool& quit);
	bool executeCommand(const string& command);
	void makeTable(const string& tableName, const vector<string>& fields);
	void saveTables();
private:
	Map<string, Table> tables;
};

#endif // !SQL_H