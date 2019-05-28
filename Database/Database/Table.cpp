#include "Table.h"

Table::Table(const string& tableName)
{
	this->tableName = tableName;
	open();
}

Table::Table(const string& tableName, const vector<string>& fields)
{
	this->tableName = tableName;
	this->fields = fields;
	open();
	create();
}


Table::~Table()
{
}

void Table::create()
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
		indices.insert(fields[i], MMap<string, long>());
	}
	out.close();
}

void Table::open()
{
	ofstream out;
	out.open(tableName + binaryExt);
	out.close();
	out = ofstream();
	out.open(tableName + fieldExt);
	out.close();
}

void Table::insert(const vector<string>& data)
{
	fstream f;
	string name = tableName + binaryExt;
	open_fileW(f, name.c_str());
	Record r(data);
	long fileIndex = r.write(f);
	for (int i = 0; i < fields.size(); i++) {
		indices[fields[i]][data[i]] += fileIndex;
	}
	f.close();
	cout << indices << endl;
	f = fstream();
	open_fileRW(f, name.c_str());
	for (int i = 0; i < data.size(); i++) {
		Record r;
		cout << "Retrive data " << fields[i] << endl;
		r.read(f, indices[fields[i]][data[i]][0]);
		cout << r << endl;
	}
	f.close();
}
