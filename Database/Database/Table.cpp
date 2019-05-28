#include "Table.h"

Table::Table(const string& tableName)
{
	this->tableName = tableName;
	create();
}


Table::~Table()
{
}
