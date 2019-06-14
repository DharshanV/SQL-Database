#ifndef SQL_H
#define SQL_H
#include "Table.h"
#include "STokenizer.h"
#include "Parser.h"
#include "Record.h"

using namespace std;
typedef const vector<string>& cvstring;
typedef const string& cstring;
class SQL
{
public:
	SQL();
	~SQL();
	void run();
	void run(const char* fileName);
	void quit();
private:
	void makeTable(cstring tableName, cvstring fields);
	void insertTable(cstring tableName, cvstring value);
	void selectTable(cstring tableName, cvstring fields,cvstring condition);
	void getInput(string& input,bool& quit);
	void printHelp();
	void saveTables();
	void removeAllSelect();
	void loadPreviosSessions();
	bool hasPreviousSessions();
	bool executeCommand(cstring command);
	vector<string> getTableFields(cstring tableName);
private:
	Map<string, Table> tables;
};

#endif // !SQL_H