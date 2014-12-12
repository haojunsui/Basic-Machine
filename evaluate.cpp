// Simple Expression Evaluation
// This program will evaluate simple arithmetic expressions
// represented as a linked-list of tokens.  Keyboard input
// will be accepted into a string, which will be converted
// into that linked list.
//
// If the first symbol in the input string is an operator,
// then the value of the previous expression will be taken
// as an implicit first operand.
//
// The expressions may consist of the following:
// -- integer values (which may have multiple digits)
// -- simple arithmetic operators ( +, -, *, /, % )
// -- matched parentheses for grouping

// This implementation consists of a set of mutually-recursive
// functions. which will track the structure of the expression.
//
// A sum expression is the sum or difference of one or more products.
// A product expression is the product or quotient of one or more factors.
// A factor may be a number or a parenthesized sum expression.

#include "tokenlist.h"
#include "exprtree.h"
#include "evaluate.h"
#include "vartree.h"
#include "funmap.h"

void define	   (ExprNode *&root, ListIterator &IFX_iter, const ListIterator IFX_end, FunctionDef &funs);
void assign	   (ExprNode *&root, ListIterator &IFX_iter, const ListIterator IFX_end, FunctionDef &funs);
void condition (ExprNode *&root, ListIterator &IFX_iter, const ListIterator IFX_end, FunctionDef &funs);
void compare   (ExprNode *&root, ListIterator &IFX_iter, const ListIterator IFX_end, FunctionDef &funs);
void sum	   (ExprNode *&root, ListIterator &IFX_iter, const ListIterator IFX_end, FunctionDef &funs);
void product   (ExprNode *&root, ListIterator &IFX_iter, const ListIterator IFX_end, FunctionDef &funs);
void factor	   (ExprNode *&root, ListIterator &IFX_iter, const ListIterator IFX_end, FunctionDef &funs);
void funcs	   (ExprNode *&root, ListIterator &IFX_iter, const ListIterator IFX_end, FunctionDef &funs);

int evaluate(const char str[], VarTree &vars, FunctionDef &funs)
{
    static int num = 0;
    TokenList IFX(str);
    ListIterator IFX_iter = IFX.begin();
    const ListIterator IFX_end = IFX.end();
    ExprNode *root = NULL;
    
    // Store the previous value if starting with operator
    if (!IFX_iter.token().isInteger() && !IFX_iter.token().isVariable() &&
        IFX_iter.token().tokenChar() != "(" && IFX_iter.token().variableName() != "deffn")
        IFX.push_front(Token(num));
    
    define(root, IFX_iter, IFX_end, funs);		// generate expression tree
    
    if (root != NULL)
        cout << (num = root->evaluate(vars));
    
    return num;
}

// define
// Initialize a function for future use
void define(ExprNode *&root, ListIterator &IFX_iter, const ListIterator IFX_end, FunctionDef &funs)
{
    if (IFX_iter.token().variableName() == "deffn")
    {
        FunDef func;
        func.locals = new VarTree();
        IFX_iter.advance();		// go pass deffn
        func.name = IFX_iter.token().variableName();
        IFX_iter.advance();		// go pass function name
        IFX_iter.advance();		// go pass (
        for (int pos = 0; IFX_iter.token().tokenChar() != ")" && pos < 10; pos++)
        {
            if (IFX_iter.token().tokenChar() == ",")
                IFX_iter.advance();
            func.parameter[pos] = IFX_iter.token().variableName();
            func.locals->assign(func.parameter[pos], 0);
            IFX_iter.advance();		// go pass parameter name
        }
        IFX_iter.advance();		// go pass )
        IFX_iter.advance();		// go pass =
        assign(func.functionBody, IFX_iter, IFX_end, funs);
        
        if (funs.find(func.name) == funs.end())
            funs.insert({func.name, func});
        else
            funs[func.name].functionBody = func.functionBody;
        root = NULL;
        
        // Print the function
        string print = "Define " + func.name + "(";
        for (int i = 0; func.parameter[i] != ""; i++)
        {
            if (i != 0)
                print += ",";
            print += func.parameter[i];
        }
        cout << print << ")";
    }
    else
        assign(root, IFX_iter, IFX_end, funs);
}

// equal
// Generate expression for assignment
void assign(ExprNode *&root, ListIterator &IFX_iter, const ListIterator IFX_end, FunctionDef &funs)
{
    condition(root, IFX_iter, IFX_end, funs);
    
    // If the assignment operation happens
    while (IFX_iter != IFX_end &&
           IFX_iter.token().tokenChar() == "=")
    {
        ExprNode *tempLeftNode = root,
        *tempRightNode = NULL;
        IFX_iter.advance();     // go pass =
        condition(tempRightNode, IFX_iter, IFX_end, funs);
        root = new Operation(tempLeftNode, "=", tempRightNode);
    }
}

// condition
// Generate expression fo condition
void condition(ExprNode *&root, ListIterator &IFX_iter, const ListIterator IFX_end, FunctionDef &funs)
{
    compare(root, IFX_iter, IFX_end, funs);
    
    while (IFX_iter != IFX_end &&
           IFX_iter.token().tokenChar() == "?")
    {
        ExprNode *test = root,
        *trueCase, *falseCase;
        IFX_iter.advance();		// go past the ?
        assign(trueCase, IFX_iter, IFX_end, funs);
        IFX_iter.advance();		// go past the :
        assign(falseCase, IFX_iter, IFX_end, funs);
        root = new Conditional(test, trueCase, falseCase);
    }
}

// compare
// Generate expression for comparison
void compare(ExprNode *&root, ListIterator &IFX_iter, const ListIterator IFX_end, FunctionDef &funs)
{
    sum(root, IFX_iter, IFX_end, funs);
    
    // If the operator is conditional operator
    while (IFX_iter != IFX_end &&
           (IFX_iter.token().tokenChar() == "<" || IFX_iter.token().tokenChar() == "<=" ||
            IFX_iter.token().tokenChar() == ">" || IFX_iter.token().tokenChar() == ">=" ||
            IFX_iter.token().tokenChar() == "==" || IFX_iter.token().tokenChar() == "!="))
    {
        Token oper = IFX_iter.token();
        ExprNode *tempLeftNode = root,
        *tempRightNode = NULL;
        IFX_iter.advance();		// go past the operator
        sum(tempRightNode, IFX_iter, IFX_end, funs);
        root = new Operation(tempLeftNode, oper.tokenChar(), tempRightNode);
    }
}

// sum
// Generate a sum expression: the sum or difference of one or more products
// There may be the possibility of a leading - that would be implicitly
// subtracting the first product from zero.
void sum(ExprNode *&root, ListIterator &IFX_iter, const ListIterator IFX_end, FunctionDef &funs)
{
    product(root, IFX_iter, IFX_end, funs);
    
    while (IFX_iter != IFX_end &&
           (IFX_iter.token().tokenChar() == "+" ||
            IFX_iter.token().tokenChar() == "-"))
    {
        Token oper = IFX_iter.token();
        ExprNode *tempLeftNode = root,
        *tempRightNode = NULL;
        IFX_iter.advance();     // get past the operator
        product(tempRightNode, IFX_iter, IFX_end, funs);
        root = new Operation(tempLeftNode, oper.tokenChar(), tempRightNode);
    }
}

// product
// Generate a product expression: the product or quotient of factors
void product(ExprNode *&root, ListIterator &IFX_iter, const ListIterator IFX_end, FunctionDef &funs)
{
    factor(root, IFX_iter, IFX_end, funs);
    
    while (IFX_iter != IFX_end &&
           (IFX_iter.token().tokenChar() == "*" ||
            IFX_iter.token().tokenChar() == "/" ||
            IFX_iter.token().tokenChar() == "%"))
    {
        Token oper = IFX_iter.token();
        ExprNode *tempLeftNode = root,
        *tempRightNode = NULL;
        IFX_iter.advance();     // get past the operator
        factor(tempRightNode, IFX_iter, IFX_end, funs);
        root = new Operation(tempLeftNode, oper.tokenChar(), tempRightNode);
    }
}

// factor
// A factor may either be a single-digit number
// or a parenthsized expression.
void factor(ExprNode *&root, ListIterator &IFX_iter, const ListIterator IFX_end, FunctionDef &funs)
{
    if (IFX_iter.token().isInteger())
    {
        root = new Value(IFX_iter.token().integerValue());
        IFX_iter.advance();		// get past the digit
    }
    else
    {
        if (IFX_iter.token().tokenChar() == "(")
        {
            IFX_iter.advance();		// go past assumed (
            assign(root, IFX_iter, IFX_end, funs);
            IFX_iter.advance();		// go past assumed )
        }
        else if (IFX_iter.token().tokenChar() == "-")
        {
            ExprNode *tempLeftNode = new Value(0),
            *tempRightNode = NULL;
            IFX_iter.advance();
            product(tempRightNode, IFX_iter, IFX_end, funs);
            root = new Operation(tempLeftNode, "-", tempRightNode);
        }
        else
        {
            ListIterator temp_iter = IFX_iter;
            temp_iter.advance();
            if (temp_iter != IFX_end && temp_iter.token().tokenChar() == "(")
                funcs(root, IFX_iter, IFX_end, funs);
            else
                root = new Variable(IFX_iter.token().variableName());
            IFX_iter.advance();
        }
    }
}

// funcs
// Generate functional exprnode for function call, supports recursion
void funcs(ExprNode *&root, ListIterator &IFX_iter, const ListIterator IFX_end, FunctionDef &funs)
{
    string name = IFX_iter.token().variableName();
    IFX_iter.advance();		// go pass function name;
    IFX_iter.advance();		// go pass (
    ExprNode *para_list[10] = {NULL};
    for (int pos = 0; IFX_iter != IFX_end && IFX_iter.token().tokenChar() != ")" && pos < 10; pos++)
    {
        if (IFX_iter.token().tokenChar() == ",")
            IFX_iter.advance();
        assign(para_list[pos], IFX_iter, IFX_end, funs);
    }
    root = new Functional(name, para_list, &funs);
}
