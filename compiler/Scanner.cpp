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
		"==", "!=", "^", "|", "=", "+=", "-=", "*=", "/=", "%=", "<<=", ">>=", "&=", "^=", "|=",
		"&&", "||"
	};
	char arrSeparators[] = 	{',', '(', ')', ';', '[', ']', ':','{','}'};
	char arrSkipSymbols[] = {' ', '\t','\n', EOF};
	char arrOperatSymbol[] = {'~','|','^','&','+', '-', '*', '/', '<', '>', '=', '=', '!', '?', '%'};
	reservedWords_.assign(arrKeywords, arrKeywords + sizeof(arrKeywords)/sizeof(string));
	separators_.assign(arrSeparators, arrSeparators + sizeof(arrSeparators)/sizeof(char));
	skipSymbols_.assign(arrSkipSymbols, arrSkipSymbols + sizeof(arrSkipSymbols)/sizeof(char));
	operations_.assign(arrOperations, arrOperations + sizeof(arrOperations)/sizeof(string));
	operatSymbol_.insert(arrOperatSymbol, arrOperatSymbol + sizeof(arrOperatSymbol)/sizeof(char));
	curCol_ = 0;
	curLine_ = 1;
	readNext = true;
	getNext = true;
	curState_ = NONE;
	curLexemType = Error;
}

Scanner::~Scanner(void)
{
}

Lexeme* Scanner::getWordLexeme()
{
	if (find(reservedWords_.begin(), reservedWords_.end(), buffer_) != reservedWords_.end())
	{
		return new ReservedWordLexeme (curLine_, curCol_ ,buffer_, ReservedWord);
	}
	else 
		return new IdentificatorLexeme (curLine_, curCol_ ,buffer_, Identificator);
}

Lexeme* Scanner::getIntegerLexeme()
{	
	int val;
	try {
		val = stoi(buffer_);
	} catch (out_of_range&) {
		throw scanner_exception ("Integer is out of range ", curCol_, curLine_);
	}
	return new IntegerLexeme (curLine_,curCol_ ,buffer_,Integer, val);	
}

Lexeme* Scanner::getDoubleLexeme()
{
	double val;
	try {
		val = stod(buffer_);
	} catch (out_of_range&) {
		throw scanner_exception ("Double is out of range ", curCol_, curLine_);
	}
	return new DoubleLexeme  (curLine_,curCol_ ,buffer_, Double, val);
}

Lexeme* Scanner::getOperationLexeme()
{
	return new OperationLexeme (curLine_, curCol_ ,buffer_, Operation);
}

Lexeme*  Scanner::getCharLexeme()
{
	/*char val;
	switch (prevState_)
	{
	case(IN_CHAR):
		val = buffer_[1];
	case(IN_CHAR_SLASH_HEX):

	case(IN_CHAR_SLASH_OCTAL):

	case(IN_CHAR_SLASH_DIG):

	case(IN_CHAR_ONE):

	case(IN_CHAR_SPEC):

	default:
		break;
	}

	int val;
	try {
		val = stoi(buffer_.substr(2,buffer_.length()-1));
	itoa(int value,char *dest,int radix);
	} catch (out_of_range&) {
		throw scanner_exception ("Char is out of range ", curCol_, curLine_);
	}
	if (val > 255)
		throw scanner_exception ("Char is out of range ", curCol_, curLine_);*/
	return new CharLexeme (curLine_, curCol_ ,buffer_, Char, 'a');
}

Lexeme* Scanner::getStringLexeme()
{
	return new StringLexeme (curLine_, curCol_ ,buffer_, String);
}

Lexeme*  Scanner::getSeparatorLexeme()
{
	return new SeparatorLexeme (curLine_, curCol_ ,buffer_, Separator,buffer_.c_str()[0]);
}

bool Scanner::isSeparator(char s)
{	
	return find(separators_.begin(), separators_.end(), s) != separators_.end();		
}

bool Scanner::isOperation(string str)
{
	return find(operations_.begin(), operations_.end(), str) != operations_.end();
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

bool Scanner::isHexLetter(char s)
{
	return (s == 'A' || s == 'a' || s == 'B' || s == 'b' || s == 'C' || s == 'c' || s == 'D' || s == 'd' || s == 'E' || s == 'e' || s == 'F' || s == 'f');
}

bool Scanner::isSpecChar(char s)
{
	return (s == 'n' || s == 'b' || s == 't' || s == 'r' || s == 'f' || s == '\\' || s == '\'');  
}

bool Scanner::isLetter(char s)
{
	return  ((s >= 'A' && s <= 'Z') || (s >= 'a' && s <= 'z') || s == '-' || s == '_' || s == '$');
}

void Scanner::setSymbol()
{
	s = cin.get();
	curCol_ ++;
	if (s == '\t')
	{
		curCol_ += 3;
	}
	if (s == '\n')
	{
		curLine_ ++;
		curCol_ = 1;
	}
	if (s == EOF)
	{
		getNext = false;
		readNext = false;
	}
}

bool Scanner::identityNext()
{
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
		string msg = "";
		switch (prevState_)
		{
			case(IN_INTEGER):
			case(IN_HEX_INTEGER):
			case(IN_OCT_INTEGER):
			case(IN_NULL_NUMBER):
				msg = "Invalid integer";
				break;			
			case(IN_CHAR):
			case(IN_CHAR_SLASH):
			case(IN_CHAR_SLASH_HEX):
			case(IN_CHAR_SLASH_OCTAL):
			case(IN_CHAR_SLASH_DIG):
			case(IN_CHAR_ONE):
			case(IN_CHAR_SPEC):
				msg = "Invalid char";
				break;
			case(IN_OPERATION):
				msg = "Invalid operation";
				break;
			case(IN_DECIMAL):
			case(IN_DECIMAL_E):
			case(IN_DECIMAL_POINT):
			case(IN_DECIMAL_E_SIGN): 
			case(IN_DECIMAL_E_SIGN_NUM):
				msg = "Invalid double";
				break;
			case(IN_STRING):
			case(IN_STRING_SLASH):
				msg = "Invalid string";
				break;
			case(IN_WORD):
				msg = "Invalid identifier";
				break;
			default:
				break;
		}
		scanner_exception exc (msg, curCol_, curLine_);
		throw (exc);
	}

}

bool Scanner::next()
{
	curState_ = NONE;
	curLexemType = Error;
	bool endOfAll = true;
	try{
		while (getNext || endOfAll){
			endOfAll = getNext;
			readNext ?  setSymbol() : readNext = true;
			prevState_ = curState_;
			switch (curState_)
			{
			case(NONE):
				if (isSkip(s))				     continue;
				else if (isSeparator(s))		 curState_ = IN_SEPARATOR;
				else if (s == '0')				 curState_ = IN_NULL_NUMBER;
				else if (isDigit(s))			 curState_ = IN_INTEGER;
				else if (s == '.')				 curState_ = IN_DECIMAL_POINT;
				else if (s == '/')				 curState_ = SLASH;
				else if (s == '\'')				 curState_ = IN_CHAR;
				else if (isOperatSymbol(s))		 curState_ = IN_OPERATION;
				else if (isLetter(s))			 curState_ = IN_WORD;
				else if (s == '"')				 curState_ = IN_STRING;
				buffer_ += s;
				continue;
			case(IN_WORD):
				if (isLetter(s) || isDigit(s))
				{
					buffer_ += s;
				}
				else
				{					
					if (identityNext())
					{
						curLexem = getWordLexeme();
						return true;
					}
				}
				continue;
			case(IN_SEPARATOR):
				curLexem = getSeparatorLexeme();
				return true;
			case(IN_NULL_NUMBER):
				if (isDigit(s))
				{
					buffer_ += s;
					curState_ = IN_OCT_INTEGER;
				}				
				else if(s == 'x' || s == 'X')
				{
					buffer_ += s;
					curState_ = IN_HEX_INTEGER;
				}
				else if (s == '.')
				{
					buffer_ += s;
					curState_ = IN_DECIMAL_POINT;
				}
				else
				{	
					if (identityNext())
					{
						curLexem = getIntegerLexeme();
						return true;
					}
				}
				continue;
			case (IN_HEX_INTEGER):
				if (isDigit(s) || isHexLetter(s))
				{
					buffer_ += s; 
				}
				else
				{
					if (identityNext())
					{
						curLexem = getIntegerLexeme();
						return true;
					}
				}
				continue;
			case (IN_OCT_INTEGER):
				if (isDigit(s))
				{
					if (s < '8')
					{
						buffer_ += s;
					}
					else
					{
						throw scanner_exception ("Invalid digit in octal integer ", curCol_, curLine_);
					}				
				}
				else
				{
					if (identityNext())
					{
						curLexem = getIntegerLexeme();
						return true;
					}
				}
				continue;
			case(IN_INTEGER):
				if (isDigit(s))
					buffer_ += s;
				else if ( s == 'E' || s == 'e')
				{
					curState_ = IN_DECIMAL_E;
					buffer_ += s;
				}
				else if (s == '.')
				{
					buffer_ += s;
					curState_ = IN_DECIMAL_POINT;	
				}
				else 
				{
					if (identityNext())
					{
						curLexem = getIntegerLexeme();
						return true;
					}
				}
				continue;
			case (IN_DECIMAL_POINT):
				if (isDigit(s))
				{
					buffer_ += s;
				}
				else if (s == 'E' || s == 'e')
				{
					buffer_ += s;
					curState_ = IN_DECIMAL_E;
				}
				else 
				{	
				
					if (buffer_.length() > 1 )
					{
						if (identityNext())
						{
							curLexem = getDoubleLexeme();
							return true;
						}
					}
					else
					{
						scanner_exception exc ("Point with out number ", curCol_, curLine_);
						throw (exc);
					}
				}
				continue;
			case(IN_DECIMAL):
				if(isDigit(s))
				{
					buffer_ += s;
				}
				else if ( s == 'E' || s == 'e')
				{
					curState_ = IN_DECIMAL_E;
					buffer_ += s;
				}
				else 
				{
					if (identityNext())
					{
						curLexem = getDoubleLexeme();
						return true;
					}
				}
				continue;
			case(IN_DECIMAL_E):
				if(s == '-' || s == '+')
				{
					buffer_ += s;
					curState_ = IN_DECIMAL_E_SIGN;
				}
				else if (isDigit(s))
				{
					buffer_ += s;
					curState_ = IN_DECIMAL_E_SIGN_NUM; 
				}
				else
				{
					scanner_exception exc ("Exponent without desimal or digits", curCol_, curLine_);
					throw (exc);
				}
				continue;
			case(IN_DECIMAL_E_SIGN):
				if (isDigit(s))
				{
					buffer_ += s;
					curState_ = IN_DECIMAL_E_SIGN_NUM;
				}
				else
				{
					scanner_exception exc ("Exponent without desimal or digits", curCol_, curLine_);
					throw (exc);
				}
				continue;
			case(IN_DECIMAL_E_SIGN_NUM):
				if (isDigit(s))
				{
					buffer_ += s;
				}
				else
				{
					if (identityNext())
					{
						curLexem = getDoubleLexeme();
						return true;
					}
				}
				continue;
			case(IN_CHAR):
				buffer_ += s;
				if(s == '\\')
				{	
					curState_ = IN_CHAR_SLASH;
				}
				else if (s == '\'' )
				{
					throw scanner_exception ("Empty Char", curCol_, curLine_);
					 	
				}
				else				
				{
					curState_ = IN_CHAR_ONE;
				}
				continue;
			case(IN_CHAR_ONE):
				if(s == '\'')
				{
					buffer_ += s;
					setSymbol();
				    if (identityNext())
					{
						curLexem = getCharLexeme();
						return true;
					}
				}
				else
				{
					scanner_exception exc ("Invalid char ", curCol_, curLine_);
					throw (exc);
				}
			case(IN_CHAR_SLASH):
				if (s == 'x')
				{
					buffer_ += s;	
					curState_ = IN_CHAR_SLASH_HEX;
				}
				else if (isDigit(s))
				{
					if (s < '8')
					{
						buffer_ += s;					
						curState_ = IN_CHAR_SLASH_OCTAL;
					}
					else
					{
						scanner_exception exc ("Invalid characters in octal char ", curCol_, curLine_);
						throw (exc);					
					}
				}
				else if (isSpecChar(s))
				{
					buffer_ += s;
					curState_ = IN_CHAR_ONE;
				}
				else 
				{
					scanner_exception exc ("Invalid char ", curCol_, curLine_);
					throw (exc);
				}
				continue;			
			case(IN_CHAR_SLASH_OCTAL):
				if (isDigit(s))
				{
					if (s < '8')
						buffer_ += s;
					else
					{
						scanner_exception exc ("Invalid characters in octal char ", curCol_, curLine_);
						throw (exc);
					}
				}
				else if (s == '\'')
				{
					buffer_ += s;
					curLexemType = Char;
					if (buffer_.length() > 6)
					{
						scanner_exception exc ("Exceed the dimension of the char ", curCol_, curLine_ - buffer_.length());
						throw (exc);
					}
					setSymbol();
					if (identityNext())
					{
						curLexem = getCharLexeme();
						return true;
					}
				}
				continue;
			case(IN_CHAR_SLASH_HEX):
				if (isDigit(s) || isHexLetter(s))
				{
					buffer_ += s;
				}
				else if(s == '\'')
				{
					buffer_ += s;
					setSymbol();
					if (identityNext())
					{
						curLexem = getCharLexeme();
						return true;
					}
				}
				else
				{
					scanner_exception exc ("Invalid characters in hex char ", curCol_, curLine_);
					throw (exc);
				}
				continue;
			case(IN_STRING):
				buffer_ += s;
				if (s == '\\')
				{
					curState_ = IN_STRING_SLASH; 
				}
				else if ( s == EOF)
				{
					scanner_exception exc ("Not a closed string ", curCol_, curLine_);
					throw (exc);
				}
				else if ( s == '"')
				{
					setSymbol();
					if (identityNext())
					{
						curLexem = getStringLexeme();
						return true;
					}
				}
				continue;
			case(IN_STRING_SLASH):
				if ( s == EOF)
				{
					scanner_exception exc ("Not a closed string ", curCol_, curLine_);
					throw (exc);
				}
				buffer_ += s;
				curState_ = IN_STRING;
				continue;
			case(IN_OPERATION):
				if (isOperation(buffer_ + s))
				{
					buffer_ += s;
				}
				else
				{
					if (isDigit(s) || isLetter(s) || s == '.' || identityNext() )
					{
						curLexem = getOperationLexeme();
						return true;
					}
				}
				continue;
 			case(SLASH):
				if (s == '*')
				{
					curState_ = IN_COMMENT_MULTI_LINE;
					buffer_ = "";
				}
				else if (s == '/')
				{
					curState_ = IN_COMMENT_LINE;
					buffer_ = "";
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
					buffer_ = "";
					curState_ = NONE;
				}
				continue;
			case(IN_COMMENT_MULTI_LINE):
				if (s == '*' )
				{
					curState_ = IN_COMMENT_MULTI_LINE_ASTERISK;
				}
				else if(s == EOF )
				{
					scanner_exception exc ("Not a closed multi-line comment", curCol_, curLine_);
					throw (exc);
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
				else if(s == EOF )
				{
					scanner_exception exc ("Not a closed multi-line comment", curCol_, curLine_);
					throw (exc);
				}
				continue;		
			default:
				scanner_exception exc ("Unknown error", curCol_, curLine_);
				throw (exc);
			}
		}
	}	
	catch(scanner_exception exc)
	{
		cout << "Exeption: " << exc.getExceptionMsg() << endl << "row:" << exc.getExRow()<< " " << "col:" << exc.getExCol();
	}
	return false;
}


Lexeme* Scanner::get()
{
	buffer_.clear();
	return curLexem;	
}