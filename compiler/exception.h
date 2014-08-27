#pragma once
#include <exception>
#include <string>

using namespace std;

class compiler_exception : public exception
{
private:
	string exceptionMsg_;
	int exLine_, exCol_;
	bool printingPos;
public:
	string getExceptionMsg(){ return exceptionMsg_; };
	int getExCol(){ return exCol_;}
	int getExRow(){ return exLine_;}
	bool getPrintingPos(){ return printingPos;}
	compiler_exception(string msg, int col, int row, bool prPos = true):exceptionMsg_(msg), exCol_(col), exLine_(row), printingPos(prPos){};
};

class scanner_exception : public compiler_exception
{
public:
	scanner_exception(string msg, int col, int line, bool prPos = true) : compiler_exception(msg, col, line, prPos){};
};

class parser_exception : public compiler_exception
{
	bool printPos;
public:
	parser_exception(string msg, int col, int line, bool prPos = true) : compiler_exception(msg, col, line, prPos){};
	parser_exception(string msg, bool prPos = true ) : compiler_exception(msg, 0, 0,prPos){}
};


