#include "Lexeme.h"
#include <iostream>

char* typeDef[] = {
	(char*)"ReservedWord",
	(char*)"Identificator",
	(char*)"Integer",
	(char*)"Float",
	(char*)"Separator",
	(char*)"Operation",
	(char*)"Char",
	(char*)"String",
	(char*)"Error"
};

string Lexeme::getInfo()
{
    char * info = (char*) calloc(sizeof(char), 300);
	sprintf(info, "%s\t\t%d\t\t%d\t\t%s\t\t", typeDef[type], line, col, stringValue.c_str());
	return string(info);
}

void Lexeme::print()
{
	cout << getInfo() << endl;
}

string IntegerLexeme::getInfo()
{
	return Lexeme::getInfo() + string(getValue());
}

string IntegerLexeme::getValue()
{
	char * info = (char*) malloc(sizeof(char) * 100);
	sprintf(info, "%d", intValue_);
	return string(info);
} 


string FloatLexeme::getInfo()
{

	return Lexeme::getInfo() + string(getValue());
}

string FloatLexeme::getValue()
{
	char * info = (char*) malloc(sizeof(char) * 100);
	sprintf(info, "%f", floatValue_);
	return string(info);
}

string OperationLexeme::getInfo()
{
	return Lexeme::getInfo() + stringValue;
}
string OperationLexeme::getValue()
{
	return stringValue;
}

string SeparatorLexeme::getInfo()
{
	return Lexeme::getInfo() + charValue_;
}

string SeparatorLexeme::getValue()
{
	return string(charValue_ + string(" "));
}


string CharLexeme::getInfo()
{
	return Lexeme::getInfo() + charValue_;
}

string CharLexeme::getValue()
{
	return string(charValue_ + string(" "));
}

string StringLexeme::getInfo()
{
	return Lexeme::getInfo() + val_;	
}

string StringLexeme::getValue()
{
	return val_;
}

string EofLexeme::getInfo(){
	return getInfo() + "EOF";
}







