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
	void run();												//cordinates all the functions
	void run(const char* fileName);							//runs with file with commands
	void quit();											//cleans up the files created and saves table names
private:
	void makeTable(cstring tableName, cvstring fields);		//makes the table
	void insertTable(cstring tableName, cvstring value);	//inserts into table whose values corrisponds to fields
	void selectTable(cstring tableName, cvstring fields,	//select from the table
					cvstring condition);
	void getInput(string& input,bool& quit);
	void printHelp();
	void saveTables();										//saves all the tables which have been created
	void loadPreviosSessions();								//reads through all the previous session and reIndex tables
	bool hasPreviousSessions();								//check if sql.txt has any table names
	bool executeCommand(cstring command);					//exectue each command checking for invalids
	vector<string> getTableFields(cstring tableName);		//gets the fields of table from the text file
private:
	Map<string, Table> tables;
};

#endif // !SQL_H