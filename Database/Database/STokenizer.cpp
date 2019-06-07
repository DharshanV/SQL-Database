#include "STokenizer.h"

int STokenizer::_table[MAX_ROWS][MAX_COLUMNS];

STokenizer::STokenizer()
{
	make_table(this->_table);							//Makes the alpha, number, and space table
	_pos = 0;
}


STokenizer::~STokenizer()
{
}

STokenizer::STokenizer(const string& input)
{
	//resets all values.
	make_table(this->_table);
	_pos = 0;
	int i = 0;
	memset(_buffer, '\0', sizeof(_buffer));				//Sets buffer to null
	memcpy(_buffer, input.c_str(), MAX_BUFFER);					//Copies the passed in cstring
}

STokenizer::STokenizer(char str[]) {
	//resets all values.
	make_table(this->_table);
	_pos = 0;
	int i = 0;
	memset(_buffer, '\0', sizeof(_buffer));				//Sets buffer to null
	memcpy(_buffer, str, MAX_BUFFER);					//Copies the passed in cstring
}

bool STokenizer::done()
{
	return !more();
}

bool STokenizer::more()
{
	return _buffer[_pos] != '\0';
}

void STokenizer::set_string(char str[]) {
	int i = 0;
	_pos = 0;
	memset(_buffer, '\0', sizeof(_buffer));
	memcpy(_buffer, str, MAX_BUFFER);
}

void STokenizer::set_string(const string& str)
{
	int i = 0;
	_pos = 0;
	memset(_buffer, '\0', sizeof(_buffer));
	memcpy(_buffer, str.c_str(), MAX_BUFFER);
}

void STokenizer::make_table(int _table[][MAX_COLUMNS]) {
	init_table(_table);

	//_table for double and number
	mark_fail(_table, NUMBER);
	mark_success(_table, NUMBER + 1);
	mark_fail(_table, NUMBER + 2);
	mark_success(_table, NUMBER + 3);

	//_table for Alpha
	mark_fail(_table, ALPHA);
	mark_success(_table, ALPHA + 1);

	//_table for Spaces
	mark_fail(_table, SPACE);
	mark_success(_table, SPACE + 1);

	//_table for Tabs
	mark_fail(_table, TABS);
	mark_success(_table, TABS + 1);

	//EQUAL
	mark_success(_table, EQUAL);

	//LESS
	mark_fail(_table, LESS);
	mark_success(_table, LESS+1);

	//GREATER
	mark_fail(_table, GREATER);
	mark_success(_table, GREATER + 1);

	//For NUMBER
	mark_cells(NUMBER, _table, '0', '9', NUMBER + 1);
	mark_cells(NUMBER, _table, '.', '.', NUMBER + 2);
	mark_cells(NUMBER + 1, _table, '0', '9', NUMBER + 1);
	mark_cells(NUMBER + 1, _table, '.', '.', NUMBER + 2);
	mark_cells(NUMBER + 2, _table, '0', '9', NUMBER + 3);
	mark_cells(NUMBER + 3, _table, '0', '9', NUMBER + 3);

	//For ALPHA
	mark_cells(ALPHA, _table, 'a', 'z', ALPHA + 1);
	mark_cells(ALPHA + 1, _table, 'a', 'z', ALPHA + 1);
	mark_cells(ALPHA, _table, 'A', 'Z', ALPHA + 1);
	mark_cells(ALPHA + 1, _table, 'A', 'Z', ALPHA + 1);

	mark_cells(ALPHA, _table, '_', '_', ALPHA + 1);
	mark_cells(ALPHA + 1, _table, '_', '_', ALPHA + 1);

	//For SPACES
	mark_cells(SPACE, _table, ' ', ' ', SPACE + 1);
	mark_cells(SPACE + 1, _table, ' ', ' ', SPACE + 1);

	//For TABS
	mark_cells(TABS, _table, '\t', '\t', TABS + 1);
	mark_cells(TABS + 1, _table, '\t', '\t', TABS + 1);

	//For EQUAL
	mark_cells(EQUAL, _table, '=', '=', EQUAL);

	//For LESS
	mark_cells(LESS, _table, '<', '<', LESS+1);
	mark_cells(LESS+1, _table, '<', '<', LESS+1);
	mark_cells(LESS+1, _table, '=', '=', EQUAL);

	//For GREATER
	mark_cells(GREATER, _table, '>', '>', GREATER + 1);
	mark_cells(GREATER + 1, _table, '>', '>', GREATER + 1);
	mark_cells(GREATER + 1, _table, '=', '=', EQUAL);

	//For QUOTES
	mark_cells(QUOTES_, _table, '\"', '\"', QUOTES_ + 1);
	mark_cells(QUOTES_ + 1, _table, '\"', '\"', QUOTES_ + 1);

	//Marking pointer cells
	mark_cells(POINTER, _table, '0', '9', NUMBER);
	mark_cells(POINTER, _table, '.', '.', NUMBER);
	mark_cells(POINTER, _table, 'a', 'z', ALPHA);
	mark_cells(POINTER, _table, 'A', 'Z', ALPHA);
	mark_cells(POINTER, _table, '_', '_', ALPHA);
	mark_cells(POINTER, _table, ' ', ' ', SPACE);
	mark_cells(POINTER, _table, '\t', '\t', TABS);
	mark_cells(POINTER, _table, '=', '=', EQUAL);
	mark_cells(POINTER, _table, '<', '<', LESS);
	mark_cells(POINTER, _table, '>', '>', GREATER);
	mark_cells(POINTER, _table, '\"', '\"', QUOTES_);


	mark_fail(_table, QUOTES_);
	mark_success(_table, QUOTES_ + 2);
	mark_cells(QUOTES_, _table, '\"', '\"', QUOTES_+1);
	mark_cells(QUOTES_ + 1, _table, 1, '\"'-1, QUOTES_ + 1);
	mark_cells(QUOTES_ + 1, _table, '\"' + 1, MAX_COLUMNS, QUOTES_ + 1);
	mark_cells(QUOTES_ + 1, _table, '\"', '\"', QUOTES_ + 2);
}

bool STokenizer::getToken(int start_state, string& token)
{
	return get_token(_buffer, _pos, _table, start_state, token);
}

STokenizer& operator>>(STokenizer& s, Token& t)
{
	string token;
	int pointerState = s._table[POINTER][s._buffer[s._pos]];	//Gets the valid pointer table index
	bool isValid = s.getToken(pointerState, token);
	if (isValid) {
		t = Token(token, pointerState);
	}
	else {
		token += s._buffer[s._pos];
		t = Token(token, -1);
		s._pos++;
	}
	return s;
}
