#include "Symbols.h"
#include <iostream>

using namespace std;

static const int N = 3;

map<OperationType, SymType*> operationTypeOperands;
map<OperationType, SymType*> operationReturningType;

SymTypeScalar* intType = new SymTypeInt();
SymTypeScalar* floatType = new SymTypeFloat();
SymTypeScalar* charType = new SymTypeChar();
SymTypeScalar* voidType = new SymTypeVoid();

Node* makeTypecastNode(Node* exp, SymType* from, SymType* to)
{
    if (!from->canConvertTo(to))
        throw parser_exception("Cannot perform conversion", exp->getLine(), exp->getCol());
    if (from == to || *from == to)
        return exp;
    if (!dynamic_cast<SymTypeScalar*>(from) || !dynamic_cast<SymTypeScalar*>(to))
    {
        return exp;
    } else {
        if (to->getPriority() - from->getPriority() == 1)
            return new TypecastNode(0, exp, to);
        return new TypecastNode(0, makeTypecastNode(exp, from, intType), floatType);
    }
}

UnaryOpNode::UnaryOpNode(Lexeme* op, Node* oper): OpNode(op), operand(oper)
{

}

SymType* TypecastNode::getType()
{
    return type;
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
    SymType* leftType = left->getType();
    SymType* rightType = right->getType();
    if (dynamic_cast<SymTypeDef*>(leftType))
        leftType = leftType->getType();
    if (dynamic_cast<SymTypeDef*>(rightType))
        rightType = rightType->getType();
    OperationType op = dynamic_cast<OperationLexeme*>(lexeme_)->getOpType();
    SymType* maxTypeOfArgs = NULL;
    if (operationTypeOperands.count(op))
        maxTypeOfArgs = operationTypeOperands[op];
    else
        maxTypeOfArgs = leftType->getPriority() > rightType->getPriority() ? leftType : rightType;
    SymTypePointer* lp = dynamic_cast<SymTypePointer*>(leftType);
    SymTypePointer* rp = dynamic_cast<SymTypePointer*>(rightType);
    SymTypeArray* la = dynamic_cast<SymTypeArray*>(leftType);
    SymTypeArray* ra = dynamic_cast<SymTypeArray*>(rightType);


//
//    if (operationTypeOperands.count(op))
//        maxTypeOfArgs = operationTypeOperands[op];
//    else
//        maxTypeOfArgs = typePriority[leftType] > typePriority[rightType] ? leftType : rightType;
//    PointerSym* lp = dynamic_cast<PointerSym*>(leftType);
//    PointerSym* rp = dynamic_cast<PointerSym*>(rightType);
//    ArraySym* la = dynamic_cast<ArraySym*>(leftType);
//    ArraySym* ra = dynamic_cast<ArraySym*>(rightType);
    
    
    switch (op)
    {
        case MOD_ASSIGN:
        case AND_ASSIGN:
        case OR_ASSIGN:
        case BITWISE_SHIFT_LEFT_ASSIGN:
        case BITWISE_SHIFT_RIGHT_ASSIGN:
            if (!leftType->canConvertTo(intType) || !rightType->canConvertTo(intType))
                throw parser_exception("Invalid operator arguments type (required int in both sides)", lexeme_->getLine(), lexeme_->getCol());
        case ASSIGN:
            if (leftType->isStruct() && *leftType == rightType)
                return leftType;
        case MULT_ASSIGN:
        case PLUS_ASSIGN:
        case MINUS_ASSIGN:
        case DIV_ASSIGN:
            if (!left->isModifiableLvalue())
                throw parser_exception("Left argument of assignment must be modifiable lvalue", left->getLine(), left->getCol());
            right = makeTypecastNode(right, rightType, leftType);
            return leftType;
        case DOT:
        case ARROW:
            return rightType;
        case MINUS:
            if ((lp && rp) || (la && ra))
            {
                if ((lp && rp && !(*lp->getType() == rp->getType()))
                    || (la && ra && !(*la->getType() == ra->getType())))
                    throw parser_exception("Operand types are incompatible", lexeme_->getLine(), lexeme_->getCol());
                return intType;
            }
        case PLUS:
            if ((lp && rp) || (la && ra))
                throw parser_exception("Cannot add two pointers", lexeme_->getLine(), lexeme_->getCol());
            if (lp || rp)
                return lp == 0 ? rightType : leftType;
            if (la || ra)
                return new SymTypePointer(la == 0 ? ra->getType() : la->getType());
        default:
            if (leftType->isStruct() || rightType->isStruct())
                throw parser_exception("Cannot perform operation over two structures", lexeme_->getLine(), lexeme_->getCol());
            if (maxTypeOfArgs->getPriority() < max(leftType->getPriority(),rightType->getPriority()))
                throw parser_exception("Invalid type of operands", lexeme_->getLine(), lexeme_->getCol());
            left = makeTypecastNode(left, leftType, maxTypeOfArgs);
            right = makeTypecastNode(right, rightType, maxTypeOfArgs);
            if (operationReturningType.count(op))
                return operationReturningType[op];
            else 
                return maxTypeOfArgs;
    }
}

//bool BinaryOpNode::isAssignment(OperationType op)
//{
//    return op == ASSIGN || op == PLUS_ASSIGN || op == MINUS_ASSIGN
//    || op == MULT_ASSIGN || op == DIV_ASSIGN || op == MOD_ASSIGN
//    || op == AND_ASSIGN || op == OR_ASSIGN
//    || op == BITWISE_SHIFT_LEFT_ASSIGN || op == BITWISE_SHIFT_RIGHT_ASSIGN;
//}
//
//bool BinaryOpNode::isComparison(OperationType op)
//{
//    return op == EQUAL || op == LESS || op == GREATER
//    || op == LESS_OR_EQUAL || op == GREATER_OR_EQUAL
//    || op == NOT_EQUAL;
//}

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
            if (!type->canConvertTo(floatType))
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
    return intType;
}

void FloatNode::print(int offset, bool isTree)
{
	cout << string(isTree ? offset * N : 0, ' ') << dynamic_cast<FloatLexeme*>(lexeme_)->getValue() << (isTree ? "\n" :"");
}

SymType *FloatNode::getType()
{
    return floatType;
}

void CharNode::print(int offset, bool isTree)
{
	cout << string(isTree ? offset * N : 0, ' ') << dynamic_cast<CharLexeme*>(lexeme_)->getValue() << (isTree ? "\n" :"");
}

SymType *CharNode::getType()
{
    return charType;
}

void IdentifierNode::print(int offset, bool isTree)
{
	cout << string(isTree ? offset * N : 0, ' ') << dynamic_cast<IdentificatorLexeme*>(lexeme_)->getValue()<< (isTree ? "\n" :"");
}

bool IdentifierNode::isModifiableLvalue()
{
    return !(var->isConst() || var->getType()->isFunc() ||
             var->getType()->isStruct() || var->getType()->isArray());
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
	cout << string(isTree ? offset * N : 0, ' ') << type->getName() << (isTree ? "\n" :"");
	cout << string(isTree ? offset * N : 0, ' ') << "(" << (isTree ? "\n" :"");
	operand->print(offset + 1, isTree);
	cout << string(isTree ? offset * N : 0, ' ') << ")" << (isTree ? "\n" :"");
}

