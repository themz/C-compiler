#pragma once
#include "Lexeme.h"
#include <iostream>

char* typeDef[] = {
	"ReservedWord",
	"Identificator",
	"Integer",
	"Double",
	"Separator",
	"Operation",
	"Char",
	"String",
	"Error"
};

string Lexeme::getInfo()
{	
	char * info = (char*) malloc(sizeof(char) * 300);
	sprintf_s(info, 300, "%s\t\t%d\t\t%d\t\t%s\t\t", typeDef[type], line, col, stringValue.c_str());
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
	sprintf_s(info, sizeof(char) * 100, "%d", intValue_);
	return string(info);
} 


string DoubleLexeme::getInfo()
{

	return Lexeme::getInfo() + string(getValue());
}

string DoubleLexeme::getValue()
{
	char * info = (char*) malloc(sizeof(char) * 100);
	sprintf_s(info, sizeof(char) * 100, "%f", doubleValue_);
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
	return string(charValue_ + " ");
	//return "Ebanblu` separator";
}


string CharLexeme::getInfo()
{
	return Lexeme::getInfo() + charValue_;
}

string CharLexeme::getValue()
{
	return string(charValue_ + " ");
	//return "Ebanblu` char";
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







