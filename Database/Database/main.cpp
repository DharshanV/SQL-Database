#include <iostream>
#include "SQL.h"
using namespace std;
//TODO: work on STokenzier and change to detect quotes, 
//have a bool to keep track if i am inside quotes, if i am then take in both 

Keyword getType(const string& value)
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

vector<string> getShantingYard(const vector<string>& condition)
{
	Stack<string> stack;
	vector<string> shantingYard;
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

int main() {
	//string test = "select * from student where fname = Flo or lname = Jackson and f = g or j = k";

	//Parser parser(test);
	//MMap<string, string> tree = parser.parse_tree();
	//cout << getShantingYard(tree["CONDITION"]) << endl;
	//return 0;
	SQL sql;
	sql.run("inputs.txt");
	return 0;
}