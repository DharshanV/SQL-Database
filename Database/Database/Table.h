#ifndef TABLE_H
#define TABLE_H
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include <algorithm>
#include "MMap.h"
#include "map.h"
#include "Record.h"
#include "stack.h"
#include "queue.h"
using namespace std;

class Table
{
public:
	Table();
	Table(const string& tableName);
	Table(const string& tableName, const vector<string>& fields);
	Table& operator=(const Table& other);

	void create();
	void open();
	void reIndex();

	void insert(const vector<string>& data);
	Table selectAll();
	Table select(const vector<string>& fields);
	Table selectCondition(const vector<string>& condition);
	Table selectFieldAndCon(const vector<string>& fields,
							const vector<string>& condition);
	friend ostream& operator<<(ostream& outs, const Table& print_me) {
		print_me.print(outs);
		return outs;
	}
private:
	void print(ostream& outs) const;
	Keyword getType(const string& value);
	vector<Record> getRecords(const vector<long>& recordIndex);
	Queue<string> getShantingYard(const vector<string>& condition);
	vector<long> intersection(vector<long> left,vector<long> right);
	vector<long> _union(vector<long> left, vector<long> right);
	vector<long> getLower(vector<string>& commands,bool equal);
	vector<long> getUpper(vector<string>& commands, bool equal);
	vector<long> getRecIndices(const vector<string>& condition);
private:
	string tableName;
	vector<long> recNo;
	vector<string> fields;
	Map<string, MMap<string, long>> indices;
	Map<string, int> fieldIndex;
	const string fieldExt = "_fields.txt";
	const string binaryExt = "_tbl.bin";
	static int fileCount;
};
#endif // !TABLE_H

