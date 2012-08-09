
// Class Expression - Implementation
// (c) thSoft, 2005

#include <ctype.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include "Expression.h"

bool returnTrue(double x)
{
	return true;
}

// Class VarRef

VarRef::VarRef(const char *name, double *address)
{
	this->name = new char[strlen(name)+1];
	strcpy(this->name, name);
	this->address = address;
}

VarRef::VarRef(const VarRef &theOther)
{
	name = new char[strlen(theOther.name)+1];
	strcpy(name, theOther.name);
	address = theOther.address;
}

VarRef::~VarRef()
{
	if (name)
	{
		delete[] name;
		name = 0;
	}
}

char *VarRef::getName()
{
	return name;
}

double *VarRef::getAddress()
{
	return address;
}

// Class ConstRef

ConstRef::ConstRef(const char *name, double value)
{
	this->name = new char[strlen(name)+1];
	strcpy(this->name, name);
	this->value = value;
}

ConstRef::ConstRef(const ConstRef &theOther)
{
	name = new char[strlen(theOther.name)+1];
	strcpy(name, theOther.name);
	value = theOther.value;
}

ConstRef::~ConstRef()
{
	if (name)
	{
		delete[] name;
		name = 0;
	}
}

char *ConstRef::getName()
{
	return name;
}

double ConstRef::getValue()
{
	return value;
}

// Class FuncRef

FuncRef::FuncRef(const char *name, CalculatorFn calculator, CheckerFn checker)
{
	this->name = new char[strlen(name)+1];
	strcpy(this->name, name);
	this->calculator = calculator;
	this->checker = checker;
}

FuncRef::FuncRef(const FuncRef &theOther)
{
	name = new char[strlen(theOther.name)+1];
	strcpy(name, theOther.name);
	checker = theOther.checker;
	calculator = theOther.calculator;
}

FuncRef::~FuncRef()
{
	if (name)
	{
		delete[] name;
		name = 0;
	}
}

char *FuncRef::getName()
{
	return name;
}

CheckerFn FuncRef::check()
{
	return checker;
}

CalculatorFn FuncRef::calculate()
{
	return calculator;
}

//	Class ExpException

ExpException::ExpException(ExceptionCode code)
{
	this->code = code;
	info = new char[1];
	info[0] = 0;
}

ExpException::ExpException(ExceptionCode code, const char *info)
{
	this->code = code;
	this->info = new char[strlen(info)+1];
	strcpy(this->info, info);
}

ExpException::ExpException(ExceptionCode code, const char *info, unsigned l)
{
	this->code = code;
	this->info = new char[l+1];
	strncpy(this->info, info, l);
	this->info[l] = 0;
}

ExpException::ExpException(ExceptionCode code, char info)
{
	this->code = code;
	this->info = new char[2];
	this->info[0] = info;
	this->info[1] = 0;
}

ExpException::ExpException(const ExpException &e)
{
	code = e.code;
	info = new char[strlen(e.info)+1];
	strcpy(info, e.info);
}

ExpException::~ExpException() throw()
{
	if (info)
	{
		delete[] info;
		info = 0;
	}
}

ExceptionCode ExpException::getCode() const
{
	return code;
}

char *ExpException::getInfo() const
{
	return info;
}

//	Class ExpNode

ExpNode::ExpNode()
{
	operand1 = operand2 = 0;
}

ExpNode::~ExpNode()
{
	if (operand1)
	{
		delete operand1;
		operand1 = 0;
	}
	if (operand2)
	{
		delete operand2;
		operand2 = 0;
	}
}			  

//	Class ExpNumber

ExpNumber::ExpNumber(double value)
{
	operand1 = operand2 = 0;
	this->value = value;
}

double ExpNumber::getValue()
{
	return value;
}

//	Class ExpOperator

ExpOperator::ExpOperator(OperatorType type)
{
	operand1 = operand2 = 0;
	this->type = type;
}

double ExpOperator::getValue()
{
	double v;
	switch (type)
	{
		case OP_ADD:
		{
			return operand1->getValue()+operand2->getValue();
		}
		case OP_SUB:
		{
			return operand1->getValue()-operand2->getValue();
		}
		case OP_MUL:
		{
			return operand1->getValue()*operand2->getValue();
		}
		case OP_DIV:
		{
			if (!(v = operand2->getValue()))
			{
				throw ExpException(EE_DIVZERO);
			}
			return (operand1->getValue()/v);
		}
		case OP_POW:
		{
			if ((v = pow(operand1->getValue(), operand2->getValue())) == HUGE_VAL)
			{
				throw ExpException(EE_OVERFLOW);
			}
			if (errno == EDOM)
			{
				throw ExpException(EE_DOMAIN);
			}
			return v;
		}
	}	
}

//	Class ExpFunction

ExpFunction::ExpFunction(char *name, CalculatorFn calculate, CheckerFn check)
{
	operand1 = operand2 = 0;
	this->name = name;
	this->check = check;
	this->calculate = calculate;
}

double ExpFunction::getValue()
{
	double v = operand1->getValue();
	if (!check(v))
	{
		throw ExpException(EE_DOMAIN, name);
	}
	if ((v = calculate(v)) == HUGE_VAL)
	{
		throw ExpException(EE_OVERFLOW, name);
	}
	return v;
}

//	Class ExpVariable

ExpVariable::ExpVariable(double *address)
{
	operand1 = operand2 = 0;
	this->address = address;
}

double ExpVariable::getValue()
{
	if (address)
	{
		return *address;
	} else
	{
		return 0;
	}
}

//	Class Expression

Expression::Expression()
{
	root = 0;
	str = 0;
}

Expression::Expression(const Expression &theOther)
{
	root = 0;
	str = 0;
	variables = theOther.variables;
	constants = theOther.constants;
	functions = theOther.functions;
	if (theOther.str)
	{
		set(theOther.str);
	}
}

Expression::~Expression()
{
	clear();
	variables.clear();
	constants.clear();
	functions.clear();
}

void Expression::clear()
{
	if (root)
	{
		delete root;
		root = 0;
	}
	if (str)
	{
		delete[] str;
		str = 0;
	}
}

void Expression::set(const char *s)
{
	unsigned i, l = strlen(s);
	int b = 0;
	if (!l)
	{
		throw ExpException(EE_BLANK);
	}
	for (i = 0; i < l; i++)
	{
		if ((!((isalnum(s[i])) || (s[i] == '(') || (s[i] == ')') || (s[i] == '.') ||
			(s[i] == '+') || (s[i] == '-') || (s[i] == '*') || (s[i] == '/') || (s[i] == '^')))
			 || (isspace(s[i])))
		{
			throw ExpException(EE_ILLEGAL, s[i]);
		}
		if (s[i] == '(')
		{
			b++;
		} else
		if (s[i] == ')')
		{
			if (!b)
			{
       			throw ExpException(EE_BRACKETS);
			}
			b--;
		}
	}
	if (b)
	{
		throw ExpException(EE_BRACKETS);
	}
	clear();        
	buildTree(s, l, &root);
	str = new char[l+1];
	strcpy(str, s);
}

double Expression::evaluate()
{
	if (root)
	{
		return root->getValue();
	} else
	{
		throw ExpException(EE_BLANK);
	}
}

void Expression::addVariable(const char *name, double *address)
{
	unsigned i;
	for (i = 0; i < strlen(name); i++)
	{
		if (!isalpha(name[i]))
		{
			throw ExpException(EE_ILLEGALID, name[i]);
		}
	}
	for (i = 0; i < variables.size(); i++)
	{
		if (!strcmp(variables[i].getName(), name))
		{
			throw ExpException(EE_REDECLARED, name);
		}
	}
	for (i = 0; i < constants.size(); i++)
	{
		if (!strcmp(constants[i].getName(), name))
		{
			throw ExpException(EE_REDECLARED, name);
		}
	}
	for (i = 0; i < functions.size(); i++)
	{
		if (!strcmp(functions[i].getName(), name))
		{
			throw ExpException(EE_REDECLARED, name);
		}
	}
	variables.push_back(VarRef(name, address));
}

void Expression::addConstant(const char *name, double value)
{
	unsigned i;
	for (i = 0; i < strlen(name); i++)
	{
		if (!isalpha(name[i]))
		{
			throw ExpException(EE_ILLEGAL, name[i]);
		}
	}
	for (i = 0; i < variables.size(); i++)
	{
		if (!strcmp(variables[i].getName(), name))
		{
			throw ExpException(EE_REDECLARED, name);
		}
	}
	for (i = 0; i < constants.size(); i++)
	{
		if (!strcmp(constants[i].getName(), name))
		{
			throw ExpException(EE_REDECLARED, name);
		}
	}
	for (i = 0; i < functions.size(); i++)
	{
		if (!strcmp(functions[i].getName(), name))
		{
			throw ExpException(EE_REDECLARED, name);
		}
	}
	constants.push_back(ConstRef(name, value));
}

void Expression::addFunction(const char *name, CalculatorFn calculate, CheckerFn check)
{
	unsigned i;
	for (i = 0; i < strlen(name); i++)
	{
		if (!isalpha(name[i]))
		{
			throw ExpException(EE_ILLEGAL, name[i]);
		}
	}
	for (i = 0; i < variables.size(); i++)
	{
		if (!strcmp(variables[i].getName(), name))
		{
			throw ExpException(EE_REDECLARED, name);
		}
	}
	for (i = 0; i < constants.size(); i++)
	{
		if (!strcmp(constants[i].getName(), name))
		{
			throw ExpException(EE_REDECLARED, name);
		}
	}
	for (i = 0; i < functions.size(); i++)
	{
		if (!strcmp(functions[i].getName(), name))
		{
			throw ExpException(EE_REDECLARED, name);
		}
	}
	functions.push_back(FuncRef(name, calculate, check));
}

void Expression::buildTree(const char *s, unsigned l, ExpNode **n)
{
	int i, j, bl;
	double nv;
	if (!l)
	{
		throw ExpException(EE_SYNTAX);
	}
	if (s[0] == '(')
	{
		bl = 1;
		for (i = 1; i < l; i++)
		{
			switch (s[i])
			{
				case '(':
				{
					bl++;
					break;
				}
				case ')':
				{
					bl--;
					break;
				}
			}
			if (!bl)
			{
				if (i == l-1)
				{
					s++;
					l -= 2;
				}
				break;
			}
		}
	}
	bl = 0;
	for (i = 0; i < l; i++)
	{
		switch (s[i])
		{
			case '(':
			{
				bl++;
				break;
			}
			case ')':
			{
				bl--;
				break;
			}
			case '+':
			{
				if (!bl)
				{
					if ((s[i+1] == '+') || (s[i+1] == '-'))
					{
						throw ExpException(EE_SYNTAX);
					}
					*n = new ExpOperator(OP_ADD);
					buildTree(s, i, &((*n)->operand1));
					buildTree(s+i+1, l-i-1, &((*n)->operand2));
					i = l;
				}
				break;
			}
		}
	}
	if (*n) return;
	bl = 0;
	for (i = l-1; i > 0; i--)
	{
		switch (s[i])
		{
			case '(':
			{
				bl++;
				break;
			}
			case ')':
			{
				bl--;
				break;
			}
			case '-':
			{
				if (!bl)
				{
					if ((s[i+1] == '+') || (s[i+1] == '-'))
					{
						throw ExpException(EE_SYNTAX);
					}
					*n = new ExpOperator(OP_SUB);
					buildTree(s, i, &((*n)->operand1));
					buildTree(s+i+1, l-i-1, &((*n)->operand2));
					i = 0;
				}
				break;
			}
		}
	}
	if (*n) return;
	if ((s[0] == '-') && (!isdigit(s[1])) && (s[1] != '.'))
	{
		*n = new ExpOperator(OP_MUL);
		(*n)->operand1 = new ExpNumber(-1.0);
		buildTree(s+1, l-1, &((*n)->operand2));
	}
	bl = 0;
	for (i = 0; i < l; i++)
	{
		switch (s[i])
		{
			case '(':
			{
				bl++;
				break;
			}
			case ')':
			{
				bl--;
				break;
			}
			case '*':
			{
				if (!bl)
				{
					*n = new ExpOperator(OP_MUL);
					buildTree(s, i, &((*n)->operand1));
					buildTree(s+i+1, l-i-1, &((*n)->operand2));
					i = l;
				}
				break;
			}
		}
	}
	if (*n) return;
	bl = 0;
	for (i = l-1; i >= 0; i--)
	{
		switch (s[i])
		{
			case '(':
			{
				bl++;
				break;
			}
			case ')':
			{
				bl--;
				break;
			}
			case '/':
			{
				if (!bl)
				{
					*n = new ExpOperator(OP_DIV);
					buildTree(s, i, &((*n)->operand1));
					buildTree(s+i+1, l-i-1, &((*n)->operand2));
					i = 0;
				}
				break;
			}
		}
	}
	if (*n) return;
	bl = 0;
	for (i = 0; i < l; i++)
	{
		switch (s[i])
		{
			case '(':
			{
				bl++;
				break;
			}
			case ')':
			{
				bl--;
				break;
			}
			case '^':
			{
				if (!bl)
				{
					*n = new ExpOperator(OP_POW);
					buildTree(s, i, &((*n)->operand1));
					buildTree(s+i+1, l-i-1, &((*n)->operand2));
					i = l;
				}
				break;
			}
		}
	}
	if (*n) return;
	if (isalpha(s[0]))
	{
		for (i = 0; i < functions.size(); i++)
		{
			j = strlen(functions[i].getName());
			if ((!strncmp(s, functions[i].getName(), j)) && (s[j] == '('))
			{
				*n = new ExpFunction(functions[i].getName(), functions[i].calculate(), functions[i].check());
				buildTree(s+j+1, l-j-2, &((*n)->operand1));
				return;
			}
		}
		for (i = 0; i < constants.size(); i++)
		{
			if (!strncmp(s, constants[i].getName(), l))
			{
				*n = new ExpNumber(constants[i].getValue());
				return;
			}
		}
		for (i = 0; i < variables.size(); i++)
		{
			if (!strncmp(s, variables[i].getName(), l))
			{
				*n = new ExpVariable(variables[i].getAddress());
				return;
			}
		}
		for (i = 0; i < l; i++)
		{
			if (isdigit(s[i]))
			{
				throw ExpException(EE_SYNTAX);
			}
		}
		throw ExpException(EE_UNKNOWNID, s, l);
	} else
	{
		j = 0;
		for (i = 0; i < l; i++)
		{
			if (s[i] == '.')
			{
				if (j)
				{
					throw ExpException(EE_SYNTAX);
				} else
				{
					j = 1;
				}
			} else
			{
				if (isalpha(s[i]))
				{
					throw ExpException(EE_SYNTAX);
				}
			}
		}
		nv = atof(s);
		*n = new ExpNumber(nv);
	}
}
