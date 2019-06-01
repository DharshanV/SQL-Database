#ifndef PARSER_H
#define PARSER_H
#include <iostream>
#include <string>
#include <vector>
#include "MMap.h"
#include "map.h"
#include "STokenizer.h"
#include "FStateMachine.h"
using namespace std;

class Parser
{
public:
	Parser(const string& input);
	~Parser();
	MMap<string, string> parse_tree();
	Keyword commandType();
	bool isValid();
private:
	void makeTable();
	void makeQueue(const string& input);
	void makeIndexes();
	string toLower(const string& input);
	Keyword stringToKey(const string& word);
	bool extractValue(MMap<string,string>& tree, int& index,string& value);
private:
	bool valid;
	vector<string> input_q;
	Map<string, Keyword> indexMap;
	static int table[F_MAX_ROWS][F_MAX_COLUMNS];
};

#endif // !PARSER_H