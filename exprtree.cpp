// Expression Tree Implementation File
// These functions manage an expression tree that describes
// arithmetic expressions using integer variables and values.
// Some functions construct a tree from a character string that
// represents the expression, and then the tree can be traversed
// and evaluated.
#include <sstream>
#include "exprtree.h"
#include "tokenlist.h"
#include "vartree.h"

// Outputting any tree node will simply output its string version
ostream& operator<<( ostream &stream, const ExprNode &e )
{
    return stream << e.toString();
}

// A Value is just an integer value -- easy to evaluate
// Unfortunately, the string class does not have a constructor for it
string Value::toString() const
{
    ostringstream convert;	// output string stream
    convert << value;		// 'output' into the stream
    return convert.str();	// and extract its string equivalent
}

string Value::toLispString() const
{
    ostringstream convert;	// output string stream
    convert << value;		// 'output' into the stream
    return convert.str();	// and extract its string equivalent
}

int Value::evaluate( VarTree &v ) const
{
    return value;
}

//  A variable is just an alphabetic string -- easy to display
//  TO evaluate, would need to look it up in the data structure
string Variable::toString() const
{
    return name;
}

string Variable::toLispString() const
{
    return name;
}

int Variable::evaluate( VarTree &v ) const
{
    return v.lookup( name );
}

//  An operator is a string
//  TO evaluate, would need to evaluate left and right and either assign
//  or calculate or compare
string Operation::toString() const
{
    return "(" + left->toString() + " " + oper + " " + right->toString() + ")";
}

string Operation::toLispString() const
{
    if (oper == "=")
        return "(setq " + left->toLispString() + " " + right->toLispString() + ")";
    return "(" + oper + " " + left->toLispString() + " " + right->toLispString() + ")";
}

int Operation::evaluate( VarTree &v ) const
{
    if (oper == "+")
        return left->evaluate(v) + right->evaluate(v);
    else if (oper == "-")
        return left->evaluate(v) - right->evaluate(v);
    else if (oper == "*")
        return left->evaluate(v) * right->evaluate(v);
    else if (oper == "/")
        return left->evaluate(v) / right->evaluate(v);
    else if (oper == "%")
        return left->evaluate(v) % right->evaluate(v);
    else if (oper == "<=")
        return left->evaluate(v) <= right->evaluate(v);
    else if (oper == ">=")
        return left->evaluate(v) >= right->evaluate(v);
    else if (oper == "<")
        return left->evaluate(v) < right->evaluate(v);
    else if (oper == ">")
        return left->evaluate(v) > right->evaluate(v);
    else if (oper == "==")
        return left->evaluate(v) == right->evaluate(v);
    else if (oper == "!=")
        return left->evaluate(v) != right->evaluate(v);
    else if (oper == "=")
    {
        int temp = right->evaluate(v);
        v.assign(left->toString(), temp);
        return temp;
    }
    
    return 0;
}

//  An condition is a collection of string
//  TO evaluate, would need to evaluate test case, if true choose trueCase
//  if false choose falseCase
string Conditional::toString() const
{
    return "(" + test->toString() + " ? " + trueCase->toString() + " : " + falseCase->toString() + ")";
}

string Conditional::toLispString() const
{
    return "(if " + test->toLispString() + " " + trueCase->toLispString() + " " + falseCase->toLispString() + ")";
}

int Conditional::evaluate( VarTree &v ) const
{
    if (test->evaluate(v) != 0)
        return trueCase->evaluate(v);
    else
        return falseCase->evaluate(v);
}

string Functional::toString() const
{
    string print = name + "(";
    for (int i = 0; para_list[i] != NULL && i < 10; i++)
    {
        if (i != 0)
            print += ",";
        print += para_list[i]->toString();
    }
    print += ")";
    return print;
}

string Functional::toLispString() const
{
    string print = name + "(";
    for (int i = 0; para_list[i] != NULL && i < 10; i++)
    {
        if (i != 0)
            print += ",";
        print += para_list[i]->toLispString();
    }
    print += ")";
    return print;
}

int Functional::evaluate( VarTree &v ) const
{
    FunDef *temp_func = &funcs->find(name)->second;
    VarTree temp_var;
    
	for (int i = 0; temp_func->parameter[i] != "" && i < 10; i++)
    {
		if (para_list[i] == NULL)
			temp_var.assign(temp_func->parameter[i], 0);
		else
			temp_var.assign(temp_func->parameter[i], para_list[i]->evaluate(v));
    }
    
    return temp_func->functionBody->evaluate(temp_var);
}
