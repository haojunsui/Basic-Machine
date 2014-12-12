// Variable Tree Header File
// A symbol table for variables will be represented here with 
// a binary tree, associating variable names with integer variables.
// The exterior interface will do nothing but assign to variables
// and look up their values, so the only purpose in having the
// structure definition here is to enable access to the overall tree.
#ifndef VARTREE
#define VARTREE

#include <iostream>
#include <string>
#include <stack>
using namespace std;

class VarTree;

// A node anywhere in tree
class TreeNode
{
    friend class VarTree;
    private:
	string	name;		// variable name
	int	value;		// variable value
	TreeNode *left,		// sub-tree for less than
		 *right;	// sub-tree for greater than

	// Private constructor: only for use by VarTree
	TreeNode( string newName , int val )
	{
	    name.assign( newName );	// get the name
	    value = val;		// and the value
	    left = right = NULL;	// no children
	}
    
    public:
    friend ostream& operator<<(ostream& os, VarTree &vars);
};

// And a mechanism to store and retrieve entries in the tree.
class VarTree
{
    private:
	TreeNode *root;
    public:
        VarTree()
	{
	    root = NULL;	// empty tree
	}
	void assign( string, int );
	int lookup( string );
    friend ostream& operator<<(ostream& os, VarTree &vars);
    
    private:		// these just help VarTree do its job
	void recursiveAssign( TreeNode *&, string, int );
	void recursiveLookup( TreeNode *&, string, int & );
};

#endif
