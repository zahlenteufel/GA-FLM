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
 * BackoffGraph.cpp
 *     Object for representing and growing the backoff graph structure of a 
 *     factored language model.
 */

#include "BackoffGraph.h"
#include <queue>
#include <map>

BackoffGraph::BackoffGraph(const FLM_Conf& flm_conf, const vector<int>& indv) : 
  g(flm_conf, indv), factor_to_predict(flm_conf.factor_to_predict), default_discount(flm_conf.default_discount) {
  // factor_to_predict = flm_conf.factor_to_predict;
  // default_discount = flm_conf.default_discount;
  grow();
}

BackoffGraph::~BackoffGraph() {
  for (Node* node : nodes)
    delete node;
}

// Transforms factor tag, eg. M1 becomes M(-1). This is needed for writing factor-files.
string transform_factor_tag(const string orig) {
  // NOTE: current implementation assumes that the number in M1 is only single digit, 
  // and blindly adds a ")" to the end and a "(-" before the last character.
  // If more complex factor-tags are written (which is unlikely), this function may need to be rewritten.
  
  string newtag = orig;

  int openparen_position = orig.size() - 1;
  newtag.insert(openparen_position, "(-");
  newtag.insert(newtag.size(), ")");
  return newtag;
}

// Main function call in charge of printing the factor file for a backoff graph.
// This is done by recursively calling Node::printFile for each Node in the Backoff graph.
// The smoothing options are implemented in the end by addOptions().
string BackoffGraph::factor_file(const string& ga_path, const string& basename) {
  // If nodes is empty, write no file
  if (!nodes.empty()) {
    queue<int> que;
    string ffile = "1\n";
    ffile += factor_to_predict + ": ";
    int count = 0; //counts the number of lines for backoff

    Node* root = nodes[0];

    vector<string> startFactors = root->activeFactors();
    ffile += " " + to_string(startFactors.size()) + " ";

    for (string factor : startFactors)
      ffile += transform_factor_tag(factor) + " ";
    
    ffile += ga_path + basename + ".count.gz " + ga_path + basename + ".lm.gz INSERT_NUMBER_HERE\n";

    ffile += root->printFile();
    count++; // counts the number of lines

    // Init queue
    for (int child : root->children())
      que.push(child);

    // Breadth-first printout of nodes by using queue
    map<string, int> headmap;
    while (!que.empty()) {
      int n = que.front(); // note that n is an index to a Node, not the node or node reference.
      que.pop(); // pop first element off the queue
      
      Node* node = nodes[n];

      // get the line to print out for this node
      string ffile_line = node->printFile();
      
      // Due to the implementation of backoffgraph.grow(), repeated nodes may occur.
      // Thus, there may be repeated lines. This doesn't change the FLM, but makes the factor-file looks not pretty.
      // So here we find those duplicates and don't print them out. This is done by finding the "head" of each line
      // (the active factors and drop list), which uniquely identifies a node and checking a map to see if this head appeared before.
      string::size_type headend = ffile_line.find_first_of(' ');
      string head = ffile_line.substr(0, headend);

      if (headmap.count(head) == 0) {
        // this node is new, so we print it
        count++;
        ffile += ffile_line;
        headmap[head] = 1;
      }

      // Now push the children nodes in the que, to be printed later
      vector<int> children = node->children();
      for (int child : children)
        que.push(child);
    }

    // Factor-file requires us to indicate the number of lines in backoff, so here it is.
    string::size_type pos = ffile.find(string("INSERT_NUMBER_HERE"));
    ffile.replace(pos, 18, to_string(count + 1));

    // Add smoothing options by changing the ffile string
    string newffile = add_options(ffile);

    // final unigram distribution 
    newffile +=  "0 \t 0 \t ";
    newffile += default_discount;
    newffile += " gtmin 1\n";

    return newffile;
  }
}

// Uses information in smoothing options part of the gene to change the factor-file correspondingly
string BackoffGraph::add_options(const string& ffile) {
  size_t i = 0;
  vector<string> lines;
  map<string, string> smoothmap;

  while (i != string::npos) {
    j = ffile.find_first_of("\n", i + 1);
    lines.push_back(ffile.substr(i + 1, j - i));
    i = j;
  }

  // Here we create newffile and start reading ffile line by line, and writing the changes to newffile
  string newffile;
  newffile += "1\n";

  int index = 0;
  for (int j = 0; j < int(lines.size()) - 1; j++) {
    string l = lines[j];

    string::size_type pos = l.find("CHANGEME");
    if (pos != string::npos) {
      string replacestring = g.getSmoothOpt(index);
      l.replace(pos, 17, replacestring);
      newffile += l;
      index = index + 2;
      // index points to the current bit/integer in the smoothing options gene. 
      // Note that index jumps by two because we have a tuple (discount,gtmin)
    }
    else {
      newffile += l;
    }
  }
  return newffile;
}

// Creates all children nodes of a parent node using the rules specified by the gene.
// This is a recursive call, so it actually creates all descendents of the root node, resulting in a backoff graph.
void BackoffGraph::use_rule(Node* n) {
  int lev = n->level();

  if (lev > 1) {
    vector<int> rules = g.seqAtLevel(lev);
    int sum = 0; // used to check if all rules are 0 (skip level)
    for (int i = 0; i < int(rules.size()); i++) {
      sum += rules[i];
      // RuleBit=1, so grammar rule generates child node
      if (rules[i] == 1) {
        // j is index into nodes. It is used in MakeChild node to tell the current node the index of its children
        Node* child = n->makeChild(i, j);
        nodes.push_back(child);
        j++;
        // recursion if needed
        if (lev > 2) {
          use_rule(child);
        }
        else {
          vector<string> factor = child->activeFactors();
          child->addDropList(factor[0]);
        }
      }
    }
    // Check if sum==0 still. If so, skip the level.
    if (sum == 0)
      skip(n, lev);
  }
  else if (lev == 1) {
    vector<string> factor = n->activeFactors();
    n->addDropList(factor[0]);
  }
}

// This function implements skipping a level in backoff. fngram-count assumes that we only drop one factor at a time in backoff, 
// so skipping is achieved by setting the gtmin to very high values (so that it will immediately backoff twice)
void BackoffGraph::skip(Node* n, int lev) {
  n->setSkip(true);

  for (int i = 0; i < lev; i++) {
    // grammar rule generates child node
    Node* child = n->makeChild(i,j);
    nodes.push_back(child);
    j++;
    // recursion if needed
    if (lev > 2) {
      use_rule(child);
    }
    else {
      vector<string> factor = child->activeFactors();
      child->addDropList(factor[0]);
    }
  }
}

// The main function for growing a backoff graph from initial node and graph production rules.
void BackoffGraph::grow() {
  // Create root node
  vector<string> factors = g.allFactors();
  if (!factors.empty()) {
    Node* root = new Node(factors);
    nodes.push_back(root); 
    j = 1; // index for all nodes in nodes
    use_rule(root);
  }
  // else {
  //   // Grow nothing. nodes vector remain empty
  // }
}
