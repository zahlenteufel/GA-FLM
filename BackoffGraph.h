#include "Node.h"
#include "Gene.h"

class BackoffGraph {

  public:

    BackoffGraph(const Gene& gene);
    ~BackoffGraph();
    void printDebug();
    void useRule(Node* n); // grows the current node's children
    void skip(Node* n, int lev); // skips the next level
    // main function for printing factor-files
    void printFile(
      const string& FACTOR_TO_PREDICT,
      const string& DEFAULT_DISCOUNT,
      const string& GA_PATH,
      const string& basename);

    // main function for including smoothing options in the backoff graph's factor file.
    string addOptions(const string& ffile); 

  private:

    vector<Node*> _nodes; // contains all nodes in the backoff graph
    int j; // index into _nodes
    string flmfile; // name of factorfile for this backoff graph

    const Gene& g;
    void grow(); // main function for growing a backoff graph

};
