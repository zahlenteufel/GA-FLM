/*
 *  Written by Kevin Duh and Sonia Parandekar
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
 */

#ifndef GA_H
#define GA_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <map>

#include "GA_Conf.h"
#include "FLM_Conf.h"

using namespace std;

class GA {

public:

  GA(const GA_Conf& ga_conf, const FLM_Conf& flm_conf, float convergence_threshold = 0.001);

  void search();

private:

  const GA_Conf& ga_conf;
  const FLM_Conf& flm_conf;

  float convergence_threshold;
  int generation_number;
  Population population;
  vector<float> fitness;
  Chromosome historic_best;
  float historic_best_fitness;
  
  void create_new_generation();
  void create_initial_population();
  bool termination_criteria_satisfied() const;
  float average_fitness() const;
  float best_fitness() const;

  struct info {
    float fitness, perplexity, logprob;
    int complexity;
  };

  info parse_info(const string& evallog_filename, const string& complexity_filename) const;
  info evaluate(const Chromosome& chromosome) const;
  info evaluate(const string& filename) const;
  mutable map<string, info> cache;

  void create_factor_file(const Chromosome& chromosome) const;
  void compute_fitness_of_each_gene();
  bool has_converged() const;
  void debug_algorithm() const;

};

#endif 
