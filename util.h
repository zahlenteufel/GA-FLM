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

#ifndef _UTIL_H
#define _UTIL_H

#include <string>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

void sys(const string& s);
void sys(const char* s);

string itos(int i); // converts int into string

float parse_float(const string& s);
int parse_int(const string& s);

void prv(vector<int> v);
void prs(vector<string> v);

vector<string> split(const string& input, const string& delimiter);

string extract_option(ifstream& param_file);

string extract_enum(const string name, ifstream& param_file, const list<string>& options);

#endif
