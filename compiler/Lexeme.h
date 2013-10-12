#pragma once
#include <vector>
#include <string>

using namespace std;


enum LexemType
{
	ReservedWord,
	Identificator,
	Integer,
	Float,
	Separator,
	Operation,
	Char,
	String,
	Error,
};


class Lexeme
{
private:
	int row_,col_;
	double floatValue_;
	int intValue_;
	string stringValue_;		
	LexemType type_;
public:
	LexemType getType(){return type_;};
	string getStringValue(){return stringValue_;};
	void print();
	Lexeme(void);
	Lexeme(int row, int col, double floatVal, int intVal, string strVal, LexemType type):row_(row), col_(col), floatValue_(floatVal), intValue_(intVal), stringValue_(strVal), type_(type){} 
	~Lexeme(void);
};



