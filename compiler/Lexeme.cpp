#pragma once
#include "Lexeme.h"
#include <iostream>


Lexeme::Lexeme(void)
{
}

Lexeme::~Lexeme(void)
{
}

void Lexeme::print()
{
	char* typeDef[] = {
	"ReservedWord",
	"Identificator",
	"Integer",
	"Float",
	"Error",
	"Separator",
	"Operation",
	"Char",
	"String"
};

	printf("row = %d, col = %d\n",row_,col_);
	if (floatValue_ != 0)
		printf("float value = %f\n",floatValue_);
	else if (intValue_ != 0)
		printf("int value = %d\n",intValue_);
	std::cout << "string value = "<< stringValue_ << endl;  
	printf("type = %s\n ================================================ \n", typeDef[type_]);
}

