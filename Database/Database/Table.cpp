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
	fstream f;
	open_fileRW(f, (tableName + fieldExt).c_str());
	if (f.fail()) cout << "failed to open file" << endl;
	string output;
	//output += to_string(fields.size());
	//output += "\n";
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
}

void Table::selectAll()
{
	select(fields);
}

void Table::select(const vector<string>& fields)
{
	cout << setw(SETW) << std::left << " ";
	for (int i = 0; i < fields.size(); i++) {
		cout << setw(SETW) << std::left << fields[i];
	}
	cout << endl;
	fstream f;
	open_fileRW(f, (tableName + binaryExt).c_str());
	if (f.fail()) return;
	Record r;
	int recordNo = 0;
	while (f.good()) {
		r.read(f, recordNo);
		if (f.eof())break;
		cout << setw(15) << std::left << string("Rec No "+to_string(recordNo)+":");
		for (int i = 0; i < fields.size(); i++) {
			int fieldI = fieldIndex[fields[i]];
			cout << setw(SETW)<<std::left<< r.buffer[fieldI];
		}
		cout << endl;
		recordNo++;
	}
	f.close();
}

void Table::selectCondition(const vector<string>& condition) {
	struct Expresion {
		Expresion(const string* f = NULL,const string* o = NULL,
				 const string* v = NULL) {
			field = f;
			op = o;
			value = v;
		}
		const string* field;
		const string* op;
		const string* value;
	};
	
	cout << setw(SETW) << std::left << " ";
	for (int i = 0; i < fields.size(); i++) {
		cout << setw(SETW) << std::left << fields[i];
	}
	cout << endl;

	for (int i = 0; i < condition.size(); i += 3) {
		Expresion ex(&condition[0], 
					&condition[1], &condition[2]);
		if (*ex.op == "=") {
			MMap<string, long>* mmPtr = &indices[*ex.field];
			vector<long>* vPtr = &(*mmPtr)[*ex.value];
			vector<Record> records = getRecords(*vPtr);
			for (int j = 0; j < records.size(); j++) {
				cout << setw(SETW) << std::left << string("Rec No " + to_string(j) + ":");
				for (int k = 0; k < fields.size(); k++) {
					cout << setw(SETW) << std::left << records[j].buffer[k];
				}
				cout << endl;
			}
		}
	}
}

void Table::reIndex()
{
	fstream f;
	open_fileRW(f, (tableName + binaryExt).c_str());
	long recIndex = 0;
	while (f.good()) {
		Record r;
		r.read(f, recIndex);
		if (f.eof()) break;
		for (int i = 0; i < fields.size(); i++) {
			string data(r.buffer[i]);
			indices[fields[i]][data] += recIndex;
		}
		recIndex++;
	}
}

vector<Record> Table::getRecords(const vector<long>& recordIndex)
{
	fstream f;
	vector<Record> records;

	open_fileRW(f, (tableName + binaryExt).c_str());

	for (int i = 0; i < recordIndex.size(); i++) {
		Record r;
		r.read(f, recordIndex[i]);
		records += r;
	}

	f.close();
	return records;
}
