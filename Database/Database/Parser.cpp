#include "Parser.h"

int Parser::table[F_MAX_ROWS][F_MAX_COLUMNS];

Parser::Parser(const string& input)
{
	makeIndexes();
	makeTable();
	makeQueue(input);
}

Parser::~Parser()
{
}

MMap<string, string> Parser::parse_tree()
{
	MMap<string, string> tree;
	int rowIndex = 0;
	int colIndex = 0;
	bool valid = true;
	for (int i = 0; i < input_q.size(); i++) {
		string value = toLower(input_q[i]);
		colIndex = stringToKey(value);
		switch (table[rowIndex][colIndex])
		{
		case 1:
			tree["COMMAND"] += value;
			break;
		case 2:
			tree["FIELD_NAME"] += input_q[i];
			break;
		case 4:
			if (table[rowIndex][0]) {
				tree["TABLE_NAME"] += input_q[i];
			}
			else {
				tree["VALUE"] += input_q[i];
			}
			break;
		case 5:
			tree["FIELD_NAME"] += input_q[i];
			break;
		case 8:
			tree["RELATIONAL_OPERATOR"] += input_q[i];
			break;
		case 9:
			tree["LOGICAL_OPERATOR"] += input_q[i];
			break;
		case 10:
			if (!extractValue(tree,i,value)) {
				cout << "INVALID COMMAND" << endl;
				return tree;
			}
			tree["VALUE"] += value;
			break;
		case 13:
			if (!extractValue(tree, i, value)) {
				cout << "INVALID COMMAND" << endl;
				return tree;
			}
			tree["FIELD_NAME"] += value;
			break;
		case -1:
			cout << "INVALID COMMAND" << endl;
			return tree;
			break;
		default:
			break;
		}
		rowIndex = table[rowIndex][colIndex];
	}
	return tree;
}

void Parser::makeTable()
{
	init_table(table);

	//SELECT
	makeSelect();
	//INSERT
	makeInsert();
	//CREATE AND MAKE
	makeCreate();
}

void Parser::makeQueue(const string& input)
{
	STokenizer st(input);
	Token t;
	while (!st.done()) {
		st >> t;
		if (t.type() != SPACE && t.type() != TABS) {
			input_q.push_back(t.token_str());
		}
	}
}

void Parser::makeIndexes()
{
	indexMap["select"] = SELECT;
	indexMap["insert"] = INSERT;
	indexMap["create"] = CREATE;
	indexMap["make"] = MAKE;
	indexMap["into"] = INTO;
	indexMap["values"] = VALUES;
	indexMap["*"] = STAR;
	indexMap["from"] = FROM;
	indexMap[","] = COMMA_K;
	indexMap["where"] = WHERE;
	indexMap["="] = RELATIONAL_OPERATOR;
	indexMap[">="] = RELATIONAL_OPERATOR;
	indexMap["<="] = RELATIONAL_OPERATOR;
	indexMap["<"] = RELATIONAL_OPERATOR;
	indexMap[">"] = RELATIONAL_OPERATOR;
	indexMap["and"] = LOGICAL_OPERATOR;
	indexMap["or"] = LOGICAL_OPERATOR;
	indexMap["\""] = QUOTES;
	indexMap["table"] = TABLE;
	indexMap["fields"] = FIELDS;
}

string Parser::toLower(const string& input)
{
	string temp;
	for (int i = 0; i < input.size(); i++) {
		temp += char(tolower(input[i]));
	}
	return temp;
}

Keyword Parser::stringToKey(const string& word)
{
	return indexMap.contains(word) ? indexMap[word] : SYMOBL;
}

bool Parser::extractValue(MMap<string, string>& tree, int& index,string& value)
{
	bool valid = false;
	int stopIndex = index;
	for (int i = index+1; i < input_q.size(); i++) {
		if (input_q[i] == "\"") {
			stopIndex = i ;
			valid = true;
			break;
		}
	}
	if (valid == false) return false;
	value.clear();
	for (int i = index+1; i <stopIndex; i++) {
		value += input_q[i];
		if(i+1!=stopIndex) value += " ";
	}
	index = stopIndex;
	return true;
}

void Parser::makeSelect()
{
	mark_success(table, 3);
	//Start -> select
	mark_cell(0, table, 1, 1);
	//select -> star
	mark_cell(1, table, 2, 2);
	//star -> from
	mark_cell(2, table, 3, 3);
	//from -> table name
	mark_cell(3, table, 4, 4);
	//select -> field
	mark_cell(1, table, 4, 5);
	//field -> from
	mark_cell(5, table, 3, 3);
	//field -> comma
	mark_cell(5, table, 5, 6);
	//comma -> field
	mark_cell(6, table, 4, 5);
	//table name -> where
	mark_cell(4, table, 6, 7);
	//where -> field
	mark_cell(7, table, 4, 5);
	//field -> RELATIONAL_OPERATOR
	mark_cell(5, table, 7, 8);
	//RELATIONAL_OPERATOR -> value
	mark_cell(8, table, 4, 4);
	//value -> logical op
	mark_cell(4, table, 8, 9);
	//logical op -> field
	mark_cell(9, table, 4, 5);
	//RELATIONAL_OPERATOR -> "
	mark_cell(8, table, 9, 10);
	//" -> logical op
	mark_cell(10, table, 8, 9);
}

void Parser::makeInsert()
{
	mark_cell(0, table, 10, 1);
	mark_cell(1, table, 11, 3);
	mark_cell(4, table, 12, 11);
	mark_cell(11, table, 4, 4);
	mark_cell(4, table, 5, 11);
	mark_cell(11, table, 9, 10);
	mark_cell(10, table, 5, 11);
}

void Parser::makeCreate()
{
	mark_cell(0, table, 13, 1);
	mark_cell(1, table, 14, 3);
	mark_cell(4, table, 15, 12);
	mark_cell(12, table, 4, 5);
}
