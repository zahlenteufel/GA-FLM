/* -
 * util.h
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

#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <vector>
#include <fstream>
#include <string>
#include <list>
#include <sstream>

using namespace std;

void sys(const string& s);
void sys(const char* s);

string to_string(int i); // converts int into string

float parse_float(const string& s);
int parse_int(const string& s);

template<typename T>
ostream& operator<<(ostream &os, const vector<T> &v) {
	for (int i = 0; i < int(v.size()); i++) {
		if (i > 0)
			os << " "; 
		os << v[i]; 
	}
  return os;
}

vector<string> split(const string& input, const string& delimiter);

string extract_option(ifstream& param_file);

string extract_enum(const string name, ifstream& param_file, const list<string>& options);

#endif
