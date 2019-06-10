#include <iostream>
#include "SQL.h"
#include "random.h"
using namespace std;

int main() {
	SQL sql;
	sql.run("inputs.txt");
	return 0;
}