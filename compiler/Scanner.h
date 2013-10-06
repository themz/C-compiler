#pragma once
#include <iostream>
#include "Lexeme.h"
#include <string>
#include <vector>
using namespace std;

class Scanner 
{
private:
	vector <string> reservedWords_;
	vector <string> operations_;
	vector <char> separators_;
	enum {
		none,
		inComment,
		inString,
		inRead,
		inOperation
	} typedef State;
	int curRow_,
		curCol_;
	string curLex_;
	State curState_, prevState_;
	bool isIdentificator(string);
    bool isReservedWord(string);
    bool isOperation(string);
    bool isChar(string);
	bool isString(string);
	bool isInteger(string);
	bool isFloat(string);
	bool isSeparator(char);
	LexemType identity();
public:
	bool next();
	int getRow(){return curRow_;};
	int getCol(){return curCol_;};
	Lexeme get();
	Scanner(void);
	~Scanner(void);
};
