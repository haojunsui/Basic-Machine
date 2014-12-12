// Variable Tree Implementation File
// This is a simple binary search tree associating variables
// with integer values.
#include <iostream>
#include <string>
#include "vartree.h"
using namespace std;

//  recursiveAssign
//  A recursive tree-traversal function to assign to a variable.
//  Parameters:
//  	node	(modified TreeNode ptr)	tree to insert into
//  	name	(input string)		name of variable
//  	value	(input integer)		value to assign
//  NOTE: node May be a null pointer, indicating an empty tree
void VarTree::recursiveAssign( TreeNode *&root, string name, int value )
{
    if (root != NULL)
    {
        if (name.compare(root->name) == 0)
            root->value = value;
        else if (name.compare(root->name) < 0)
            recursiveAssign(root->left, name, value);
        else
            recursiveAssign(root->right, name, value);
    }
    else
        root = new TreeNode(name, value);
}

//  assign
//  Assigns a value to a variable.
//  If the variable does not yet exist, it is created.
//  Parameters:
//  	name	(input string)		name of variable
//  	value	(input integer)		value to assign
void VarTree::assign( string name, int value )
{
    recursiveAssign( root, name, value );
}

//  recursiveLookup
//  A recursive tree-traversal function to search for a variable.
//  If the variable does not exist, it is created with a value of 0.
//  Parameters:
//  	node	(modified TreeNode ptr)	tree to insert into
//  	name	(input string)		name of variable
//  	value	(output integer)	value of variable
//  NOTE: node May be a null pointer, indicating an empty tree
void VarTree::recursiveLookup( TreeNode *&root, string name, int &value )
{
    if (root == NULL)
        root = new TreeNode(name, value = 0);
    else
    {
        if (name.compare(root->name) == 0)
            value = root->value;
        else if (name.compare(root->name) < 0)
            recursiveLookup(root->left, name, value);
        else
            recursiveLookup(root->right, name, value);
    }
}

//  lookup
//  Searches for a variable to get its value
//  If the variable does not yet exist, it is created.
//  Parameters:
//  	name	(input char array)	name of variable
//  Returns:				value of variable
int VarTree::lookup( string name )
{
    int value;
    recursiveLookup( root, name, value );
    return value;
}

ostream& operator<<(ostream& os, VarTree &vars)
{
    os << "\n\nThe variables you inserted are as the following: \n\n";
    
    if (vars.root == NULL)
        os << "None\n";
    else
    {
        stack<TreeNode*> s;
        TreeNode *current = vars.root;
        bool done = false;
        while (!done)
        {
            if (current)
            {
                s.push(current);
                current = current->left;
            }
            else
            {
                if (s.empty())
                    done = true;
                else
                {
                    current = s.top();
                    s.pop();
                    os << current->name << " = " << current->value << endl;
                    current = current->right;
                }
            }
        }
    }
    return os;
}
