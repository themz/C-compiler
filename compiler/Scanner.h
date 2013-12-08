#pragma once
#include <iostream>
#include <fstream>
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
	map <string, ReservedWordType> reservedWords_;
	map <string, OperationType> operations_;
	vector <char> separators_;
	vector <char> skipSymbols_;
	set <char> operatSymbol_;
	map <char, char> specChar_; 
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
	} typedef State_;
	int curLine_,curCol_,prevLine_,prevCol_;
	ifstream input_;	
	string buffer_, filename_;
	char s_;
	State_ curState_;
	bool readNext_, getNext_,chLine_;
	Lexeme* curLexem_;

    bool isOperation(string);
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
	Lexeme* getCharLexeme(State_ state = IN_CHAR_ONE);
	Lexeme* getStringLexeme();
	Lexeme* getEofLexeme();
	char getSpecChar(char);
public:
	string getFileName(){return filename_;};
	bool next();
	int getLine(){return (chLine_)? prevLine_ : curLine_;};
	int getCol(){return (chLine_)? prevCol_ : curCol_;};
	Lexeme* get();
	Lexeme* getNextLex(bool need = true);
	Scanner(string filename);
	Scanner(Scanner& c): input_(c.filename_), filename_(c.filename_), curCol_(0), curLine_(1), 
		prevCol_(0), prevLine_(1),readNext_(true), getNext_(true), curState_(NONE), curLexem_(NULL), chLine_(false), specChar_(c.specChar_),
		operations_(c.operations_), reservedWords_(c.reservedWords_), separators_(c.separators_), skipSymbols_(c.skipSymbols_), operatSymbol_(c.operatSymbol_){}
};
