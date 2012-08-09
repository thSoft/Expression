
// Class Expression - Header File
// (c) thSoft, 2005

#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <exception>
#include <vector>

typedef enum enum_exception_code
{
	EE_BLANK,	// Blank expression
	EE_ILLEGAL,	// Illegal character in expression
	EE_BRACKETS,	// Parantheses do not match
	EE_SYNTAX,	// Expression syntax error
	EE_UNKNOWNID,	// Unknown identifier
	EE_DIVZERO,	// Division by zero
	EE_DOMAIN,	// Domain error
	EE_OVERFLOW,	// Overflow
	EE_ILLEGALID,	// Illegal character in identifier
	EE_REDECLARED	// Identifier redeclared
} ExceptionCode;

typedef enum enum_operator_type
{
	OP_ADD,
	OP_SUB,
	OP_MUL,
	OP_DIV,
	OP_POW
} OperatorType;

typedef double (*CalculatorFn)(double);

typedef bool (*CheckerFn)(double);

bool returnTrue(double x);

class VarRef
{
	char *name;
	double *address;
public:
	VarRef(const char *name, double *address);
	VarRef(const VarRef &theOther);
	~VarRef();
	char *getName();
	double *getAddress();
};

class ConstRef
{
	char *name;
	double value;
public:
	ConstRef(const char *name, double value);
	ConstRef(const ConstRef &theOther);
	~ConstRef();
	char *getName();
	double getValue();
};

class FuncRef
{
	char *name;
	CalculatorFn calculator;
	CheckerFn checker;
public:
	FuncRef(const char *name, CalculatorFn calculator, CheckerFn checker);
	FuncRef(const FuncRef &theOther);
	~FuncRef();
	char *getName();
	CalculatorFn calculate();
	CheckerFn check();
};

class ExpException : public std::exception
{
	ExceptionCode code;
	char *info;
public:
	ExpException(ExceptionCode code);
	ExpException(ExceptionCode code, const char *info);
	ExpException(ExceptionCode code, const char *info, unsigned l);
	ExpException(ExceptionCode code, char info);
	ExpException(const ExpException &e);
	~ExpException() throw();
	ExceptionCode getCode() const;
	char *getInfo() const;
};

class Expression;

class ExpNode
{
	friend class Expression;
protected:
	ExpNode *operand1, *operand2;
public:
	ExpNode();
	virtual ~ExpNode();
	virtual double getValue() = 0;
};

class ExpNumber : public ExpNode
{
	double value;
public:
	ExpNumber(double value);
	double getValue();
};

class ExpOperator : public ExpNode
{
	OperatorType type;
public:
	ExpOperator(OperatorType type);
	double getValue();
};

class ExpFunction : public ExpNode
{
	char *name;
	CalculatorFn calculate;
	CheckerFn check;
public:
	ExpFunction(char *name, CalculatorFn calculate, CheckerFn check);
	double getValue();
};

class ExpVariable : public ExpNode
{
	double *address;
public:
	ExpVariable(double *address);
	double getValue();
};

class Expression
{
	ExpNode *root;
	char *str;
	std::vector<VarRef> variables;
	std::vector<ConstRef> constants;
	std::vector<FuncRef> functions;
	void buildTree(const char *s, unsigned l, ExpNode **n);
public:
	Expression();
	Expression(const Expression &theOther);
	~Expression();
	void clear();
	void set(const char *s);
	double evaluate();
	void addVariable(const char *name, double *address);
	void addConstant(const char *name, double value);
	void addFunction(const char *name, CalculatorFn calculate, CheckerFn check = returnTrue);
};

#endif // EXPRESSION_H
