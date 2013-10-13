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
	char arrSkipSymbols[] = {' ', '\t','\n', EOF};
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
	curLexemType = Error;
	exceptionType_ = noError;
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
	if (!isLetter(str[0]) || isReservedWord(str) || isOperation(str)){
		return false;
	}
	for (int i = 1; i < (int)str.length(); i++)
	{
		if (!(isLetter(str[i]) || isDigit(str[i])))
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
	return 0;
}

bool Scanner::isString(string str)
{
	return (str[0] == '\"' && str[(int)str.length()-1] == '\"' && str.length() > 2)? true : false;	
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
	for (int i = 1; i < (int)str.length(); i++)
	{
		if ((str[i] == '.' && point) || (str[i] == 'e' && e))
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
		if (!isDigit(str[i]) && str[i] != '+' && str[i] != '-')
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
	return (s >= '0' && s <= '9');
}

bool Scanner::isLetter(char s)
{
	return  ((s >= 'A' && s <= 'z') || s == '-' || s == '_' || s == '$');

}

LexemType Scanner::identity()
{
	if (curLexemType != Error)
		return curLexemType;
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
	curLexemType = Error;
	while (!cin.eof())	{
		readNext ?  s = cin.get() : readNext = true;
		if (curState_ != NONE && s == EOF)
			return true;
		prevState_ = curState_;
		switch (curState_)
		{
		case(NONE):
			if (isSkip(s))				     continue;
			else if (isSeparator(s))		 curState_ = IN_SEPARATOR;
			else if (isDigit(s) || s == '.') curState_ = IN_NUMBER;
			else if (s == '/')				 curState_ = SLASH;
			else if (s == '\'')				 curState_ = IN_CHAR;
			else if (isLetter(s))			 curState_ = IN_WORD;
			else if (s == '"')				 curState_ = IN_STRING;
			else if (isOperatSymbol(s))		 curState_ = IN_OPERATION;
			curLex_ += s;
			continue;
		case(IN_WORD):
			if (isLetter(s) || isDigit(s))
			{
				curLex_ += s;
			}
			else
			{
				curState_ = AFTER_WORD;
				readNext = false;
			}
			continue;
		case(IN_SEPARATOR):
			readNext = false;
			return true;
			break;
		case(IN_NUMBER):
			if (isDigit(s))
				curLex_ += s;
			else if ( s == '.')
			{
				curState_ = IN_NUMBER_DECIMAL;
				curLex_ += s;
			}
			else if ( s == 'E' || s == 'e')
			{
				curState_ = IN_NUMBER_DECIMAL_NUM_E;
				curLex_ += s;
			}
			else 
			{
				curState_ = AFTER_INT;
				readNext = false;
			}
			continue;
		case(IN_NUMBER_DECIMAL):
			if (isDigit(s))
			{
				curState_ = IN_NUMBER_DECIMAL_NUM;
				curLex_ += s;
			}
			else
			{
				exceptionType_ = Exception1;
				return false; // выкинуть исключение
			}
			continue;
		case(IN_NUMBER_DECIMAL_NUM):
			if(isDigit(s))
			{
				curLex_ += s;
			}
			else if ( s == 'E' || s == 'e')
			{
				curState_ = IN_NUMBER_DECIMAL_NUM_E;
				curLex_ += s;
			}
			else 
			{
				curState_ = AFTER_FLOAT;
				readNext = false;
			}
			continue;
		case(IN_NUMBER_DECIMAL_NUM_E):
			if(s == '-' || s == '+')
			{
				curLex_ += s;
				curState_ = IN_NUMBER_DECIMAL_E_SIGN;
			}
			else if (isDigit(s))
			{
				curLex_ += s;
				curState_ = IN_NUMBER_DECIMAL_E_SIGN_NUM; 
			}
			else
			{
				exceptionType_ = Exception1;
				return false; //  выкинуть исключение
			}
			continue;
		case(IN_NUMBER_DECIMAL_E_SIGN):
			if (isDigit(s))
			{
				curLex_ += s;
				curState_ = IN_NUMBER_DECIMAL_E_SIGN_NUM;
			}
			else
			{
				exceptionType_ = Exception1;
				return false; // выкинуть исключение
			}
			continue;
		case(IN_NUMBER_DECIMAL_E_SIGN_NUM):
			if (isDigit(s))
			{
				curLex_ += s;
			}
			else
			{
				curState_ = AFTER_FLOAT_WITH_E;
				readNext = false;
			}
			continue;
		case(IN_CHAR):
			curLex_ += s;
			if(s == '\\')
			{
				curLex_ += s;				
				curState_ = IN_CHAR_SLASH;
			}
			else
			{
				curState_ = IN_CHAR_ONE;
			}
			continue;
		case(IN_CHAR_ONE):
			if(s == '\'')
			{
				curLex_ += s;
				curLexemType = Char;
				return true;
			}
			else
			{
				exceptionType_ = Exception1;
				return false; // обработать исключение
			}
		case(IN_CHAR_SLASH):
			return false;
			//curLex_ += s;
			//if(isDigit(s) &&  s < '7')
			//{
			//	curState_ = IN_CHAR_SLASH_DIG; 
			//}
			//else
			//{
			//	curState_ = IN_CHAR_ONE;
			//}
			////if()
		case(IN_STRING):
			curLex_ += s;
			if (s == '\\')
			{
				curState_ = IN_STRING_SLASH; 
			}
			else if ( s == '"')
			{
				curState_ = AFTER_STRING;
			}
			continue;
		case(IN_STRING_SLASH):
			curLex_ += s;
			curState_ = IN_STRING;
			continue;
		case(IN_OPERATION):
			if (isOperation(curLex_ + s))
			{
				curLex_ += s;
			}
			else
			{
				curState_ = AFTER_OPERATION;
				readNext = false;
			}
			continue;
		case(SLASH):
			if (s == '*')
			{
				curState_ = IN_COMMENT_MULTI_LINE;
				curLex_ = "";
			}
			else if (s == '/')
			{
				curState_ = IN_COMMENT_LINE;
				curLex_ = "";
			}
			else
			{
				curState_ = IN_OPERATION;
				readNext = false;
			}
			continue;
		case(IN_COMMENT_LINE):
			if (s == '\n' || s == EOF)
			{
				curLex_ = "";
				curState_ = NONE;
			}
			continue;
		case(IN_COMMENT_MULTI_LINE):
			if (s == '*' )
			{
				curState_ = IN_COMMENT_MULTI_LINE_ASTERISK;
			}
			else
			{
				continue;
			}
			continue;
		case(IN_COMMENT_MULTI_LINE_ASTERISK):
			if ( s == '/')
			{
				curState_ = NONE;				
			}
			else if ( s != '*' )
			{
				curState_ = IN_COMMENT_MULTI_LINE;
			}
			continue;	
		case(AFTER_OPERATION):
			if(isDigit(s) || isLetter(s))
			{
				readNext = false;
				return true;	
			}
		case(AFTER_INT):
		case(AFTER_STRING):
		case(AFTER_FLOAT):
		case(AFTER_FLOAT_WITH_E):
		case(AFTER_WORD):
		if (isSeparator(s))
			{
				curState_ = IN_SEPARATOR;
				readNext = false;
				return true;
			}
			else if (isSkip(s))
				return true;
			else if (isOperatSymbol(s))
			{
				readNext = false;
				return true;
			}
			else
			{
				exceptionType_ = Exception1;
				return false; // выкинуть исключение
			}
			continue;
		default:
			exceptionType_ = Exception1;
			return false; // выкинуть исключение
		}
	}
	return false;
}

Lexeme Scanner::get()
{
	Lexeme res(getRow(), getCol(), 0, 0,curLex_, identity());
	curLex_.clear();
	return res;

}