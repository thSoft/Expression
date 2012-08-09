
// Function Integrator Sample Program
// (c) thSoft, 2005

#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <ctype.h>
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
	"The Range is Invalid!",
	"The Range is Invalid!",
	"An Overflow Occured!"
};

double getDouble(char *msg)
{
	char s[256];
	double x;
	unsigned i;
	bool ok = false, d;
	while (!ok)
	{
		cout << msg;
		cin >> ws;
		cin.getline(s, 256);
		ok = true;
		d = false;
		for (i = 0; i < strlen(s); i++)
		{
			switch (s[i])
			{
				case '.':
				{
					if (d)
					{
						ok = false;
					} else
					{
						d = true;
					}
					break;
				}
				case '-':
				{
					if (i)
					{
						ok = false;
					}
					break;
				}
				default:
				{
					if (!isdigit(s[i]))
					{
						ok = false;
					}
				}
			}
		}
	}
	x = atof(s);
	return x;
}

unsigned getPositive(char *msg)
{
	unsigned x, i;
	char s[256];
	bool ok = false;
	while (!ok)
	{
		cout << msg;
		cin >> ws;
		cin.getline(s, 256);
		ok = true;
		for (i = 0; i < strlen(s); i++)
		{
			if (!isdigit(s[i]))
			{
				ok = false;
			}
		}
		x = atoi(s);
		if (!x)
		{
			ok = false;
		}
	}
	return x;
}

int main()
{
	char *s = new char[256];
	Expression *expression = new Expression();
	double x, x1, x2, step, aux, aux2, result = 0;
	bool ok = false;
	unsigned n, i;
	addCommon(*expression);
	expression->addVariable("x", &x);
	cout << endl << "Function Integrator" << endl << "-------------------" << endl;
	cout << endl << "[WARNING! This Program can't Calculate Improprius Integral Properly!" << endl;
	cout << "And it doesn't Perform too much Error Checking, so Use It at your Own Risk!]" << endl;
	while (!ok)
	{
	 	cout << endl << "Enter a Function to Integrate (x is the Variable):" << endl << "f(x) = ";
 		cin >> ws;
		cin.getline(s, 256);
		cout << endl;
  		try
		{
			expression->set(s);
			ok = true;
		}
		catch (const ExpException &e)
		{
			ExceptionCode c = e.getCode();
			cout << "Oh no, this Expression seems to be Incorrect: " << endl << errorNames[c];
			if ((c == EE_ILLEGAL) || (c == EE_UNKNOWNID) || (c == EE_ILLEGALID) || (c == EE_REDECLARED))
			{
				cout << " \"" << e.getInfo() << "\"";
			}
			cout << endl;
		}
	}
	try
	{
		cout << "Enter the Range: " << endl;
		x1 = getDouble("x1 = ");
		x2 = getDouble("x2 = ");
		cout << "Enter the Number of Approximating Intervals:" << endl;
		n = getPositive("n = ");
		if (x1 > x2)
		{
			swap(x1, x2);
		}
		step = (x2-x1)/n;
		x = x1;
		i = 1;
		while (x < x2)
		{
			aux = expression->evaluate();
			result += 4*aux*(i%2)+2*aux*((i+1)%2);
			x += step;
			i++;
		}
		x = x1;
		aux = expression->evaluate();
		x = x2;
		aux2 = expression->evaluate();
		result = step*(result+aux+aux2)/3;
		cout << endl << "The Approximated Value of the Integral is:" << endl << setprecision(8) << result << endl;
	}
	catch (const ExpException &e)
	{	
		cout << endl << errorNames[e.getCode()] << " Be more Careful next time!" << endl;
	}
	delete expression;
	delete[] s;
	return 0;
}
