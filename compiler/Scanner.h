#pragma once
#include <iostream>
#include "Lexeme.h"
#include <string>
#include <vector>
#include <set>
using namespace std;

class Scanner 
{
private:
	vector <string> reservedWords_;
	vector <string> operations_;
	vector <char> separators_;
	vector <char> skipSymbols_;
	set <char> operatSymbol_;

	enum {
		NONE,
		IN_COMMENT,
		IN_STRING,
		IN_WORD,
		IN_OPERATION,
		IN_NUMBER,
		IN_NUMBER_DECIMAL,
		IN_NUMBER_DECIMAL_E,
		IN_NUMBER_DECIMAL_E_SIGN,
		IN_NUMBER_DECIMAL_E_SIGN_NUM,
		IN_SEPARATOR,
		IN_DECIMAL,
		IN_HEX,
		IN_INTEGER,
		IN_EXP,

	} typedef State;
	int curRow_,
		curCol_;
	string curLex_;
	char s;
	State curState_, prevState_ ,subCurState_;
	bool readNext;

	bool isIdentificator(string);
    bool isReservedWord(string);
    bool isOperation(string);
    bool isChar(string);
	bool isString(string);
	bool isInteger(string);
	bool isFloat(string);

	bool isSeparator(char);
	bool isSkip(char);
	bool isLetter(char);
	bool isDigit(char);
	bool isOperatSymbol(char);

	LexemType identity();
public:
	bool next();
	int getRow(){return curRow_;};
	int getCol(){return curCol_;};
	Lexeme get();
	Scanner(void);
	~Scanner(void);
};
