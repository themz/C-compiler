#pragma once
#include "Scanner.h"
#include <iostream>


Scanner::Scanner(void)
{
	string arrKeywords[] = 
	{
	   "auto", "break", "case", "char", "const", "continue", "default", "do", "double", "else", "enum", 
	   "extern", "float", "for", "goto", "if", "int", "long", "register", "return", "short", "signed", 
	   "sizeof", "static", "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"
	};
	reservedWords_.assign(arrKeywords, arrKeywords + sizeof(arrKeywords)/sizeof(string));

	string arrOperations[] = { "+","-","*","/","<",">","<=",">=",":=","=","!=","AND","XOR"};
	operations_.assign(arrOperations, arrOperations + sizeof(arrOperations)/sizeof(string));

	char arrSeparators[] = 	{',', '(', ')', ';', '[', ']', ':','}','{'};
	separators_.assign(arrSeparators, arrSeparators + sizeof(arrSeparators)/sizeof(char));
	curCol_ = -1;
	curRow_ = -1;
	curState_ = none;
}


Scanner::~Scanner(void)
{
}

bool Scanner::isReservedWord(string str)
{
	return false;
	for (int i = 0; i < (int)reservedWords_.size(); i++)
	{
		if (str == reservedWords_[i])
			return true;
	}
	return false;
}

bool Scanner::isIdentificator(string str)
{
	return false;
	if (str[0] < 'a' || str[0] > 'Z' || isReservedWord(str) || isOperation(str)) 
		return false;
	for (int i = 1; i < (int)str.length(); i++)
	{
		if (str[0] < 'a' || str[0] > 'Z' || !isdigit(str[i]))
			return false;
	}
	return true;
}

bool Scanner::isOperation(string str)
{
	return false;
	for (int i = 0; i < (int)operations_.size(); i++)
	{
		if (str == operations_[i])
			return true;
	}
	return false;
}

bool Scanner::isChar(string str) // говно способ
{
	return false;
	if (isSeparator(str[1]) ||  str.length() != 3 || str[0] != '\'' || str[2] != '\'' )
		return false;
	return true;
}

bool Scanner::isString(string str)
{
	//if (str[0] != '"' || str[str]
	return false;
	
}

bool Scanner::isInteger(string str)
{
	return false;
}

bool Scanner::isFloat(string str)
{
	return false;
}

bool Scanner::isSeparator(char s)
{
	//return false;
	for (int i = 0; i < (int)separators_.size(); i++)
		if (s == separators_[i]) 
			return true; 
	return false;
}

LexemType Scanner::identity()
{
	if (isReservedWord(curLex_)) return ReservedWord;
	else if (isIdentificator(curLex_)) return Identificator;
	else if (isOperation(curLex_)) return Operation;
	else if (isChar(curLex_)) return Char;
	else if (isString(curLex_)) return String;
	else if (isInteger(curLex_)) return Integer;
	else if (isFloat(curLex_)) return Float;
	else return Error;

}

bool Scanner::next()
{
	char s;
	curState_ = none;
	while (!cin.eof())
	{
		s = cin.get();
		if ((s == ' ' || s == '\n' || s == '\t' )&& curState_ == none )
		{
			continue;
		}
		if (!isSeparator(s) && s != ' ' && s != '\n' && s != '\t'  )
		{
			curLex_ += s;
			curState_ = inRead;
		} else {
			return true;
		}
	}
	return false;
}



Lexeme Scanner::get()
{
	Lexeme res(getRow(), getCol(), 0, 0,curLex_, identity());

	//res.type_ = 
	//identify();
	curLex_.clear();
	return res;

}

	
