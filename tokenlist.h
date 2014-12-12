// Token List Header file
// This is a linked list for use with the tokens for an
// arithmetic expression.  Although it is used for that
// particular application, this implementation more
// resembles a generic linked list, which may be used
// either as a stack or a queue.

// There will be support for displaying the entire list at once
#ifndef TOKENLIST
#define TOKENLIST

// And of course, the tokens themselves
#include "token.h"

class ListIterator;

// Since this is a linked list, we actually need a linked list node.
struct ListElement
{
    Token		token;		// the token data itself
    struct ListElement *next;		// next element of list
};

class TokenList
{
	friend class ListIterator;
    friend ostream& operator<<( ostream &, TokenList &);
private:
	ListElement *head, 	// front of the list
    *tail;	// tail of the list
	ListElement *current;	// facilitates traversal of the list
public:
	TokenList()		// create an empty list
	{
	    head = NULL;
	    tail = NULL;
	    current = NULL;
	}
    
	TokenList( const char str[] );	// or create initial list
    // to appear in 'tokenlist.c'
    //
	~TokenList()			// destructor -- clear the list
	{
	    ListElement *remove;
	    while ( (remove = head) != NULL)
	    {
            head = head->next;	// find the successor
            delete remove;		// and deallocate this
	    }
	}
    
	//  A couple functions to add to the list (in 'tokenlist.cpp')
	void push_front( Token t );
	void push_back( Token t );
	ListIterator begin();
	ListIterator end();
	void removeHead();
	void print();

	//  A couple functions to support list traversal:
	//  They are all very short, using the 'current' member variable
	void start()
	{
        current = head;
	}
    
	void advance()
	{
        current = current->next;
	}
	
	bool atEnd() const
	{
        if (current == tail || current == NULL)
            return true;
        
        return false;
	}
	
	// nextToken
	// Return the current token
	Token nextToken() const
	{
        return current->token;
	}
};



//  NOTE:  This code is intended to be copied to the end
//  of an existing "tokenlist.h" file, since it will be used
//  in many of the same contexts.
//
//  This ListIterator is here to facilitate sequential access
//  to a linked list by those parts of the program that are not
//  given full access to the list itself.   It is initialized
//  in its constructor, and then proceeds to the end of the list.
//
//  For this to succeed, TokenList must declare this class as a friend.
//  and TokenList will have these two extra methods declared as well.
//  (The }; you see below is the end of the TokenList class definition)

// and now some support for the List Iterator


class ListIterator
{
	friend class TokenList;	// let that class create iterators
private:
	TokenList *list;	// which list
	ListElement **curr;	// pointer to pointer to current node
	ListIterator(TokenList *l, ListElement **le)
	{
		list = l;
		curr = le;
	}
public:
	Token& token() const
	{
		return (*curr)->token;
	}
	bool ended() const
	{
		return (*curr == NULL);	// list ends with NULL
	}
	void advance()
	{
		if (*curr != NULL)
			curr = &(*curr)->next;
	}
	int operator !=(const ListIterator &other) const
	{
		return list != other.list || curr != other.curr;
	}
};

#endif
