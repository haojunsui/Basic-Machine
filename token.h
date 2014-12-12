// Token Header file
// This is for the beginning of an object-oriented string tokenizer
// to be used for arithmetic expressions.    The tokenizer is an
// object that returns a linked list of object tokens, which are these.

// Since lots of files want to know what a token is, there is the
// danger of redeclaration, which these next couple lines will neutralize.
// (There is an #endif at the bottom as well)
#ifndef TOKEN
#define TOKEN

// Provides some helpful functionality for understanding tokens
#include <iostream>
#include <stdlib.h>
#include <ctype.h>
using namespace std;

// Here is a definition of the token itself:
class Token
{
	//  All the data members are private to keep them protected.
private:
	bool    isInt;          // to identify the token type later
	int     value;          // value for an integer token
	string    oper;           // character for an operator token
    string  variable;
    
	//  All of the methods here are public (which is not always the case)
	//  First, a couple to initialize a new token, either operator or integer
public:
	Token(int i)
	{
		value = i;
		isInt = true;
		oper = "";           // initialize unused value
	}

    Token(string v)
    {
        isInt = false;
        value = 0;
        
        if (!isOper(v))
        {
            variable = v;
            oper = "";
        }
        else
        {
            variable = "";
            oper = v;
        }
    }
    
	Token()                 // default constructor
	{
		value = 0;
		oper = "";
		isInt = false;
	}
	//  Here are several accessor methods used to describe
	//  the token, making visible the hidden private members.
	//  No changes are permitted to this object -- only queries.
	//
	//  Placing these within the class definition allows for
	//  inline expansion at the function call point, to save
	//  the need for call and return.  In-line is only suitable
	//  for very short functions (like these and constructors).
	//
	//  The keyword 'const' at the end of these function headings
	//  promises that calling the method will not change the
	//  object.  The method only returns information.

	bool isNull() const
	{
		return !isInt && oper == "" && !isVariable();
	}

    bool isOper(string v) const
    {
        if (v != "+" && v != "-" && v != "*" && v != "/" &&
            v != "%" && v != "<=" && v != ">=" && v != ">" &&
            v != "<" && v != "==" && v != "!=" && v != "=" &&
            v != "(" && v != ")" && v != "?" && v != ":" && v!= ",")
            return false;
        
        return true;
    }
    
	bool isInteger() const
	{
		return isInt;
	}

    bool isVariable() const
    {
        if (variable == "")
            return false;
        return true;
    }
    
	int integerValue() const
	{
		return value;
	}

    string variableName() const
    {
        return variable;
    }
    
	string tokenChar() const
	{
		return oper;
	}

	//   And some functions will be postponed to an
	//   implementation file.

	friend ostream& operator <<(ostream& stream, Token &t);
};

// End of the conditional compilation
#endif
