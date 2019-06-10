#ifndef RECORD_H
#define RECORD_H
#include <iostream>
#include <fstream>
#include <vector>
#include "constants.h"
using namespace std;

class Record {
public:
	Record() {
		recno = -1;
		recCount = 0;
	}
	Record(const vector<string>& data);
	long write(fstream& outs);
	long read(fstream& ins, long recno);
	friend ostream& operator<<(ostream& outs,
		const Record& r);
	vector<string> getVec(int size);
	int recno;
	int recCount;
	char buffer[R_MAX_ROW][R_MAX_COL];
};
bool file_exists(const char filename[]);
void open_fileRW(fstream& f, const char filename[]);
void open_fileW(fstream& f, const char filename[]);
void save_list(Record list[], int count);
void print_list(Record list[], int count);
#endif // !RECORD_H

