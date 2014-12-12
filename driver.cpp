#include <iostream>
#include "evaluate.h"
using namespace std;

int main()
{
	VarTree vars;		// initially empty tree
	FunctionDef funs;
	int cnt = 1;
	char userInput[80];
	string input;

	cout << "******************************\n"
		 << "    Welcome to my Machine.\n"
		 << "******************************\n"
		 << "Here are some functions that are already defined for you.\n\n";
    
    evaluate("deffn gcf(a,b) = (rem = a%b) == 0?b:gcf(b,rem)", vars, funs);
    cout << endl;
    evaluate("deffn lcm(a,b) = a*b/gcf(a,b)", vars, funs);
    cout << endl;
    evaluate("deffn mod(a,b) = a % b", vars, funs);
    cout << endl;
    evaluate("deffn sqr(s) = s*s", vars, funs);
    cout << endl;
    evaluate("deffn abs(x) = x > 0 ? x : -x", vars, funs);
    cout << endl;
    evaluate("deffn cube(x) = x * x * x", vars, funs);
    cout << endl;
    evaluate("deffn sum3(x,y,z) = x + y + z", vars, funs);
    cout << endl;
    evaluate("deffn avg5(x,y,z,a,b) = (x + y + z + a + b)/5", vars, funs);
    cout << endl;
    evaluate("deffn odd(x) = x%2?1:0", vars, funs);
    cout << endl;
    evaluate("deffn even(x) = x%2?0:1", vars, funs);
    cout << endl;
    evaluate("deffn neg(x) = -x", vars, funs);
    cout << endl;
    evaluate("deffn fact(n) = n <= 1 ? 1 : n * fact(n-1)", vars, funs);
    cout << endl;
    evaluate("deffn pow(a,b)= b==0?1:a*pow(a,b-1)", vars, funs);
    cout << endl;
    evaluate("deffn fib(n) = n <2?n:fib(n-1)+fib(n-2)", vars, funs);
    cout << endl;
    cout << endl;
    
	cout << "You may define more functions in the following format.\n\n"
		 << "deffn sqr(s) = s*s\n\n"
		 << "You may type 'exit' to exit the program.\n"
		 << "Good luck.\n\n";

	while (input != "exit")
	{
		cout << cnt++ << ": ";
		getline(cin, input);
		strcpy(userInput, input.c_str());
		if (!input.empty() && input != "exit")
		{
			cout << cnt++ << ": ";
			evaluate(userInput, vars, funs);
			cout << endl;
		}
	}
    
    cout << vars << endl;
    
    system("pause");
    return 0;
}
