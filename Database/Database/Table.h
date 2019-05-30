#ifndef TABLE_H
#define TABLE_H
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "MMap.h"
#include "map.h"
#include "Record.h"
using namespace std;
class Table
{
public:
	Table(const string& tableName);
	Table(const string& tableName, const vector<string>& fields);
	~Table();
	void create();
	void open();
	void insert(const vector<string>& data);
	void selectAll();
	void select(const vector<string>& fields);
private:
	string tableName;
	vector<string> fields;
	Map<string, MMap<string, long>> indices;
	const string fieldExt = "_fields.txt";
	const string binaryExt = "_tbl.bin";
};
#endif // !TABLE_H

