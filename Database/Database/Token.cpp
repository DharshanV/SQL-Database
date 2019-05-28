#include "Token.h"

Token::Token() {
}


Token::~Token() {
}

Token::Token(string str, int type) {
	_token = str;
	_type = type;
}

int Token::type() {
	return _type;
}

string Token::type_string()
{
	switch (_type)
	{
	case ALPHA:
		return "ALPHA";
	case NUMBER:
		return "NUMBER";
	case SPACE:
		return "SPACE";
	case EQUAL:
		return "EQUAL";
	case LESS:
		return "LESS";
	default:
		return "UNKNOWN";
	}
}

string Token::token_str()
{
	return _token;
}

ostream& operator<<(ostream& outs, const Token& t)
{
	outs <<"|"<< t._token <<"|";
	return outs;
}
