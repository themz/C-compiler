#pragma once
#include <vector>
#include <string>

using namespace std;

typedef enum
{
	RESERVEDWORD,
	IDENTIFICATOR,
	INTEGER,
	DOUBLE,
	SEPARATOR,
	OPERATION,
	CHAR,
	STRING,
	START,
	ENDOF,
} LexemType;

typedef enum {
	PLUS, 
	MINUS,
	MULT,
	DIV,
	MOD,
	INC,
	DEC,
	PLUS_ASSIGN,
	MINUS_ASSIGN,
	MULT_ASSIGN,
	DIV_ASSIGN,
	MOD_ASSIGN,
	QUESTION,
	COLON,
	BITWISE_AND,
	BITWISE_OR,
	LOGICAL_AND,
	LOGICAL_OR,
	LOGICAL_NOT,
	BITWISE_XOR,
	AND_ASSIGN,
	OR_ASSIGN,
	BITWISE_XOR_ASSIGN,
	ASSIGN,
	EQUAL,
	NOT_EQUAL,
	GREATER,
	LESS,
	GREATER_OR_EQUAL,
	LESS_OR_EQUAL,
	BITWISE_SHIFT_LEFT,
	BITWISE_SHIFT_RIGHT,
	DOT,
	ARROW,
	BITWISE_SHIFT_LEFT_ASSIGN,
	BITWISE_SHIFT_RIGHT_ASSIGN,
	PARENTHESIS_FRONT,
	PARENTHESIS_BACK,
	BRACKET_FRONT,
	BRACKET_BACK,
	COMMA,
	BITWISE_NOT,
	OPERATIONS_COUNT
} OperationType;

typedef enum {
	T_AUTO, T_BREAK, T_CASE, T_CHAR, T_CONST , T_CONTINUE, T_DEFAULT, T_DO, T_DOUBLE, T_ELSE, T_ENUM, 
	T_EXTERN, T_FLOAT, T_FOR, T_GOTO, T_IF, T_INT, T_LONG, T_REGISTER, T_RETURN, T_SHORT, T_SIGNED, 
	T_SIZEOF, T_STATIC, T_STRUCT, T_SWITCH, T_TYPEDEF, T_UNION, T_UNSIGNED, T_VOID, T_VOLATILE, T_WHILE
} ReservedWordType;

class Lexeme
{
protected:
	const int line, col; 
	const string stringValue;
	const LexemType type;
public:
	virtual string getStringValue(){return stringValue;};
	virtual void print();
	const LexemType getLexType(){ return type;} 
	virtual string getInfo();
	virtual string getValue(){return stringValue;}
	virtual bool operator == (OperationType) const{return false;}
	virtual bool operator != (OperationType) const{return false;}
	bool operator == (LexemType t) const { return type == t;}
	bool operator != (LexemType t) const { return type != t;}
	virtual bool operator == (ReservedWordType t) const{return false;}
	virtual bool operator != (ReservedWordType t) const{return false;}
	Lexeme(Lexeme &l) : line(l.line), col(l.col), stringValue(l.stringValue), type(l.type){};
	Lexeme(int line, int col, const string& strVal, LexemType type) : line(line), col(col), stringValue(strVal),type(type){};
};

class ReservedWordLexeme : public Lexeme
{
private:
	ReservedWordType tVal_;
public:
	bool operator == (ReservedWordType t) const {return tVal_ == t;}
	bool operator != (ReservedWordType t) const {return tVal_ != t;}
	string getValue(){ return stringValue;}
	ReservedWordType getRwType(){return tVal_;};
	ReservedWordLexeme(int line, int col, const string& strVal, LexemType type, ReservedWordType val) : Lexeme(line,col,strVal,type), tVal_(val){};
};

class IdentificatorLexeme : public Lexeme
{
public:
	string getValue(){ return stringValue;}
	IdentificatorLexeme(IdentificatorLexeme &l): Lexeme(l.line,l.col,l.stringValue,l.type){};
	IdentificatorLexeme(int line, int col, const string& strVal, LexemType type) : Lexeme(line,col,strVal,type){};
};

class IntegerLexeme : public Lexeme
{
private:
	int intValue_;
public:
	string getInfo();
	string getValue();
	IntegerLexeme(IntegerLexeme &l): Lexeme(l.line,l.col,l.stringValue,l.type), intValue_(l.intValue_){};
	IntegerLexeme(int line, int col, const string& strVal, LexemType type, int intValue) : Lexeme(line,col,strVal,type), intValue_(intValue){};
};

class DoubleLexeme : public Lexeme
{
private:
	double doubleValue_;
public:
	string getInfo();
	string getValue();
	DoubleLexeme(DoubleLexeme &l): Lexeme(l.line,l.col,l.stringValue,l.type), doubleValue_(l.doubleValue_){};
	DoubleLexeme(int line, int col, const string& strVal, LexemType type, double doubleValue) : Lexeme(line,col,strVal,type), doubleValue_(doubleValue){};
};

class SeparatorLexeme : public Lexeme
{
private:
	char charValue_; 
public:
	string getInfo();
	string getValue();
	SeparatorLexeme(SeparatorLexeme &l): Lexeme(l.line,l.col,l.stringValue,l.type), charValue_(l.charValue_){};
	SeparatorLexeme(int line, int col, const string& strVal, LexemType type, char charValue) : Lexeme(line,col,strVal,type), charValue_(charValue){};
};

class OperationLexeme : public Lexeme
{
private:
	OperationType tVal_;
public:
	string getInfo();
	string getValue();
	OperationType getOpType(){return tVal_;};
	bool operator == (OperationType t) const{return tVal_ == t;};
	bool operator != (OperationType t) const{return tVal_ != t;};
	OperationLexeme(OperationLexeme &l): Lexeme(l.line,l.col,l.stringValue,l.type), tVal_(l.tVal_){};
	OperationLexeme(int line, int col, const string& strVal, LexemType type, OperationType val) : Lexeme(line,col,strVal,type), tVal_(val){};
};


class CharLexeme : public Lexeme
{
private:
	char charValue_;
public:
	string getInfo();
	string getValue();
	CharLexeme(CharLexeme &l): Lexeme(l.line,l.col,l.stringValue,l.type), charValue_(l.charValue_){};
	CharLexeme(int line, int col, const string& strVal, LexemType type, char charValue) : Lexeme(line,col,strVal,type), charValue_(charValue){};
};

class StringLexeme : public Lexeme
{
private:
	string val_;
public:
	string getInfo();
	string getValue();
	StringLexeme(StringLexeme &l): Lexeme(l.line,l.col,l.stringValue,l.type), val_(l.val_){};
	StringLexeme(int line, int col, const string& strVal, LexemType type, const string& val) : Lexeme(line,col,strVal,type), val_(val){};
};

class EofLexeme : public Lexeme
{
private:
public:
	string getInfo();
	string getValue(){ return "EOF\n";}
	EofLexeme(EofLexeme &l): Lexeme(l.line,l.col,l.stringValue,l.type){};
	EofLexeme(int line, int col, const string& strVal, LexemType type) : Lexeme(line,col,strVal,type){};
};


