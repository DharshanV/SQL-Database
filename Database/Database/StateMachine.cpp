#include "StateMachine.h"

void init_table(int _table[][MAX_COLUMNS]) {
	for (int i = 0; i < MAX_ROWS; i++) {
		for (int j = 0; j < MAX_COLUMNS; j++) {
			if (j == 0) {
				_table[i][j] = 0;
			}
			else {
				_table[i][j] = -1;
			}
		}
	}
}

void mark_success(int _table[][MAX_COLUMNS], int state) {
	_table[state][0] = 1;
}

void mark_fail(int _table[][MAX_COLUMNS], int state) {
	_table[state][0] = 0;
}

void mark_cells(int row, int _table[][MAX_COLUMNS], int from,
	int to, int state) {
	for (int i = from; i <= to; i++) {
		_table[row][i] = state;
	}
}

void mark_cells(int row, int _table[][MAX_COLUMNS],
	const char columns[], int state) {
	int index = 0;
	while (columns[index] != '\0')
	{
		_table[row][(int)columns[index]] = state;
		index++;
	}
}

void mark_cell(int row, int _table[][MAX_COLUMNS], int column, int state) {
	_table[row][column] = state;
}

bool is_success(int _table[][MAX_COLUMNS], int state) {
	return _table[state][0];
}

void show_string(const char s[], int _pos) {
	cout << s << "\n";
	for (int i = 0; i < _pos; i++) {
		cout << " ";
	}
	cout << "^" << endl;
}

void print_table(int _table[][MAX_COLUMNS]) {
	for (int i = 0; i < MAX_ROWS; i++) {
		for (int j = 0; j < MAX_COLUMNS; j++) {
			cout << _table[i][j] << " ";
		}
		cout << endl;
	}
}

bool get_token(char input[], int& pos,
	int table[][MAX_COLUMNS], int state, string& token) {
	int index = pos;
	bool isValid = false;
	int lastValidPos = -1;
	int initialPos = pos;
	while (input[index] != '\0' && state != -1)
	{
		state = table[state][(int)input[index]];
		if (state == -1) {
			isValid = false;
			break;
		}
		if (table[state][0]) {
			lastValidPos = index;
			pos = lastValidPos + 1;
		}
		index++;
	}
	if (lastValidPos != -1) {
		isValid = true;
	}
	for (int i = initialPos; i <= lastValidPos; i++) {
		token += input[i];
	}
	return isValid;
}