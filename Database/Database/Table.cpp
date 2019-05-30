#include "Table.h"

Table::Table()
{
}

Table::Table(const string& tableName)
{
	this->tableName += "Data\\";
	this->tableName += tableName;
	open();
}

Table::Table(const string& tableName, const vector<string>& fields)
{
	this->tableName += "Data\\";
	this->tableName += tableName;
	this->fields = fields;
	open();
	create();
}


Table::~Table()
{
}

void Table::create()
{
	//ofstream out(tableName + fieldExt);
	//string output;
	//output += to_string(fields.size());
	//output += "\n";
	//for (int i = 0; i < fields.size(); i++) {
	//	output += fields[i];
	//	output += "\n";
	//	indices.insert(fields[i], MMap<string, long>());
	//}
	//out << output;
	//out.close();
	fstream f;
	open_fileRW(f, (tableName + fieldExt).c_str());
	if (f.fail()) cout << "failed to open file" << endl;
	string output;
	output += to_string(fields.size());
	output += "\n";
	for (int i = 0; i < fields.size(); i++) {
		output += fields[i];
		output += "\n";
		fieldIndex[fields[i]] = i;
		indices.insert(fields[i], MMap<string, long>());
	}
	f << output;
	f.close();
}

void Table::open()
{
	fstream f;
	open_fileRW(f, (tableName + binaryExt).c_str());
	f.close();
	f = fstream();
	open_fileRW(f, (tableName + fieldExt).c_str());
	f.close();
	//ofstream out;
	//out.open((tableName + binaryExt));
	//out.clear();
	//out.close();
	//out = ofstream();
	//out.open((tableName + fieldExt));
	//out.clear();
	//out.close();
}

void Table::insert(const vector<string>& data)
{
	fstream f;
	string name = tableName + binaryExt;
	open_fileRW(f, name.c_str());
	Record r(data);
	long fileIndex = r.write(f);
	for (int i = 0; i < fields.size(); i++) {
		indices[fields[i]][data[i]] += fileIndex;
	}
	//cout << indices << endl;
	//f.close();
	//f = fstream();
	//open_fileRW(f, name.c_str());
	//for (int i = 0; i < data.size(); i++) {
	//	Record r;
	//	cout << "Retrive data " << fields[i] << endl;
	//	r.read(f, indices[fields[i]][data[i]][0]);
	//	cout << r << endl;
	//}
	//f.close();
}

void Table::selectAll()
{
	cout << "Select All" << endl;
	fstream f;
	open_fileRW(f, (tableName + binaryExt).c_str());
	Record r;
	int i = 0;
	while (f.good()) {
		r.read(f, i);
		if (f.eof())break;
		cout << "=================" << endl;
		cout << "Record number: " << i << endl;
		cout << r << endl;
		i++;
	}
	f.close();
}

void Table::select(const vector<string>& fields)
{
	cout << "Select: " << fields << endl;
	fstream f;
	open_fileRW(f, (tableName + binaryExt).c_str());
	if (f.fail()) return;
	Record r;
	int recordNo = 0;
	cout << "**********************" << endl;
	while (f.good()) {
		r.read(f, recordNo);
		if (f.eof())break;
		cout << "Record number: " << recordNo << endl;
		for (int i = 0; i < fields.size(); i++) {
			int fieldI = fieldIndex[fields[i]];
			cout << fields[i] << ": " << r.buffer[fieldI] << endl;
		}
		cout << endl;
		recordNo++;
	}
	f.close();
}
