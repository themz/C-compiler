#pragma once
#include <exception>
#include <string>

using namespace std;

class scanner_exception : public exception
{
private:
	string exceptionMsg_;
	int exRow_, exCol_;
public:
	string getExceptionMsg(){ return exceptionMsg_;};
	int getExCol(){ return exCol_;}
	int getExRow(){ return exRow_;}
	scanner_exception(string msg, int col, int row):exceptionMsg_(msg), exCol_(col), exRow_(row){}
	scanner_exception(void);
	~scanner_exception(void);
};

