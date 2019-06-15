#include "Parser.h"

int Parser::table[F_MAX_ROWS][F_MAX_COLUMNS];

Parser::Parser(const string& input)
{
	valid = true;
	makeIndexes();
	makeTable();
	makeQueue(input);
}

MMap<string, string> Parser::parse_tree()
{
	MMap<string, string> tree;
	int rowIndex = 0;
	int colIndex = 0;
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
				if (rowIndex == 9) {
					tree["VALUES"] += input_q[i];
				}
				else {
					tree["TABLE_NAME"] += input_q[i];
				}
			}
			break;
		case 5:
			tree["FIELD_NAME"] += input_q[i];
			break;
		case 12:
		case 13:
		case 15:
			tree["CONDITION"] += input_q[i];
			break;
		case -1:
			valid = false;
			return tree;
			break;
		default:
			break;
		}
		valid = table[rowIndex][0];
		rowIndex = table[rowIndex][colIndex];
	}
	return tree;
}

Keyword Parser::commandType()
{
	switch (indexMap[input_q[0]])
	{
	case SELECT:
		return SELECT;
	case CREATE:	//or MAKE
		return CREATE;
	case INSERT:
		return INSERT;
	default:
		return ZERO;
		break;
	}
}

bool Parser::isValid()
{
	return valid;
}

void Parser::makeTable()
{
	//state machine
	init_table(table);

	mark_success(table, 3);
	mark_success(table, 6);
	mark_success(table, 9);
	mark_success(table, 13);

	mark_cell(0, table, 1, 1);
	mark_cell(0, table, 10, 1);
	mark_cell(0, table, 13, 1);

	mark_cell(1, table, 2, 2);
	mark_cell(1, table, 4, 5);
	mark_cell(1, table, 11, 3);
	mark_cell(1, table, 14, 3);

	mark_cell(2, table, 3, 3);

	mark_cell(3, table, 4, 4);

	mark_cell(4, table, 5, 9);
	mark_cell(4, table, 6, 11);
	mark_cell(4, table, 12, 9);
	mark_cell(4, table, 15, 10);

	mark_cell(5, table, 3, 3);
	mark_cell(5, table, 5, 6);

	mark_cell(6, table, 4, 5);

	mark_cell(7, table, 4, 5);

	mark_cell(8, table, 5, 9);
	mark_cell(8, table, 8, 7);

	mark_cell(9, table, 4, 4);
	mark_cell(9, table, 9, 8);

	mark_cell(10, table, 4, 5);

	//new stuff
	mark_cell(11, table, 4, 12);

	mark_cell(12, table, 7, 13);
	mark_cell(12, table, 8, 15);

	mark_cell(13, table, 4, 12);
	mark_cell(13, table, 9, 14);

	mark_cell(14, table, 8, 15);

	mark_cell(15, table, 4, 12);
}

void Parser::makeQueue(const string& input)
{
	//tokenize the given command into tokens
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
	//everything that is unknown is a symbol
	return indexMap.contains(word) ? indexMap[word] : SYMOBL;
}