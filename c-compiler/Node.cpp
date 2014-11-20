#include "Symbols.h"
#include <iostream>

using namespace std;

static const int N = 3;


UnaryOpNode::UnaryOpNode(Lexeme* op, Node* oper): OpNode(op), operand(oper) 
{

}

void EmptyNode::print(int offset, bool isTree)
{ 
    cout << string(isTree ? offset * N : 0, ' ') << "<empty>" << (isTree ? "\n" :"");
}

void BinaryOpNode::print(int offset, bool isTree)
{
    if (left != NULL) {
        left->print(offset + 1, isTree);
    } else {
        cout << string(isTree ? offset * N : 0, ' ') << "";
    }
	cout << string(isTree ? offset * N : 0, ' ') << dynamic_cast<OperationLexeme*>(lexeme_)->getValue()<< (isTree ? "\n" :"");
    if (right != NULL) {
        right->print(offset + 1, isTree);
    } else {
        cout << string(isTree ? offset * N : 0, ' ') << "";
    } 
}

bool BinaryOpNode::haveBranch()
{
    return left != NULL && right != NULL;
}

bool BinaryOpNode::isLvalue()
{
    switch (dynamic_cast<OperationLexeme *>(lexeme_)->getOpType())
    {
        case DOT:
        case ARROW:
            return right->isLvalue() && !right->getType()->isArray();
        default:
            return isModifiableLvalue();
    }

}

bool BinaryOpNode::isModifiableLvalue()
{
    switch (dynamic_cast<OperationLexeme *>(lexeme_)->getOpType())
    {
        case ASSIGN:
        case PLUS_ASSIGN:
        case MINUS_ASSIGN:
        case MULT_ASSIGN:
        case DIV_ASSIGN:
        case MOD_ASSIGN:
        case BITWISE_XOR_ASSIGN:
        case BITWISE_SHIFT_LEFT_ASSIGN:
        case BITWISE_SHIFT_RIGHT_ASSIGN:
        case OR_ASSIGN:
        case AND_ASSIGN:
            return left->isModifiableLvalue();
        case DOT:
        case ARROW:
            return right->isModifiableLvalue();
        default:
            return false;
    }
}

SymType *BinaryOpNode::getType()
{
    SymType* rootType = NULL;
    SymType* leftType = left->getType();
    SymType* rightType = right->getType();
    
    OperationType opType = dynamic_cast<OperationLexeme *>(lexeme_)->getOpType();

    if (dynamic_cast<SymTypeDef *>(leftType))
        leftType = leftType->getType();
    if (dynamic_cast<SymTypeDef*>(rightType))
        rightType = rightType->getType();
    
    SymType* maxTypeOfArgs = priorityType[leftType] > priorityType[rightType] ? leftType : rightType;
//    PointerSym* lp = dynamic_cast<PointerSym*>(leftType);
//    PointerSym* rp = dynamic_cast<PointerSym*>(rightType);
//    ArraySym* la = dynamic_cast<ArraySym*>(leftType);
//    ArraySym* ra = dynamic_cast<ArraySym*>(rightType);
    switch (opType)
    {
        case MOD_ASSIGN:
        case AND_ASSIGN:
        case OR_ASSIGN:
        case BITWISE_SHIFT_LEFT_ASSIGN:
        case BITWISE_SHIFT_RIGHT_ASSIGN:
            //if (!leftType->canConvertTo(intType) || !rightType->canConvertTo(intType))
            //    throw CompilerException("Invalid operator arguments type (required int in both sides)", token->line, token->col);
            // fallthrough
        case ASSIGN:
            //if (leftType->isStruct() && *leftType == rightType)
            //    return leftType;
            // fallthrough
        case MULT_ASSIGN:
        case PLUS_ASSIGN:
        case MINUS_ASSIGN:
        case DIV_ASSIGN:
            //if (!left->isModifiableLvalue())
            //    throw CompilerException("Left argument of assignment must be modifiable lvalue", left->token->line, left->token->col);
            //right = makeTypeCoerce(right, rightType, leftType);
            //return leftType;
        case DOT:
            return rType;
        case ARROW:
            //if (!lp || (!dynamic_cast<StructSym*>(lp->type) && !dynamic_cast<StructSym*>(dynamic_cast<AliasSym*>(lp->type)->type)))
            //    throw CompilerException("Left operand of -> must be of pointer-to-structure type", left->token->line, left->token->col);
            //return rightType;
        case MINUS:
            //if (lp && rp || la && ra)
            //{
            //   if (lp && rp && *lp->type != rp->type
            //        || la && ra && *la->type != ra->type)
            //    throw CompilerException("Operand types are incompatible", token->line, token->col);
            //    return intType;
            //}
        case PLUS:
            //if (lp && rp || la && ra)
            //   throw CompilerException("Cannot add two pointers", token->line, token->col);
            //if (lp || rp)
            //    return lp == 0 ? rightType : leftType;
            //if (la || ra)
            //    return new PointerSym(la == 0 ? ra->type : la->type);
        default: ;
            //if (leftType->isStruct() || rightType->isStruct())
            //    throw CompilerException("Cannot perform operation over two structures", token->line, token->col);
            //if (typePriority[maxTypeOfArgs] < max(typePriority[leftType], typePriority[rightType]))
            //    throw CompilerException("Invalid type of operands", token->line, token->col);
            //left = makeTypeCoerce(left, leftType, maxTypeOfArgs);
            //right = makeTypeCoerce(right, rightType, maxTypeOfArgs);
            //if (operationReturningType.count(op))
            //    return operationReturningType[op];
            //else
            //    return maxTypeOfArgs;
    }
    return rootType;
}


bool UnaryOpNode::isLvalue()
{
    return *lexeme_ == MULT || ((*lexeme_ == DEC || *lexeme_ == INC) && operand->isLvalue());
}

bool UnaryOpNode::isModifiableLvalue()
{
    return (*lexeme_ == MULT || *lexeme_ == DEC || *lexeme_ == INC) && getType()->isModifiableLvalue();
}

SymType *UnaryOpNode::getType()
{
    SymType* type = operand->getType();
    OperationType op = dynamic_cast<OperationLexeme *>(lexeme_)->getOpType();
    switch (op)
    {
        case MULT:
            if (!dynamic_cast<SymTypePointer*>(type))
                throw parser_exception("Type of right expression of '*' is not a pointer", lexeme_->getLine(), lexeme_->getCol());
            return dynamic_cast<SymTypePointer*>(type)->getType();
        case BITWISE_AND:
            if (!operand->isLvalue())
                throw parser_exception("Expression must have lvalue", lexeme_->getLine(), lexeme_->getCol());
            return new SymTypePointer(type);
            break;
        case BITWISE_NOT:
            //operand = makeTypeCoerce(operand, type, intType);
            break;
        case LOGICAL_NOT:
            if (dynamic_cast<SymTypeStruct*>(type))
                throw parser_exception("Cannot perform logical not operation over structure", lexeme_->getLine(), lexeme_->getCol());
            break;
        case DEC:
        case INC:
            if (!operand->isModifiableLvalue())
                throw parser_exception("Expression must have modifiable lvalue", lexeme_->getLine(), lexeme_->getCol());
            break;
        case MINUS:
            if (!type->canConvertTo(new SymTypeFloat()))
                throw parser_exception("Expression must have arithmetic type", lexeme_->getLine(), lexeme_->getCol());
            break;
        default:
            break;
    }
    return type;
}

void UnaryOpNode::print(int offset, bool isTree)
{
    cout << string(isTree ? offset * N : 0, ' ') << dynamic_cast<OperationLexeme*>(lexeme_)->getValue()<< (isTree ? "\n" :"");
    operand->print(offset + 1, isTree);
}

void TernaryOpNode::print(int offset, bool isTree)
{
	string tab = string(isTree ? offset * N : 1, ' ');
	condition->print(offset + 1, isTree);
	cout << tab << "?" << (isTree ? "\n" :"");
	left->print(offset + 1, isTree);
	cout << tab << ":" << (isTree ? "\n" :"");
	right->print(offset + 1, isTree);

}

void IntNode::print(int offset, bool isTree)
{
	cout << string(isTree ? offset * N : 0, ' ') << dynamic_cast<IntegerLexeme*>(lexeme_)->getValue()<< (isTree ? "\n" :"");
}

SymType *IntNode::getType()
{
    return new SymTypeInt();
}

void FloatNode::print(int offset, bool isTree)
{
	cout << string(isTree ? offset * N : 0, ' ') << dynamic_cast<FloatLexeme*>(lexeme_)->getValue() << (isTree ? "\n" :"");
}

SymType *FloatNode::getType()
{
    return new SymTypeFloat();
}

void CharNode::print(int offset, bool isTree)
{
	cout << string(isTree ? offset * N : 0, ' ') << dynamic_cast<CharLexeme*>(lexeme_)->getValue() << (isTree ? "\n" :"");
}

SymType *CharNode::getType()
{
    return new SymTypeChar();
}

void IdentifierNode::print(int offset, bool isTree)
{
	cout << string(isTree ? offset * N : 0, ' ') << dynamic_cast<IdentificatorLexeme*>(lexeme_)->getValue()<< (isTree ? "\n" :"");
}

bool IdentifierNode::isModifiableLvalue()
{
    return NULL;
}

SymType *IdentifierNode::getType()
{
    return var->getType();
}

void IdentifierNode::setVar(SymVar *v)
{
    var = v;
}

void PostfixUnaryOpNode::print(int offset, bool isTree)
{
	operand->print(offset, isTree);
	cout << string(isTree ? offset * N : 0, ' ') << dynamic_cast<OperationLexeme*>(lexeme_)->getValue()<< (isTree ? "\n" :"");
}

void FuncCallNode::printArgs(int offset, bool isTree)
{
	for (int i = 0; i < (int)args.size(); i++)
	{
		args[i]->print(offset + 1, isTree);
		if (i < (int)args.size() - 1)
			cout << string(isTree ? offset * N : 0, ' ') << ',' << (isTree ? "\n" :"");
	}
}

void FuncCallNode::addArg(Node* arg)
{
	if(arg != NULL)
		args.push_back(arg); 
}

void FuncCallNode::print(int offset, bool isTree)
{
	name->print(offset, isTree);
	cout << string(isTree ? offset * N : 0, ' ') << "(" << (isTree ? "\n" :"");
	printArgs(offset, isTree);
	cout << string(isTree ? offset * N : 0, ' ') << ")" << (isTree ? "\n" :"");
}

void ArrNode::print(int offset, bool isTree)
{
    name->print(offset, isTree);
	cout << string(isTree ? offset * N : 0, ' ') << "[" << (isTree ? "\n" :"");
	printArgs(offset, isTree);
	cout << string(isTree ? offset * N : 0, ' ') << "]" << (isTree ? "\n" :"");
}

SymType *ArrNode::getType()
{
    if (!dynamic_cast<SymTypeArray *>(type)) {
        throw parser_exception("Subscripted value is not an array", lexeme_->getCol(), lexeme_->getLine() );
    }
    return dynamic_cast<SymTypeArray *>(type)->getType();
}

void ArrNode::printArgs(int offset, bool isTree)
{
    for (int i = 0; i < (int)size.size(); i++)
    {
        size[i]->print(offset + 1, isTree);
        if (i < (int)size.size() - 1)
            cout << string(isTree ? offset * N : 0, ' ') << ',' << (isTree ? "\n" :"");
    }
}

void ArrNode::addIndex(Node *idx)
{
    if(idx != NULL)
        size.push_back(idx);
}

bool ArrNode::isModifiableLvalue()
{
    SymType* type = name->getType();
    for (int i = 0; i < size.size(); i++)
        type = type->getType();
    return type->isModifiableLvalue();
}

void ListNode::print(int deep, bool isTree)
{
    cout << string(isTree ? deep * N : 0, ' ') << "{" ;
    for (int i = 0 ; i < args.size(); i++) {
        args[i]->print(deep, isTree);
        if (i != args.size() - 1) {
            cout << string(isTree ? deep * N : 0, ' ') << "," << (isTree ? "\n" :"");
        }
    }
    cout << string(isTree ? deep * N : 0, ' ') << "}" << (isTree ? "\n" :"");
}
void ListNode::addArg(Node* newNode)
{
    if(newNode != NULL)
        args.push_back(newNode);
    else
    {
        throw parser_exception("Args in list node == NULL", false);
    }
}

SymType *ListNode::getType()
{
    return type;
}


void TypecastNode::print(int offset, bool isTree)
{
	string typeName = "";
	switch (dynamic_cast<ReservedWordLexeme*>(lexeme_)->getRwType())
	{
	case(T_CHAR):
		typeName = "char";
		break;
	case(T_INT):
		typeName = "int";
		break;
	case(T_FLOAT):
		typeName = "float";
		break;
    default:
        break;
	}	
	cout << string(isTree ? offset * N : 0, ' ') << typeName << (isTree ? "\n" :"");
	cout << string(isTree ? offset * N : 0, ' ') << "(" << (isTree ? "\n" :"");
	operand->print(offset + 1, isTree);
	cout << string(isTree ? offset * N : 0, ' ') << ")" << (isTree ? "\n" :"");
}

