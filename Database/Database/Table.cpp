#include "Table.h"

Table::Table(const string& tableName)
{
	this->tableName = tableName;
	create();
}

Table::Table(const string& tableName, const vector<string>& fields)
{
	this->tableName = tableName;
	this->fields = fields;
	create();
	open();
	cout << indices;
}


Table::~Table()
{
}

void Table::create()
{
	ofstream out;
	out.open(tableName + binaryExt);
	out.close();
	out.clear();
	out.open(tableName + fieldExt);
	out.close();
}

void Table::open()
{
	ofstream out;
	out.open(tableName + fieldExt);
	if (out.fail()) cout << "failed to open file" << endl;
	string output;
	output += to_string(fields.size());
	output += "\n";
	for (int i = 0; i < fields.size(); i++) {
		output += fields[i];
		output += "\n";
		indices[fields[i]] = MMap<string, long>();
	}
	out << output;
	out.close();
}

void Table::insert(const vector<string>& data)
{
}
