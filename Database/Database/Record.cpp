#include "Record.h"

ostream& operator<<(ostream& outs, const Record& r)
{
	for (int i = 0; i < R_MAX_ROW; i++) {
		cout << r.buffer[i] << endl;
	}
	return outs;
}

Record::Record(const vector<string>& data)
{
	for (int i = 0; i < data.size(); i++) {
		strcpy_s(buffer[i], data[i].c_str());
	}
	recCount = data.size();
	recno = -1;
}

long Record::write(fstream& outs)
{
	//write to the end of the file.
	long pos = outs.tellp();
	outs.seekg(0, ios_base::end);
	outs.write(reinterpret_cast<char*>(buffer), sizeof(buffer));
	return pos;
}

long Record::read(fstream& ins, long recno)
{
	long pos = recno * sizeof(buffer);
	ins.seekg(pos, ios_base::beg);
	ins.read(reinterpret_cast<char*>(buffer), sizeof(buffer));
	return ins.gcount();
}

bool file_exists(const char filename[])
{
	const bool debug = false;
	fstream ff;
	ff.open(filename,
		std::fstream::in | std::fstream::binary);
	if (ff.fail()) {
		if (debug) cout << "file_exists(): File does NOT exist: " << filename << endl;
		return false;
	}
	if (debug) cout << "file_exists(): File DOES exist: " << filename << endl;
	ff.close();
	return true;
}

void open_fileRW(fstream& f, const char filename[])
{
	const bool debug = false;
	//openning a nonexistent file for in|out|app causes a fail()
	//  so, if the file does not exist, create it by openning it for
	//  output:
	if (!file_exists(filename)) {
		f.open(filename, std::fstream::out | std::fstream::binary);
		if (f.fail()) {
			cout << "file open (RW) failed: with out|" << filename << "]" << endl;
			throw("file RW failed  ");
		}
		else {
			if (debug) cout << "open_fileRW: file created successfully: " << filename << endl;
		}
	}
	else {
		f.open(filename,
			std::fstream::in | std::fstream::out | std::fstream::binary);
		if (f.fail()) {
			cout << "file open (RW) failed. [" << filename << "]" << endl;
			throw("file failed to open.");
		}
	}
}

void open_fileW(fstream& f, const char filename[])
{
	f.open(filename, std::fstream::out | std::fstream::binary);
	if (f.fail()) {
		cout << "file open failed: " << filename << endl;
		throw("file failed to open.");
	}
}

void save_list(Record list[], int count)
{
	fstream f;
	open_fileW(f, "record_list.bin");
	vector<long> index;
	for (int i = 0; i < count; i++) {
		index.push_back(list[i].write(f)/sizeof(list[0].buffer));
	}
	f.close();
	f = fstream();
	open_fileRW(f, "record_list.bin");
	for (int i = 0; i < index.size(); i++) {
		Record r;
		r.read(f, index[i]);
		cout << r << endl;
	}
	f.close();
}

void print_list(Record list[], int count)
{
	for (int i = 0; i < count; i++) {
		cout << list[i] << endl;
	}
}
