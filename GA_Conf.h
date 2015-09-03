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
  int fitness_scaling_constant;
  
  CrossoverOperator crossover;
  SelectionOperator selection;
  MutationOperator mutation;

  FitnessFunction fitness_function;

  float terminatefitness;

  string ga_path;

  string id;

private:

  void parse_GA_PARAMS_file(const string& gaparamsfile);

};

#endif
