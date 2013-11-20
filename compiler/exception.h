#pragma once
#include <exception>
#include <string>

using namespace std;

class compiler_exception : public exception
{
private:
	string exceptionMsg_;
	int exLine_, exCol_;
public:
	string getExceptionMsg(){ return exceptionMsg_; };
	int getExCol(){ return exCol_;}
	int getExRow(){ return exLine_;}
	compiler_exception(string msg, int col, int row):exceptionMsg_(msg), exCol_(col), exLine_(row){};
};

class scanner_exception : public compiler_exception
{
public:
	scanner_exception(string msg, int col, int line) : compiler_exception(msg, col, line){};
};

class parser_exception : public compiler_exception
{
public:
	parser_exception(string msg, int col, int line) : compiler_exception(msg, col, line){};
};


