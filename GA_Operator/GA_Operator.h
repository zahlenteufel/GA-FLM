/* -
 * GA_Operator.h
 *      .h file the .cpp file.
 *
 *  Written by Kevin Duh and Sonia Parandekar
 * 
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

#ifndef GA_OPERATOR_H
#define GA_OPERATOR_H

#include <random>
#include <vector>

using namespace std;

typedef vector<int> Chromosome;
typedef vector<Chromosome> Population;


class GA_Operator {

public:

  GA_Operator(int chromosome_length);

  // void be_elitist(); // <-- TODO: change this name...

  //return a random number between 0 and 1
  float random_number_in_01();

  int random_bit();

  int random_int(int max);

protected:

  int chromosome_length;
  default_random_engine rng;

};

#endif
