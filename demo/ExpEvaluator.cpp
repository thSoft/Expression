
// Expression Evaluator Sample Program
// (c) thSoft, 2005

#include <iostream>
#include <iomanip>
#include <string>
#include "Expression.h"
#include "SomeFunctions.h"

using namespace std;

const char *errorNames[] =
{
	"Blank expression",
	"Illegal character in expression",
	"Parantheses do not match",
	"Expression syntax error",
	"Unknown identifier",
	"Division by zero",
	"Domain error",
	"Overflow"
};

int main()
{
	char *s = new char[256];
	Expression *expression = new Expression();
	double result;
	bool ok = false;
	addCommon(*expression);
	cout << endl << "Expression Evaluator" << endl << "--------------------" << endl;
	while (!ok)
	{
	 	cout << endl << "Enter an Expression to Evaluate:" << endl;
		cin >> ws;
		cin.getline(s, 256);
		cout << endl;
  		try
		{
			expression->set(s);
			ok = true;
			result = expression->evaluate();
			cout << "The Result is: " << endl << setprecision(8) << result << endl;
		}
		catch (const ExpException &e)
		{
			ExceptionCode c = e.getCode();
			char *d = e.getInfo();
			cout << "Alas, an Error occured while Evaluating this Expression: " << endl << errorNames[c];
			switch (c)
			{
				case EE_ILLEGAL:
				case EE_UNKNOWNID:
				case EE_ILLEGALID:
				case EE_REDECLARED:
				{
					cout << " \"" << d << "\"";
					break;
				}
				case EE_DOMAIN:
				case EE_OVERFLOW:
				{
					if (!strcmp(d, ""))
					{
						cout << " while Raising to Power";
					} else
					{
						cout << " in Function " << d;
					}
					break;
				}
			}
			cout << endl;
		}
	}
	delete expression;
	delete[] s;
	return 0;
}
