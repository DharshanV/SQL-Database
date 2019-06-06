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

Table::Table(const string& tableName, const vector<string>& fields, const vector<long>& recNo)
{
	this->tableName += "Data\\";
	this->tableName += tableName;
	this->fields = fields;
	this->recNo = recNo;
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
	cout << setw(SETW) << std::left << " ";
	for (int i = 0; i < fields.size(); i++) {
		cout << setw(SETW) << std::left << fields[i];
	}
	cout << endl;

	Queue<string> shantingYard = getShantingYard(condition);
	cout << shantingYard << endl;
	Stack<vector<long> > recordIndices;
	vector<string> commands;
	while (!shantingYard.empty()) {
		if (shantingYard.front() == "=") {
			recordIndices += indices[commands[0]][commands[1]];
			shantingYard.pop();
			commands.clear();
		}
		else if (shantingYard.front() == "<") {
			recordIndices += getLower(commands,false);
			shantingYard.pop();
		}
		else if (shantingYard.front() == "<=") {
			recordIndices += getLower(commands, true);;
			shantingYard.pop();
		}
		else if (shantingYard.front() == ">") {
			recordIndices += getUpper(commands,false);
			shantingYard.pop();
		}
		else if (shantingYard.front() == ">=") {
			recordIndices += getUpper(commands, true);
			shantingYard.pop();
		}
		else if (shantingYard.front() == "and") {
			recordIndices += intersection(recordIndices.pop(), recordIndices.pop());
			shantingYard.pop();
			commands.clear();
		}
		else if (shantingYard.front() == "or") {
			recordIndices += _union(recordIndices.pop(), recordIndices.pop());
			shantingYard.pop();
			commands.clear();
		}
		else {
			commands += shantingYard.pop();
		}
	}
	vector<Record> records = getRecords(recordIndices.pop());
	for (int j = 0; j < records.size(); j++) {
		cout << setw(SETW) << std::left << string("Rec No " + to_string(j) + ":");
		for (int k = 0; k < fields.size(); k++) {
			cout << setw(SETW) << std::left << records[j].buffer[k];
		}
		cout << endl;
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

Queue<string> Table::getShantingYard(const vector<string>& condition)
{
	Stack<string> stack;
	Queue<string> shantingYard;
	for (int i = 0; i < condition.size(); i++) {
		Keyword type = getType(condition[i]);
		switch (type)
		{
		case RELATIONAL_OPERATOR:
			stack.push(condition[i]);
			break;
		case LOGICAL_OPERATOR:
			if (condition[i] == "and") {
				shantingYard += stack.pop();
				stack.push(condition[i]);
			}
			if (condition[i] == "or") {
				shantingYard += stack.pop();
				if (stack.empty()) {
					stack.push("or");
					break;
				}
				if (stack.top() == "and") {
					shantingYard += stack.pop();
					stack.push(condition[i]);
				}
				else {
					stack.push(condition[i]);
				}
			}
			break;
		case SYMOBL:
			shantingYard += condition[i];
			break;
		default:
			break;
		}
	}
	while (!stack.empty()) {
		shantingYard += stack.pop();
	}
	return shantingYard;
}

Keyword Table::getType(const string& value)
{
	if (value == "=" || value == "<" || value == "<="
		|| value == ">" || value == ">=") {
		return RELATIONAL_OPERATOR;
	}
	else if (value == "and" || value == "or") {
		return LOGICAL_OPERATOR;
	}
	else {
		return SYMOBL;
	}
}

vector<long> Table::getLower(vector<string>& commands, bool equal)
{
	vector<long> temp;
	MMap<string, long>* map = &(indices[commands[0]]);
	cout << *map << endl;
	MMap<string, long>::Iterator it;
	MMap<string, long>::Iterator end = map->find(commands[1]);
	if(equal)temp += indices[commands[0]][commands[1]];
	for (it = map->begin(); it != end; it++) {
		temp += (*it).value;
	}
	commands.clear();
	return temp;
}

vector<long> Table::getUpper(vector<string>& commands, bool equal)
{
	vector<long> temp;
	MMap<string, long>* map = &(indices[commands[0]]);
	MMap<string, long>::Iterator it;
	if (!map->contains(commands[1])) return temp;
	MMap<string, long>::Iterator start = map->find(commands[1]);
	if (!equal)start++;
	for (it = start; it != map->end(); it++) {
		temp += (*it).value;
	}
	commands.clear();
	return temp;
}

vector<long> Table::intersection(vector<long> left, vector<long> right) {
	vector<long> mergedVector;
	sort(left.begin(), left.end());
	sort(right.begin(), right.end());
	set_intersection(left.begin(), left.end(),
		right.begin(), right.end(), back_inserter(mergedVector));
	return mergedVector;
}

vector<long> Table::_union(vector<long> left, vector<long> right) {
	vector<long> mergedVector;
	sort(left.begin(), left.end());
	sort(right.begin(), right.end());
	set_union(left.begin(), left.end(),
		right.begin(), right.end(), back_inserter(mergedVector));
	return mergedVector;
}