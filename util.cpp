/*
 * "Copyright 2004, University of Washington. All Rights Reserved
 *
 *    Written by Kevin Duh <duh@ee.washington.edu>
 *
 * NO WARRANTY
 * THE PROGRAM IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED INCLUDING, WITHOUT
 * LIMITATION, ANY WARRANTIES OR CONDITIONS OF TITLE, NON-INFRINGEMENT,
 * MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE. Each Recipient is
 * solely responsible for determining the appropriateness of using the Program
 * and assumes all risks associated with such use, including but not limited
 * to the risks and costs of program errors, compliance with applicable laws,
 * damage to or loss of data, programs or equipment, and unavailability or
 * interruption of operations.

 * DISCLAIMER OF LIABILITY
 * THE UNIVERSITY OF WASHINGTON AND KEVIN DUH
 * SHALL NOT HAVE ANY LIABILITY FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING WITHOUT 
 * LIMITATION LOST PROFITS), HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE  OF
 * THE PROGRAM, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES."
*/

/*-
 * util.cpp
 *     Simple utility functions for ga-flm.cpp
 */

#include <cmath>
#include <sstream>
#include <iostream>
#include <unistd.h>
#include "util.h"

// converts int into string
string itos(int i) {
  stringstream s;
  s << i;
  return s.str();
}

// Prints vectors (Helper)
void prv(vector<int> v) {
  for (int x : v)
    cout << x << " ";
  cout << endl;
}

void prs(vector<string> v) {
  for (string s : v)
    cout << s << " ";
  cout << endl;
}

// Splits a string into several a vector of smaller strings based on the delimiter.
vector<string> split(const string& input, const string& delimiter) {
  size_t start = 0, end = 0;
  vector<string> results;
  while ((end = input.find(delimiter, start)) != string::npos) {
    results.push_back(input.substr(start, end - start));
    start = end + 1;
  }
  results.push_back(input.substr(start));
  return results;
}

float parse_float(const string& s) {
  stringstream ss(s);
  float result;
  ss >> result;
  return result;
}

int parse_int(const string& s) {
  stringstream ss(s);
  int result;
  ss >> result;
  return result;
}

// Extracts the option from a line of PARAM file
string _extract_option_string(const string& input_str) {
  // Extracts substring after "=" character
  int delim_pos = input_str.find_first_of('=');
  int length = input_str.size() - delim_pos;
  string option_str = input_str.substr(delim_pos + 1, length);

  // Trim white spaces before and after the substring
  int beginpos = option_str.find_first_not_of(' ');
  int endpos = option_str.find_last_not_of(' ');

  int optionlength = endpos - beginpos + 1;
  string final_str = option_str.substr(beginpos,optionlength);
  
  return final_str;
}

string extract_option(ifstream& param_file) {
  string input_str;
  getline(param_file, input_str, '\n');
  return _extract_option_string(input_str);
}

void sys(const string& s) {
  sys(s.c_str());
}

void sys(const char* s) {
  // cerr << s << endl;
  if (system(s) != 0) {
    cerr << "system() failed" << endl;
    exit(1);
  }
}
