#include "Table.h"
int Table::fileCount = 0;

Table::Table()
{
}

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

Table& Table::operator=(const Table& other) {
	tableName = other.tableName;
	fields = other.fields;
	indices = other.indices;
	fieldIndex = other.fieldIndex;
	return *this;
}

void Table::create()
{
	//using the text file to store a table fields
	fstream f;
	open_fileRW(f, ("Data\\" + tableName + fieldExt).c_str());
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
	//create the bin and text file that is used for resession
	fstream f;
	open_fileRW(f, ("Data\\" + tableName + binaryExt).c_str());
	f.close();
	f = fstream();
	open_fileRW(f, ("Data\\" + tableName + fieldExt).c_str());
	f.close();
}

void Table::insert(const vector<string>& data)
{
	//save the data into the bin file (calling record write)
	//but also index its values into the map for selecting
	fstream f;
	string name = "Data\\" + tableName + binaryExt;
	open_fileRW(f, name.c_str());
	Record r(data);
	long fileIndex = r.write(f);
	for (int i = 0; i < fields.size(); i++) {
		indices[fields[i]][data[i]] += fileIndex;
	}
}

Table Table::selectAll()
{
	return select(fields);
}

Table Table::select(const vector<string>& fields)
{
	//Look through the entire bin file, and select only
	//the necessary ones. This does not contain where
	//conditions. But only fields such as lname...
	fstream f;
	open_fileRW(f, ("Data\\" + tableName + binaryExt).c_str());
	if (f.fail()) return Table();
	//Temporary table to create bin and txt file of the selected request
	Table t("select" + to_string(fileCount++),fields);
	Record r;
	int recordNo = 0;
	while (f.good()) {
		vector<string> temp;
		r.read(f, recordNo);
		if (f.eof())break;
		for (int i = 0; i < fields.size(); i++) {
			//mapping from string to index inorder
			//to get the specific data from the record
			int fieldI = fieldIndex[fields[i]];
			temp += string(r.buffer[fieldI]);
		}
		t.insert(temp);
		recordNo++;
	}
	f.close();
	cout << t << endl;
	t.drop();			//droping or deleting the temp created files
	return t;
}

Table Table::selectCondition(const vector<string>& condition) {
	//selecting based on only the condition, and all the fields
	Table t("select"+to_string(fileCount++), fields);
	//we need to do shuntingyard because the order of "and" and "or"
	//execution matters
	vector<Record> records = getRecords(getRecIndices(condition));
	for (int j = 0; j < records.size(); j++) {
		vector<string> temp;
		for (int k = 0; k < fields.size(); k++) {
			temp += string(records[j].buffer[k]);
		}
		t.insert(temp);
	}
	cout << t << endl;
	t.drop();
	return t;
}

Table Table::selectFieldAndCon(const vector<string>& fields,
							const vector<string>& condition)
{
	//selection based on both the fields and condition
	Table t("select" + to_string(fileCount++), fields);
	vector<Record> records = getRecords(getRecIndices(condition));
	for (int j = 0; j < records.size(); j++) {
		vector<string> temp;
		for (int i = 0; i < fields.size(); i++) {
			int fieldI = fieldIndex[fields[i]];
			temp += string(records[j].buffer[fieldI]);
		}
		t.insert(temp);
	}
	cout << t << endl;
	t.drop();
	return t;
}

void Table::reIndex()
{
	//we have to reindex because if we had previous data,
	//we need to know where they are in order to select 
	//them again
	fstream f;
	open_fileRW(f, ("Data\\" + tableName + binaryExt).c_str());
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

void Table::drop()
{
	remove(string("Data\\" + tableName + binaryExt).c_str());
	remove(string("Data\\" + tableName + fieldExt).c_str());
}

vector<Record> Table::getRecords(const vector<long>& recordIndex)
{
	//given a list of rec index, get all the records associated
	//with that index
	fstream f;
	vector<Record> records;
	open_fileRW(f, ("Data\\" + tableName + binaryExt).c_str());
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
	//if !equal and we find it, then we need the next one
	//if !equal and we dont find it, then map.find will return a the current it
	//if equal and we dont find it, then it is fine
	//if equal and we find it, then its fine
	vector<long> temp;
	MMap<string, long>* map = &(indices[commands[0]]);
	MMap<string, long>::Iterator it = map->begin();
	MMap<string, long>::Iterator end = map->find(commands[1]);
	if (equal && !end.isNull() && (*end).key == commands[1]) {
		end++;
	}
	while (it != end) {
		temp += (*it).value;
		it++;
	}
	commands.clear();
	return temp;
}

vector<long> Table::getUpper(vector<string>& commands, bool equal)
{
	//if !equal and we find it, then we need the next one
	//if !equal and we dont find it, then map.find will return a the current it
	//if equal and we dont find it, then it is fine
	//if equal and we find it, then its fine
	vector<long> temp;
	MMap<string, long>* map = &(indices[commands[0]]);
	MMap<string, long>::Iterator start = map->find(commands[1]);
	if (!equal && !start.isNull() && (*start).key == commands[1]) {
		start++;
	}
	while (start != map->end()) {
		temp += (*start).value;
		start++;
	}
	commands.clear();
	return temp;
}

vector<long> Table::getRecIndices(const vector<string>& condition)
{
	Queue<string> shantingYard = getShantingYard(condition);
	Stack<vector<long> > recordIndices;
	vector<string> commands;
	while (!shantingYard.empty()) {
		if (shantingYard.front() == "=") {
			recordIndices += indices[commands[0]][commands[1]];
			shantingYard.pop();
			commands.clear();
		}
		else if (shantingYard.front() == "<") {
			recordIndices += getLower(commands, false);
			shantingYard.pop();
		}
		else if (shantingYard.front() == "<=") {
			recordIndices += getLower(commands, true);;
			shantingYard.pop();
		}
		else if (shantingYard.front() == ">") {
			recordIndices += getUpper(commands, false);
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
	return recordIndices.pop();
}

void Table::print(ostream & outs) const
{
	cout << setw(SETW) << std::left << " ";
	for (int i = 0; i < fields.size(); i++) {
		cout << setw(SETW) << std::left << fields[i];
	}
	cout << endl;
	fstream f;
	open_fileRW(f, ("Data\\"+tableName + binaryExt).c_str());
	if (f.fail()) return;
	int recordNo = 0;
	Record r;
	while (f.good()) {
		r.read(f, recordNo);
		if (f.eof())break;
		cout << setw(15) << std::left << string("Rec No " + to_string(recordNo) + ":");
		for (int i = 0; i < fields.size(); i++) {
			int fieldI = fieldIndex[fields[i]];
			cout << setw(SETW) << std::left << r.buffer[fieldI];
		}
		cout << endl;
		recordNo++;
	}
	f.close();
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