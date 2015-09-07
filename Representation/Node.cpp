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
 * Node.cpp
 *     Class for representing a node in the backoff graph.
 */

#include <iostream>
// #include "../util.h"
#include "Node.h"

Node::Node(vector<string> factors) : _activeFactors(factors) {
  _skip = false;
  _level = factors.size();
}

int Node::level() {
  return _level;
}

vector<string> Node::activeFactors() {
  return _activeFactors;
}

void Node::setSkip(bool b) {
  _skip = b;
}

// adds a factor into the droplist. The drop list specifies which factors may be dropped for a given backoff
void Node::addDropList(string factor) {
  _dropList.push_back(factor);
}

// Creates factor for child node and augments dropList for self, given a rule that's invoked
Node* Node::makeChild(int rule_index, int child_index) {
  vector<string> childFactors;
  for (int i = 0; i < int(_activeFactors.size()); i++) {
    if (i != rule_index)
      childFactors.push_back(_activeFactors[i]);
    else
      _dropList.push_back(_activeFactors[rule_index]);
  }

  Node* child = new Node(childFactors);
  _child.push_back(child_index);

  return child;
}

vector<int> Node::children() {
  return _child;
}

// void Node::printDebug() {
//   cout << "Node: level=" << _level << endl;
//   cout << "Factors: ";
//   prs(_activeFactors);
//   cout << "Drop: ";
//   prs(_dropList);
//   cout << "Child: " ;
//   prv(_child);
//   cout << "Skip: " <<  _skip << endl;
//   cout << endl;
// }

// Main function for printing a line the factor-file. 
// The printed line represents the active factors, drop factors, discount options and gtmin, etc. of this node.
string Node::printFile() {
  string ffile;

  // Active conditioning factors at this node
  for (string factor : _activeFactors)
    ffile +=  factor;
  
  ffile += "\t";

  // Droplist, factors that can be backed off at this nodes
  for (string factor : _dropList)
    ffile += factor;

  // CHANGEME is a placeholder for the discounting method
  ffile += "\t CHANGEME";

  // skip means this node will have very high gtmin, so it's effectively skipped.
  if (_skip)
    ffile += "  gtmin 1000000";
  else
    ffile += "  gtmin 1";

  // If droplist is more than 1, we should combine the backoff paths. 
  // Currently just use "combine mean", which is computationally quicker than "combine max" for example.
  if (_dropList.size() > 1)
    ffile += "  combine mean\n";
  else
    ffile += "  interpolate\n";

  return ffile;
}
