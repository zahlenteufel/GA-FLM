/* -
 * Node.h
 *      .h file the .cpp file.
 *
 *  Written by Kevin Duh <duh@ee.washington.edu>
 * 
 * Copyright (c) 2004, University of Washington
 *
 * Permission to use, copy, modify, and distribute this
 * software and its documentation for any non-commercial purpose
 * and without fee is hereby granted, provided that the above copyright
 * notice appears in all copies.  The University of Washington,
 * Seattle make no representations about
 * the suitability of this software for any purpose.  It is provided
 * "as is" without express or implied warranty.
 *
 */

#ifndef _NODE_H
#define _NODE_H

#include <string>
#include <vector>
using namespace std;

class Node {

public:

	Node(vector<string> factors);
	void addDropList(string factor);
	int level();
	Node* makeChild(int rule_index, int child_index);
	void printDebug(); 
	vector<string> activeFactors();
	void setSkip(bool b);
	vector<int> children();
	string printFile(); // prints lines in the factor file for this node

private:
	
	vector<string> _activeFactors; // list of factors that are in the conditioning set for this node. Used by printFile().
	int _level; // the level of this node in the backoff graph
	vector<string> _dropList; // list of factors that can dropped from the current node. Used by printFile().
	vector<int> _child; //index of children in BackoffGraph's vector
	bool _skip; // true means skip this level
	
};

#endif
