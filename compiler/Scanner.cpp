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

	string arrOperations[] = { "+","-","*","/","<",">","<=",">=","+=","-=","==","=","!="};
	operations_.assign(arrOperations, arrOperations + sizeof(arrOperations)/sizeof(string));

	char arrSeparators[] = 	{',', '(', ')', ';', '[', ']', ':','{','}'};
	separators_.assign(arrSeparators, arrSeparators + sizeof(arrSeparators)/sizeof(char));

	char arrWhitespace[] = {' ', '\t','\n'};
	whitespace_.assign(arrWhitespace, arrWhitespace + sizeof(arrWhitespace)/sizeof(char));

	char arrOperatSymbol[] = {'+', '-', '*', '/', '<', '>', '=', '=', '!', '?', '%'};
	operatSymbol_.insert(arrOperatSymbol, arrOperatSymbol + sizeof(arrOperatSymbol)/sizeof(char));

	curCol_ = -1;
	curRow_ = -1;
	readNext = true;
	curState_ = NONE;
	subCurState_ = NONE;

}


Scanner::~Scanner(void)
{
}

bool Scanner::isReservedWord(string str)
{
	return find(reservedWords_.begin(), reservedWords_.end(), str) != reservedWords_.end();	
}

bool Scanner::isIdentificator(string str)
{
	if (str[0] < 'A' || str[0] > 'z' || isReservedWord(str) || isOperation(str)){
		return false;
	}
	for (int i = 1; i < (int)str.length(); i++)
	{
		if (!((str[i] > 'A' && str[i] < 'z') || (str[i] > '0' && str[i] < '9')))
			return false;
	}
	return true;
}

bool Scanner::isOperation(string str)
{
	return find(operations_.begin(), operations_.end(), str) != operations_.end();	
}

bool Scanner::isChar(string str)
{
	return !(str.length() != 3 || isSeparator(str[1])  || str[0] != '\'' || str[2] != '\'' );
}

bool Scanner::isString(string str)
{
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
	return find(separators_.begin(), separators_.end(), s) != separators_.end();		
}

bool Scanner::isWhitespace(char s)
{
	return find(whitespace_.begin(), whitespace_.end(), s) != whitespace_.end();	
}

bool Scanner::isOperatSymbol(char s)
{
	return find(operatSymbol_.begin(), operatSymbol_.end(), s) != operatSymbol_.end();
}

bool Scanner::isDigit(char s)
{
	return (s > '0' && s < '9');
}

bool Scanner::isLetter(char s)
{
	return  ((s > 'A' && s < 'z') || s == '-' || s == '_' || s == '$');

}

LexemType Scanner::identity()
{
	if (isReservedWord(curLex_)) return ReservedWord;
	else if (isIdentificator(curLex_)) return Identificator;
	else if (isOperation(curLex_)) return Operation;
	else if (isChar(curLex_)) return Char;
	else if (isString(curLex_)) return String;
	else if (isInteger(curLex_)) return Integer;
	else if (isSeparator(curLex_[0])) return Separator;
	else if (isFloat(curLex_)) return Float;
	else return Error;
}

bool Scanner::next()
{
	curState_ = NONE;
	while (!cin.eof())
	{
		if (!readNext) // Для случая когда сепаратор уже считан
		{
			curLex_ = s;
			readNext = true;
			return true;
		}
		s = cin.get();
		if (s == EOF && curState_ != NONE ) 
			return true;
		//if (isSeparator(s) && curState_ != NONE)
		//{
		//	readNext = false;
		//}
		switch (curState_)
		{
		case(NONE):
			if (isWhitespace(s))		 continue;
			else if (isSeparator(s))	 curState_ = IN_SEPARATOR;
			else if (s > '0' && s < '9') curState_ = IN_NUMBER;
			else if (isLetter(s))		 curState_ = IN_WORD;
			else if (s == '"')			 curState_ = IN_STRING;
			else if (isOperatSymbol(s))	 curState_ = IN_OPERATION;
			break;
		case(IN_WORD):
			while (isLetter(s) || isDigit(s))
			{
				curLex_ += s;
				s = cin.get();
			}
			return true;
		case(IN_NUMBER):
			/*while(s != EOF)
			{
				if (isDigit(s)) curLex_ += s;
				if (s)
			}
			if (isSeparator(s))
			{
				curState_ = IN_SEPARATOR;
				readNext = false;
				return true;				
			}
			else if (s == '.')*/
			break;
		case(IN_COMMENT):
			break;
		case(IN_SEPARATOR):
			curLex_ += s;
			return true;
		case(IN_OPERATION):
			break;
		case(IN_STRING):
			char prevSymbol;
			while (s != EOF)
			{
				
				if(s == '"' && prevSymbol != '\\')
				{
					return true;
				}
				curLex_ += s;
				prevSymbol = s;
				s = cin.get();
			}
			break;
		default:
			break;
		}
		curLex_ += s;
	}
	return false;
}



Lexeme Scanner::get()
{
	Lexeme res(getRow(), getCol(), 0, 0,curLex_, identity());
	curLex_.clear();
	prevState_ = curState_;
	return res;

}