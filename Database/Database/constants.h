#ifndef CONSTANTS_H
#define CONSTANTS_H
#define MAX_ROWS 35
#define MAX_COLUMNS 256
#define F_MAX_ROWS 20
#define F_MAX_COLUMNS 20
#define MAX_BUFFER 100
#define R_MAX_ROW 10
#define R_MAX_COL 50
#define SETW 15
enum START_STATES {
	ALPHA = 0,
	NUMBER = 7,
	SPACE = 15,
	COMMA = 17,
	EQUAL = 19,
	LESS = 21,
	GREATER = 23,
	TABS = 25,
	QUOTES_ = 27,
	POINTER = MAX_ROWS-1,
	FPOINTER = F_MAX_ROWS-1
};
enum Keyword
{
	ZERO,
	SELECT,
	STAR,
	FROM,
	SYMOBL,
	COMMA_K,
	WHERE,
	RELATIONAL_OPERATOR,
	LOGICAL_OPERATOR,
	QUOTES,
	INSERT,
	INTO,
	VALUES,
	CREATE,
	MAKE = CREATE,
	TABLE,
	FIELDS
};
#endif //!CONSTANTS_H