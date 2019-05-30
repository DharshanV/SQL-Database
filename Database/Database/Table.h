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
	Table();
	Table(const string& tableName);
	Table(const string& tableName, const vector<string>& fields);
	Table& operator=(const Table& other) {
		tableName = other.tableName;
		fields = other.fields;
		indices = other.indices;
		fieldIndex = other.fieldIndex;
		return *this;
	}
	~Table();
	void create();
	void open();
	void insert(const vector<string>& data);
	void selectAll();
	void select(const vector<string>& fields);
private:

private:
	string tableName;
	vector<string> fields;
	Map<string, MMap<string, long>> indices;
	Map<string, int> fieldIndex;
	const string fieldExt = "_fields.txt";
	const string binaryExt = "_tbl.bin";
};
#endif // !TABLE_H

