#pragma once
#include <iostream>
#include "Lexeme.h"
#include "exception.h"
#include <string>
#include <vector>
#include <set>
#include <map>
using namespace std;

class Scanner 
{
private:
	vector <string> reservedWords_;
	vector <string> operations_;
	vector <char> separators_;
	vector <char> skipSymbols_;
	set <char> operatSymbol_;
	map <char, char> specChar; 
	enum {		
		IN_WORD,

		IN_COMMENT,
		IN_COMMENT_LINE,
		IN_COMMENT_MULTI_LINE,
		IN_COMMENT_MULTI_LINE_ASTERISK,

		IN_STRING,
		IN_STRING_SLASH,

		IN_OPERATION,

		IN_CHAR,
		IN_CHAR_SLASH,
		IN_CHAR_SLASH_HEX,
		IN_CHAR_SLASH_OCTAL,
		IN_CHAR_SLASH_DIG,
		IN_CHAR_ONE,
		IN_CHAR_SPEC,

		IN_NULL_NUMBER,
		IN_INTEGER,
		IN_DECIMAL,
		IN_DECIMAL_E,
		IN_DECIMAL_POINT,
		IN_DECIMAL_E_SIGN, 
		IN_DECIMAL_E_SIGN_NUM,
		IN_HEX_INTEGER,
		IN_OCT_INTEGER,

		IN_SEPARATOR,		

		NONE,
		SLASH,
	} typedef State;
	int curLine_,curCol_,prevLine_,prevCol_;
	string buffer_;
	char s;
	State curState_, prevState_ ;
	bool readNext, getNext,chLine;
	Lexeme* curLexem;

	bool isIdentificator(string);
    bool isReservedWord(string);
    bool isOperation(string);
    bool isChar(string);
	bool isString(string);
	bool isInteger(string);
	bool isDouble(string);
	bool isSeparator(char);
	bool isSkip(char);
	bool isLetter(char);
	bool isDigit(char);
	bool isHexLetter(char);
	bool isSpecChar(char);
	bool isOperatSymbol(char);
	void setSymbol();
	bool identityNext();
	Lexeme* getWordLexeme();
	Lexeme* getIntegerLexeme();
	Lexeme* getDoubleLexeme();
	Lexeme* getSeparatorLexeme();
	Lexeme* getOperationLexeme();
	Lexeme* getCharLexeme(State state = IN_CHAR_ONE);
	Lexeme* getStringLexeme();
	int getLine(){return (chLine)? prevLine_ : curLine_;};
	int getCol(){return (chLine)? prevCol_ : curCol_;};
	char getSpecChar(string);
	char getSpecChar(char);
public:
	bool next();
	Lexeme* get();
	Scanner(void);
	~Scanner(void);
};
