#pragma once
#include <iostream>
#include "Lexeme.h"
#include "Exception.h"
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
		// identificator and reserved word
		IN_WORD,
		AFTER_WORD,
		// comment
		IN_COMMENT,
		IN_COMMENT_LINE,
		IN_COMMENT_MULTI_LINE,
		IN_COMMENT_MULTI_LINE_ASTERISK,
		// string
		IN_STRING,
		IN_STRING_SLASH,
		AFTER_STRING,
		// operation
		IN_OPERATION,
		AFTER_OPERATION,
		// char
		IN_CHAR,
		IN_CHAR_SLASH,
		IN_CHAR_SLASH_DIG,
		IN_CHAR_ONE,		
		// number
		IN_NUMBER, 
		AFTER_INT, 
		IN_NUMBER_DECIMAL, 
		IN_NUMBER_DECIMAL_NUM, 
		IN_NUMBER_DECIMAL_NUM_E,
		IN_NUMBER_DECIMAL_E_SIGN, 
		IN_NUMBER_DECIMAL_E_SIGN_NUM,
		AFTER_FLOAT,
		AFTER_FLOAT_WITH_E,
		IN_DECIMAL,
		IN_HEX,
		IN_INTEGER,
		IN_EXP,
		// separator
		IN_SEPARATOR,
		// other
		NONE,
		ERROR,
		SLASH,
	} typedef State;
	int curRow_,
		curCol_;
	string curLex_;
	char s;
	State curState_, prevState_ ;
	bool readNext;
	LexemType curLexemType;
	ExceptionType exceptionType_;
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
	ExceptionType getException(){ return exceptionType_;};
	int getRow(){return curRow_;};
	int getCol(){return curCol_;};
	Lexeme get();
	Scanner(void);
	~Scanner(void);
};
