/* -
 * ga-flm.h
 *      .h file the .cpp file.
 *
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

#ifndef _GA_FLM_H
#define _GA_FLM_H

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

class GA_Operator;

class GA_FLM {

public:

  const GA_Conf& ga_conf;
  const FLM_Conf& flm_conf;
  GA_Operator* ga_operator;

  int MAXPOP;
  int MAXGEN;
  float convergence_threshold;
  const int MAXBITS = 150;
  const int MAXSTORAGE = 32768;

	GA_FLM(const GA_Conf& ga_conf, const FLM_Conf& flm_conf, float convergence_threshold = 0.001);

  ~GA_FLM();

  void search();

  void collect_ppl();
  float fitnessByBIC(float logprob, int complexity);
  float fitnessByInversePPL(float ppl);
  void create_factor_file(const vector<int>& indv, const string& basename);
  void create_new_generation();
  void generate_initialpop();
  bool termination_criteria_satisfied();

  int generation_number;

  // this func updates the fitness_lookup table and returns the fittness value
  float fitness_lookup(const vector<int>& indv);

	vector<vector<int> > oldpop, newpop;
	vector<float> fitness;
	
	vector<int> best_so_far;
	vector<int> best_so_far_prev;

	vector<string> fitness_table_str;
	float* fitness_table_fit;
	int fitness_table_count;
	float total_fitness;
	float bestsofar_fitness;
	float bestsofar_prev_fitness;
	float bestsofar_ppl;
	float bestsofar_prev_ppl;

  // vector<Chromosome> population;

	float convergence;

  void parse_arguments(int argc, char* argv[]);
  void parse_GA_PARAMS_file();
  void parse_FLM_PARAMS_file();

  map<string, float> fitness_table_map; // Key is a string representation of a gene. Value is the fitness of that gene sequence.
  map<string, float> ppl_table_map; // Key is a string representation of a gene. Value is the perplexity of that gene sequence.
  int number_of_genes_to_date;
  queue<string> genes_to_evaluate;
  queue<string> files_to_evaluate;

private:

  void do_selection();
  void do_crossover();
  void do_mutation();
  void compute_fitness_of_each_gene();
  void find_gene_with_best_fitness();
  float average_fitness() const;
  float weakest_fitness() const;
  bool has_converged() const;
  int weakest_index() const;
  void debug_algorithm() const;

};

#endif 
