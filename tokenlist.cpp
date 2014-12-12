// Token List Implementation file
//
// This tokenizer will scan a character string representing
// an expression, and will return a list of tokens.
// --- only integers and operators are supported (no names)

// The standard C library has some useful functions for us
#include <string>
#include <ctype.h>
// And to get the definition of a token:
#include "tokenlist.h"

//  output operation
//  Display all of the tokens in the list
ostream& operator<<( ostream &stream, TokenList &t )
{
	ListElement *curr;
	for (curr = t.head; curr != NULL; curr = curr->next)
		stream << " " << curr->token;
	return stream;
}

void TokenList::push_front( Token t )
{
	ListElement *newNode = new ListElement;
	newNode->token = t;
	newNode->next = head;
	if (head == NULL)
		head = tail = newNode;
	else
		head = newNode;
}

void TokenList::push_back( Token t )
{
    ListElement *newNode = new ListElement;
    newNode->token = t;
	newNode->next = NULL;
	if (head == NULL)
		head = tail = newNode;
	else
	{
		tail->next = newNode;
		tail = newNode;
	}
}

TokenList::TokenList( const char str[] )
{
    int total = 0;
    
    head = NULL;
    tail = NULL;
    
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (!isspace(str[i]))
        {
			if (isdigit(str[i]))
			{
				while (isdigit(str[i]))
				{
					total = 10 * total + str[i] - '0';
					i++;
				}
				push_back(Token(total));
                total = 0;
                i--;
			}
            else if (isalpha(str[i]))
            {
                string temp_name;
                while (isalpha(str[i]) || isdigit(str[i]))
                {
                    temp_name.push_back(str[i]);
                    i++;
                }
                push_back(Token(temp_name));
                i--;
            }
            else
            {
                string temp_name;
                temp_name.push_back(str[i]);
                if (str[i] != ')')
                {
                    i++;
                    if (str[i] == '=')
                        temp_name.push_back(str[i]);
                    else
                        i--;
                }
                push_back(Token(temp_name));
            }
        }
    }
}

void TokenList::removeHead()
{
	ListElement *remove = head;
	if (head->next != NULL)
		current = head = head->next;
	else
		current = head = NULL;
	delete remove;
}


void TokenList::print()
{
	ListIterator l = begin();
	while (l != end())
	{
		if (l.token().isInteger())
			cout << l.token().integerValue() << " ";
		else
        {
            if (l.token().isVariable())
                cout << l.token().variableName() << " ";
            else
                cout << l.token().tokenChar() << " ";
        }
		l.advance();
	}
	cout << endl;
}

// Here follow a couple extra functions to add into
// the TokenList implementation file to support iterators.

//  Creates an iterator to refer to the beginning of the list
ListIterator TokenList::begin()
{
	return ListIterator(this, &head);
}

//  Creates an iterator to refer after the end of the list
ListIterator TokenList::end()
{
	return ListIterator(this, &tail->next);
}
