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
	string arrOperations[] = 
	{
		"++", "--", "~", "!", "-", "+", "&", "*", "/", "%", "<<", ">>", "<", ">", "<=", ">=",
		"==", "!=", "^", "|", "=", "+=", "-=", "*=", "/=", "%=", "<<=", ">>=", "&=", "^=", "|="
	};

	char arrSeparators[] = 	{',', '(', ')', ';', '[', ']', ':','{','}'};
	char arrSkipSymbols[] = {' ', '\t','\n'};
	char arrOperatSymbol[] = {'+', '-', '*', '/', '<', '>', '=', '=', '!', '?', '%'};
	reservedWords_.assign(arrKeywords, arrKeywords + sizeof(arrKeywords)/sizeof(string));
	separators_.assign(arrSeparators, arrSeparators + sizeof(arrSeparators)/sizeof(char));
	skipSymbols_.assign(arrSkipSymbols, arrSkipSymbols + sizeof(arrSkipSymbols)/sizeof(char));
	operations_.assign(arrOperations, arrOperations + sizeof(arrOperations)/sizeof(string));
	operatSymbol_.insert(arrOperatSymbol, arrOperatSymbol + sizeof(arrOperatSymbol)/sizeof(char));
	
	curCol_ = 1;
	curRow_ = 1;
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
	for (int i = 0; i < (int)str.length(); i++)
	{
		if (!isDigit(str[i])) 
			return false;
	}
	return true;
}

bool Scanner::isFloat(string str)
{
	bool e = false,
		 point = false;
	if(!isDigit(str[0]))
		return false;
	for (int i = 0; i < (int)str.length(); i++)
	{
		if ((str[i] = '.' && point) || (str[i] == 'e' && e))
			return false;
		if (str[i] == 'e')
		{
			e = true;
			continue;
		}
		if (str[i] == '.')
		{
			point = true;
			continue;
		}
		if (!isDigit(str[i]))
			return false;
	}
	return (e || point) ?  true : false;
}

bool Scanner::isSeparator(char s)
{	
	return find(separators_.begin(), separators_.end(), s) != separators_.end();		
}

bool Scanner::isSkip(char s)
{
	return find(skipSymbols_.begin(), skipSymbols_.end(), s) != skipSymbols_.end();	
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
		s = (readNext) ?  cin.get() : s, readNext = true;
		switch (curState_)
		{
		case(NONE):
			if (isSkip(s))		 continue;
			else if (isSeparator(s))	 curState_ = IN_SEPARATOR;
			else if (s > '0' && s < '9') curState_ = IN_NUMBER;
			else if (isLetter(s))		 curState_ = IN_WORD;
			else if (s == '"')			 curState_ = IN_STRING;
			else if (isOperatSymbol(s))	 curState_ = IN_OPERATION;
			curLex_ += s;
			continue;
		case(IN_WORD):
			while (isLetter(s) || isDigit(s))
			{
				curLex_ += s;
				s = cin.get();
			}
			readNext = false;
			return true;
		case(IN_NUMBER):
			while (isDigit(s))
			{
				curLex_ += s; 
				s = cin.get();
			}
			if (s == '.')
			{
				curLex_ += s;
				curState_ = IN_NUMBER_DECIMAL;

				s = cin.get();
				while (isDigit(s))
				{
					curLex_ += s;
					s = cin.get();
				}
			}
			if ( s == 'e')
			{
				curLex_ += s;
				curState_ = IN_NUMBER_DECIMAL_E;
				s = cin.get(); // безопасный приём
				if (s == '+' || s == '-' )
				{
					curLex_ += s;
					s = cin.get();
				}
				while (isDigit(s))
				{
					curLex_ += s;
					s = cin.get();
					curState_ = IN_NUMBER_DECIMAL_E_SIGN_NUM;
				}
				if (curState_ !=  IN_NUMBER_DECIMAL_E_SIGN_NUM)
					return false; // 					
			} 
			else 
			{
				if(isSeparator(s) || isOperatSymbol(s) || isSkip(s) || s == EOF)
				{	
					readNext = false;
					return true;
				}
				else 
				{
					return false; //вызвать исключение 
				}						
			}			
			break;
		case(IN_COMMENT):
			break;
		case(IN_SEPARATOR):
			readNext = false;
			return true;
		case(IN_OPERATION):
			while (isOperation(curLex_ + s))
			{
				curLex_ += s;
				s = cin.get();
			}
			readNext = false;
			return true;
			break;
		case(IN_STRING):
			/*char prevSymbol;
			while (s != EOF)
			{
				
				if(s == '"' && prevSymbol != '\\')
				{
					return true;
				}
				curLex_ += s;
				prevSymbol = s;
				s = cin.get();
			}*/
			break;
		default:
			break;
		}
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