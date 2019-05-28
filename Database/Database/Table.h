#ifndef TABLE_H
#define TABLE_H
#include <iostream>
#include <string>
#include <vector>
#include "MMap.h"
#include "map.h"
using namespace std;
class Table
{
public:
	Table(const string& tableName);
	~Table();
	void create();
	void open();
	void insert();
	void selectAll();
	void select(const string& condition);
private:
	string tableName;
	vector<string> fields;
	Map<string, MMap<string, long>> indices;
};
#endif // !TABLE_H

