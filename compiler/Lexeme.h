#pragma once
#include <vector>
#include <string>

using namespace std;


typedef enum
{
	ReservedWord,
	Identificator,
	Integer,
	Double,
	Separator,
	Operation,
	Char,
	String,
	Error,
} LexemType;

class Lexeme
{
protected:
	const int line, col; 
	const string stringValue;
	const LexemType type;
public:
	virtual string getStringValue(){return stringValue;};
	virtual void print();
	virtual string getInfo();
	Lexeme(int line, int col, const string& strVal, LexemType type) : line(line), col(col), stringValue(strVal),type(type){};
};

class ReservedWordLexeme : public Lexeme
{
public:
	ReservedWordLexeme(int line, int col, const string& strVal, LexemType type) : Lexeme(line,col,strVal,type){};
};

class IdentificatorLexeme : public Lexeme
{
public:
	IdentificatorLexeme(int line, int col, const string& strVal, LexemType type) : Lexeme(line,col,strVal,type){};
};

class IntegerLexeme : public Lexeme
{
private:
	int intValue_;
public:
	string getInfo();
	IntegerLexeme(int line, int col, const string& strVal, LexemType type, int intValue) : Lexeme(line,col,strVal,type), intValue_(intValue){};
};

class DoubleLexeme : public Lexeme
{
private:
	double doubleValue_;
public:
	string getInfo();
	DoubleLexeme(int line, int col, const string& strVal, LexemType type, double doubleValue) : Lexeme(line,col,strVal,type), doubleValue_(doubleValue){};
};

class SeparatorLexeme : public Lexeme
{
private:
	char charValue_; 
public:
	string getInfo();
	SeparatorLexeme(int line, int col, const string& strVal, LexemType type, char charValue) : Lexeme(line,col,strVal,type), charValue_(charValue){};
};

class OperationLexeme : public Lexeme
{
public:
	string getInfo();
	OperationLexeme(int line, int col, const string& strVal, LexemType type) : Lexeme(line,col,strVal,type){};
};


class CharLexeme : public Lexeme
{
private:
	char charValue_;
public:
	string getInfo();
	CharLexeme(int line, int col, const string& strVal, LexemType type, char charValue) : Lexeme(line,col,strVal,type), charValue_(charValue){};
};

class StringLexeme : public Lexeme
{
public:
	string getInfo();
	StringLexeme(int line, int col, const string& strVal, LexemType type) : Lexeme(line,col,strVal,type){};
};

