#ifndef F_STATE_MACHINE_H
#define F_STATE_MACHINE_H
#include <iostream>
#include <iomanip>
#include "constants.h"
#include <iomanip>
using namespace std;

//Fill all cells of the array with -1
void init_table(int _table[][F_MAX_COLUMNS]);

//Mark this state (row) with a 1 (success)
void mark_success(int _table[][F_MAX_COLUMNS], int state);

//Mark this state (row) with a 0 (fail)
void mark_fail(int _table[][F_MAX_COLUMNS], int state);

//true if state is a success state
bool is_success(int _table[][F_MAX_COLUMNS], int state);

//Mark a range of cells in the array. 
void mark_cells(int row, int _table[][F_MAX_COLUMNS], int from, int to, int state);

//Mark columns represented by the string columns[] for this row
void mark_cells(int row, int _table[][F_MAX_COLUMNS], const char columns[], int state);

//Mark this row and column
void mark_cell(int row, int table[][F_MAX_COLUMNS], int column, int state);

//This can realistically be used on a small table
void print_table(int _table[][F_MAX_COLUMNS]);

int getState(int _table[][F_MAX_COLUMNS], int colIndex);

//bool get_token(char input[], int& pos, int table[][F_MAX_COLUMNS], int state, string& token);

#endif // !F_STATE_MACHINE_H
