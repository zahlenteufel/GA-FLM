/* -
 * Gene.h
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

#ifndef _GENE_H
#define _GENE_H

#include <vector>
#include <string>
#include "FLM_Conf.h"

using namespace std;

class Gene {

public:
  Gene(const FLM_Conf& flm_conf, const vector<int>& geneArray);
  vector<int> seqAtLevel(int lev) const;
  vector<int> seq() const; 
  vector<string> allFactors() const; // returns all initial factors
  void setSeq(int i, int bit); // Directly sets the bit of the gene sequence. Used for debugging purposes only.
  string getSmoothOpt(int i) const;

private:

  const FLM_Conf& flm_conf;
  vector<int> _seq; // Gene sequence
  vector<int> _smooth_opt; // options for smoothing

};

#endif // _GENE_H
