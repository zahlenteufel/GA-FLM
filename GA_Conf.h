#ifndef _GA_CONF_H
#define _GA_CONF_H

#include <string>
#include <fstream>
#include <iostream>
#include "util.h"

using namespace std;

constexpr int MAXPOP = 300;
constexpr int MAXGEN = 900;

class GA_Conf {
  
public:

  GA_Conf(string gaparamsfile, string seedfile);

  float compute_fitness(float ppl, float logprob, float complexity) const;

  void dump() const;

  string seedfile;
  int population_size;
  int maximum_number_of_generations;
  float crossover_probability;
  float mutation_probability;
  int fitness_scaling_constant;

  string crossover;
  string selection;
  int tournament_n;

  float terminatefitness;

  string ga_path;
  float bic_k;
  string fitness_func;
  string id;

private:

  void parse_GA_PARAMS_file(const string& gaparamsfile);

  float fitness_by_BIC(float logprob, int complexity) const;
  float fitness_by_inverse_ppl(float ppl) const;

};

#endif
