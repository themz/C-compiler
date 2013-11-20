#pragma once
#include "Scanner.h"
#include <iostream>

Scanner::Scanner(string filename):input_(filename), filename_(filename), curCol_(0), curLine_(1), 
		prevCol_(0), prevLine_(1),readNext_(true), getNext_(true), curState_(NONE), curLexem_(NULL), chLine_(false)
{
	if (!input_)
		throw exception("Invalid filename");

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
		"&&", "||", "?","->"
	};
	char arrSeparators[] = 	{',', '(', ')', ';', '[', ']', ':','{','}'};
	char arrSkipSymbols[] = {' ', '\t','\n', EOF};
	char arrOperatSymbol[] = {'~','|','^','&','+', '-', '*', '/', '<', '>', '=', '=', '!', '?', '%'};
	reservedWords_.assign(arrKeywords, arrKeywords + sizeof(arrKeywords)/sizeof(string));
	separators_.assign(arrSeparators, arrSeparators + sizeof(arrSeparators)/sizeof(char));
	skipSymbols_.assign(arrSkipSymbols, arrSkipSymbols + sizeof(arrSkipSymbols)/sizeof(char));
	operations_.assign(arrOperations, arrOperations + sizeof(arrOperations)/sizeof(string));
	operatSymbol_.insert(arrOperatSymbol, arrOperatSymbol + sizeof(arrOperatSymbol)/sizeof(char));
	specChar_.insert (pair<char,char>('n','\n'));
    specChar_.insert (pair<char,char>('N','\n'));
    specChar_.insert (pair<char,char>('t','\t'));
    specChar_.insert (pair<char,char>('T','\t'));
    specChar_.insert (pair<char,char>('b','\b'));
    specChar_.insert (pair<char,char>('B','\b'));
    specChar_.insert (pair<char,char>('r','\r'));
    specChar_.insert (pair<char,char>('R','\r'));
    specChar_.insert (pair<char,char>('f','\f'));
    specChar_.insert (pair<char,char>('F','\f'));
    specChar_.insert (pair<char,char>('\\','\\'));
    specChar_.insert (pair<char,char>('\'','\''));
}


Lexeme* Scanner::getWordLexeme()
{
	if (find(reservedWords_.begin(), reservedWords_.end(), buffer_) != reservedWords_.end())
	{
		return new ReservedWordLexeme (getLine(), getCol() - buffer_.length(), buffer_, ReservedWord);
	}
	else 
		return new IdentificatorLexeme (getLine(), getCol() - buffer_.length(), buffer_, Identificator);
}

Lexeme* Scanner::getIntegerLexeme()
{	
	int val = 0;
	try {
		val = stoi(buffer_,nullptr,0);			
	} catch (out_of_range&) {
		throw scanner_exception ("Integer is out of range ", getCol(), getLine());
	}
	return new IntegerLexeme (getLine(), getCol() - buffer_.length(), buffer_, Integer, val);	
}

Lexeme* Scanner::getDoubleLexeme()
{
	double val;
	try {
		val = stod(buffer_);
	} catch (out_of_range&) {
		throw scanner_exception ("Double is out of range ", curCol_, getLine());
	}
	return new DoubleLexeme  (getLine(), curCol_ - buffer_.length() ,buffer_, Double, val);
}

Lexeme* Scanner::getOperationLexeme()
{
	return new OperationLexeme (getLine(), getCol() - buffer_.length(), buffer_, Operation);
}

Lexeme*  Scanner::getCharLexeme(State_ state)
{
	int val = 0;
	try {
		switch (state)
		{
		case(IN_CHAR_ONE):
			val = (int)buffer_[1];
			break;
		case(IN_CHAR_SLASH_HEX):
		case(IN_CHAR_SLASH_OCTAL):
			val = stoi( "0" + buffer_.substr(2,buffer_.length() - 3),nullptr,0);
			break;
		case(IN_CHAR_SPEC):
			val = (int)getSpecChar(buffer_[2]);
			break;
		}		
	} catch (out_of_range&) {
		throw scanner_exception ("Char is out of range ", curCol_, curLine_);
	}
	if (val > 255)
		throw scanner_exception ("Char is out of range ", curCol_, curLine_);
	return new CharLexeme (getLine(), getCol() - buffer_.length() ,buffer_, Char, (char)val);
}

Lexeme* Scanner::getStringLexeme()
{
	string str = "";
	for (unsigned int i = 1; i < buffer_.length() - 1; i++)
	{
		if (buffer_[i - 1] == '\\')
		{
			str[str.length() - 1] = getSpecChar(buffer_[i]);
		}
		else
		{ 
			str += buffer_[i];
		}
	}	
	return new StringLexeme (getLine(), getCol() - buffer_.length(), buffer_, String, str);
}

Lexeme*  Scanner::getSeparatorLexeme()
{
	return new SeparatorLexeme (getLine(), getCol() - 1, buffer_, Separator,buffer_.c_str()[0]);
}

char Scanner::getSpecChar(char c){
	return specChar_[c];
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
	return (specChar_.find(s) != specChar_.end());  
}

bool Scanner::isLetter(char s)
{
	return  ((s >= 'A' && s <= 'Z') || (s >= 'a' && s <= 'z') || s == '-' || s == '_' || s == '$');
}

void Scanner::setSymbol()
{
	chLine_ = false;
	s_ = input_.get();
	curCol_++;
	if (s_ == '\t')
	{
		curCol_ += 3;
	}
	else if (s_ == '\n')
	{
		chLine_ = true;
		prevLine_ = curLine_;
		curLine_++;
		prevCol_ = curCol_;
		curCol_ = 0;
	}
	else if (s_ == EOF)
	{
		getNext_ = false;
	}
}

bool Scanner::identityNext()
{
	if (isSeparator(s_))
	{
		curState_ = IN_SEPARATOR;
		readNext_ = false;
		return true;
	}
	else if (isSkip(s_))
		return true;
	else if (isOperatSymbol(s_))
	{
		readNext_ = false;
		return true;
	}
	else
	{
		string msg = "";
		switch (curState_)
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
		throw scanner_exception (msg, getCol(), getLine());		
	}
}

bool Scanner::next()
{
	curState_ = NONE;
	while (getNext_){
		readNext_ ?  setSymbol() : readNext_ = true;
		switch (curState_)
		{
		case(NONE):
			if (isSkip(s_))				     continue;
			else if (isSeparator(s_))		 curState_ = IN_SEPARATOR;
			else if (s_ == '0')				 curState_ = IN_NULL_NUMBER;
			else if (isDigit(s_))			 curState_ = IN_INTEGER;
			else if (s_ == '.')				 curState_ = IN_DECIMAL_POINT;
			else if (s_ == '/')				 curState_ = SLASH;
			else if (s_ == '\'')			 curState_ = IN_CHAR;
			else if (isOperatSymbol(s_))	 curState_ = IN_OPERATION;
			else if (isLetter(s_))			 curState_ = IN_WORD;
			else if (s_ == '"')				 curState_ = IN_STRING;
			buffer_ += s_;
			continue;
		case(IN_WORD):
			if (isLetter(s_) || isDigit(s_))
			{
				buffer_ += s_;
			}
			else
			{					
				if (identityNext())
				{
					curLexem_ = getWordLexeme();
					return true;
				}
			}
			continue;
		case(IN_SEPARATOR):
			readNext_ = false;
			curLexem_ = getSeparatorLexeme();
			return true;
		case(IN_NULL_NUMBER):
			if (isDigit(s_))
			{
				buffer_ += s_;
				curState_ = IN_OCT_INTEGER;
			}				
			else if(s_ == 'x' || s_ == 'X')
			{
				buffer_ += s_;
				curState_ = IN_HEX_INTEGER;
			}
			else if (s_ == '.')
			{
				buffer_ += s_;
				curState_ = IN_DECIMAL_POINT;
			}
			else
			{	
				if (identityNext())
				{
					curLexem_ = getIntegerLexeme();
					return true;
				}
			}
			continue;
		case (IN_HEX_INTEGER):
			if (isDigit(s_) || isHexLetter(s_))
			{
				buffer_ += s_; 
			}
			else
			{
				if (identityNext())
				{
					curLexem_ = getIntegerLexeme();
					return true;
				}
			}
			continue;
		case (IN_OCT_INTEGER):
			if (isDigit(s_))
			{
				if (s_ < '8')
				{
					buffer_ += s_;
				}
				else
				{
					throw scanner_exception ("Invalid digit in octal integer ", getCol(), getLine());
				}				
			}
			else
			{
				if (identityNext())
				{
					curLexem_ = getIntegerLexeme();
					return true;
				}
			}
			continue;
		case(IN_INTEGER):
			if (isDigit(s_))
				buffer_ += s_;
			else if ( s_ == 'E' || s_ == 'e')
			{
				curState_ = IN_DECIMAL_E;
				buffer_ += s_;
			}
			else if (s_ == '.')
			{
				buffer_ += s_;
				curState_ = IN_DECIMAL_POINT;	
			}
			else 
			{
				if (identityNext())
				{
					curLexem_ = getIntegerLexeme();
					return true;
				}
			}
			continue;
		case (IN_DECIMAL_POINT):
			if (isDigit(s_))
			{
				buffer_ += s_;
			}
			else if (s_ == 'E' || s_ == 'e')
			{
				buffer_ += s_;
				curState_ = IN_DECIMAL_E;
			}
			else 
			{	
				
				if (buffer_.length() > 1 )
				{
					if (identityNext())
					{
						curLexem_ = getDoubleLexeme();
						return true;
					}
				}
				else
				{
					throw scanner_exception ("Point with out number ", getCol(), getLine());
				}
			}
			continue;
		case(IN_DECIMAL):
			if(isDigit(s_))
			{
				buffer_ += s_;
			}
			else if ( s_ == 'E' || s_ == 'e')
			{
				curState_ = IN_DECIMAL_E;
				buffer_ += s_;
			}
			else 
			{
				if (identityNext())
				{
					curLexem_ = getDoubleLexeme();
					return true;
				}
			}
			continue;
		case(IN_DECIMAL_E):
			if(s_ == '-' || s_ == '+')
			{
				buffer_ += s_;
				curState_ = IN_DECIMAL_E_SIGN;
			}
			else if (isDigit(s_))
			{
				buffer_ += s_;
				curState_ = IN_DECIMAL_E_SIGN_NUM; 
			}
			else
			{
				throw scanner_exception ("Exponent without desimal or digits", getCol(), getLine());
			}
			continue;
		case(IN_DECIMAL_E_SIGN):
			if (isDigit(s_))
			{
				buffer_ += s_;
				curState_ = IN_DECIMAL_E_SIGN_NUM;
			}
			else
			{
				throw scanner_exception ("Exponent without desimal or digits", getCol(), getLine());
			}
			continue;
		case(IN_DECIMAL_E_SIGN_NUM):
			if (isDigit(s_))
			{
				buffer_ += s_;
			}
			else
			{
				if (identityNext())
				{
					curLexem_ = getDoubleLexeme();
					return true;
				}
			}
			continue;
		case(IN_CHAR):
			buffer_ += s_;
			if(s_ == '\\')
			{	
				curState_ = IN_CHAR_SLASH;
			}
			else if (s_ == '\'' )
			{
				throw scanner_exception ("Empty Char", getCol(), getLine());
					 	
			}
			else				
			{
				curState_ = IN_CHAR_ONE;
			}
			continue;
		case(IN_CHAR_ONE):
			if(s_ == '\'')
			{
				buffer_ += s_;
				setSymbol();
				if (identityNext())
				{
					curLexem_ = getCharLexeme( buffer_[1] == '\\' ?  IN_CHAR_SPEC : IN_CHAR_ONE);
					return true;
				}
			}
			else
			{
				throw scanner_exception ("Invalid char ", getCol(), getLine());
			}
			continue;
		case(IN_CHAR_SLASH):
			if (s_ == 'x' || s_ == 'X')
			{
				buffer_ += s_;	
				curState_ = IN_CHAR_SLASH_HEX;
			}
			else if (isDigit(s_))
			{
				if (s_ < '8')
				{
					buffer_ += s_;					
					curState_ = IN_CHAR_SLASH_OCTAL;
				}
				else
				{
					throw scanner_exception ("Invalid characters in octal char ", getCol(), getLine());
				}
			}
			else if (isSpecChar(s_))
			{
				buffer_ += s_;
				curState_ = IN_CHAR_ONE;
			}
			else 
			{
				throw scanner_exception ("Invalid char ", getCol(), getLine());
			}
			continue;			
		case(IN_CHAR_SLASH_OCTAL):
			if (isDigit(s_))
			{
				if (s_ < '8')
					buffer_ += s_;
				else
				{
					throw scanner_exception ("Invalid characters in octal char ", getCol(), getLine());
				}
			}
			else if (s_ == '\'')
			{
				buffer_ += s_;
				if (buffer_.length() > 6)
				{
					throw scanner_exception ("Exceed the dimension of the char ", getCol(), getLine() - buffer_.length());
				}
				setSymbol();
				if (identityNext())
				{
					curLexem_ = getCharLexeme(IN_CHAR_SLASH_OCTAL);
					return true;
				}
			}
			continue;
		case(IN_CHAR_SLASH_HEX):
			if (isDigit(s_) || isHexLetter(s_))
			{
				buffer_ += s_;
			}
			else if(s_ == '\'')
			{
				buffer_ += s_;
				setSymbol();
				if (identityNext())
				{
					curLexem_ = getCharLexeme(IN_CHAR_SLASH_HEX);
					return true;
				}
			}
			else
			{
				throw scanner_exception ("Invalid characters in hex char ", getCol(), getLine());
			}
			continue;
		case(IN_STRING):
			buffer_ += s_;
			if (s_ == '\\')
			{
				curState_ = IN_STRING_SLASH; 
			}
			else if (s_ == EOF)
			{
				throw scanner_exception ("Not a closed string ", getCol(), getLine());
			}
			else if (s_ == '"')
			{
				setSymbol();
				if (identityNext())
				{
					curLexem_ = getStringLexeme();
					return true;
				}
			}
			continue;
		case(IN_STRING_SLASH):
			if (s_ == EOF)
			{
				throw scanner_exception ("Not a closed string ", getCol(), getLine());
			}
			buffer_ += s_;
			curState_ = IN_STRING;
			continue;
		case(IN_OPERATION):
			if (isOperation(buffer_ + s_))
			{
				buffer_ += s_;
			}
			else
			{
				if (isDigit(s_) || isLetter(s_) || s_ == '.' || identityNext())
				{
					readNext_ = false;
					curLexem_ = getOperationLexeme();
					return true;
				}
			}
			continue;
 		case(SLASH):
			if (s_ == '*')
			{
				curState_ = IN_COMMENT_MULTI_LINE;
				buffer_ = "";
			}
			else if (s_ == '/')
			{
				curState_ = IN_COMMENT_LINE;
				buffer_ = "";
			}
			else
			{
				curState_ = IN_OPERATION;
				readNext_ = false;
			}
			continue;
		case(IN_COMMENT_LINE):
			if (s_ == '\n' || s_ == EOF)
			{
				buffer_ = "";
				curState_ = NONE;
			}
			continue;
		case(IN_COMMENT_MULTI_LINE):
			if (s_ == '*' )
			{
				curState_ = IN_COMMENT_MULTI_LINE_ASTERISK;
			}
			else if(s_ == EOF )
			{
				throw scanner_exception ("Not a closed multi-line comment", getCol(), getLine());
			}
			continue;
		case(IN_COMMENT_MULTI_LINE_ASTERISK):
			if ( s_ == '/')
			{
				curState_ = NONE;				
			}
			else if (s_ != '*' )
			{
				curState_ = IN_COMMENT_MULTI_LINE;
			}
			else if(s_ == EOF )
			{
				throw scanner_exception ("Not a closed multi-line comment", getCol(), getLine());
			}
			continue;		
		default:
			throw scanner_exception ("Unknown error", getCol(), getLine());
		}
	}
	return false;
}


Lexeme* Scanner::get()
{
	buffer_.clear();
	return curLexem_;	
}