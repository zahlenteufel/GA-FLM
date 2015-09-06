#ifndef BACKOFF_GRAPH_H
#define BACKOFF_GRAPH_H

#include "../FLM_Conf.h"
#include "Node.h"
#include "Gene.h"

class BackoffGraph {

  public:

    BackoffGraph(const FLM_Conf& flm_conf, const vector<int>& indv);

    ~BackoffGraph();

    void use_rule(Node* n); // grows the current node's children

    void skip(Node* n, int lev); // skips the next level

    string factor_file(const string& ga_path, const string& basename);

    // main function for including smoothing options in the backoff graph's factor file.
    string add_options(const string& ffile); 

  private:

    Gene g;
    const string factor_to_predict;
    const string default_discount;
    vector<Node*> nodes; // contains all nodes in the backoff graph
    int j; // index into nodes

    void grow(); // main function for growing a backoff graph <<-- should go in constructor

};

#endif
