#ifndef TOKEN_H
#define TOKEN_H
#include <iostream>
#include <string>
#include "constants.h"
using namespace std;
class Token
{
public:
	Token();
	~Token();
	Token(string str, int type);
	friend ostream& operator <<(ostream& outs, const Token& t);
	int type();
	string type_string();
	string token_str();
private:
	string _token;
	int _type;
};
#endif // !TOKEN_H
