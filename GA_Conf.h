#ifndef GA_CONF_H
#define GA_CONF_H

#include <string>
#include <fstream>
#include <iostream>
#include "util.h"
#include "GA_Operator/Crossover.h"
#include "GA_Operator/Mutation.h"
#include "GA_Operator/Selection.h"
#include "GA_Operator/Initializator.h"
#include "FLM_Conf.h"
#include "Fitness.h"

using namespace std;

constexpr int MAXPOP = 300;
constexpr int MAXGEN = 900;

class GA_Conf {
  
public:

  GA_Conf(const FLM_Conf& flm_conf, const string& gaparamfile, const string& seedfile);

  ~GA_Conf();

  float compute_fitness(float ppl, float logprob, float complexity) const;

  void dump() const;

  int chromosome_length;
  int population_size;
  int maximum_number_of_generations;
  
  Crossover* crossover;
  Selection* selection;
  Mutation* mutation;
  Initializator* initializator;

  Fitness* fitness_function;

  float terminatefitness;

  string ga_path;

  string id;

private:

  void parse_GA_PARAMS_file(const string& gaparamsfile);

};

#endif
