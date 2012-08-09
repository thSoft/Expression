
// Some Useful Functions for the Expression Class
// (c) thSoft, 2005

#ifndef SOMEFUNCTIONS_H
#define SOMEFUNCTIONS_H

#include <math.h>

bool checkInvTrig(double x)
{
	return ((x >= -1) && (x <= 1));
}

bool checkAcosh(double x)
{
	return (x >= 1);
}

bool checkAtanh(double x)
{
	return ((x > -1) && (x < 1));
}

bool checkPositive(double x)
{
	return (x > 0);
}

bool checkNotNegative(double x)
{
	return (x >= 0);
}

bool checkTan(double x)
{
	return (cos(x) != 0);
}

double acosh(double x)
{
	return log(x+sqrt(x-1)*sqrt(x+1));
}

double asinh(double x)
{
	return log(x+sqrt(x*x+1));
}

double atanh(double x)
{
	return (log(1+x)-log(1-x))/2;
}

double fracpart(double x)
{
	return modf(x, &x);
}

double intpart(double x)
{
	modf(x, &x);
	return x;
}

void addCommon(Expression &e)
{
	e.addFunction("abs", fabs);
	e.addFunction("acos", acos, checkInvTrig);
	e.addFunction("acosh", acosh, checkAcosh);
	e.addFunction("asin", asin, checkInvTrig);
	e.addFunction("asinh", asinh);
	e.addFunction("atan", atan);
	e.addFunction("atanh", atanh, checkAtanh);
	e.addFunction("ceil", ceil);
	e.addFunction("cos", cos);
	e.addFunction("cosh", cosh);
	e.addFunction("exp", exp);
	e.addFunction("floor", floor);
	e.addFunction("frac", fracpart);
	e.addFunction("int", intpart);
	e.addFunction("log", log10, checkPositive);
	e.addFunction("ln", log, checkPositive);
	e.addFunction("sin", sin);
	e.addFunction("sinh", sinh);
	e.addFunction("sqrt", sqrt, checkNotNegative);
	e.addFunction("tan", tan, checkTan);
	e.addFunction("tanh", tanh);
	e.addConstant("pi", acos(-1.0));
	e.addConstant("e", exp(1.0));
}

#endif // SOMEFUNCTIONS_H
