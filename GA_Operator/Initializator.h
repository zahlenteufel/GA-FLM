#ifndef INITIALIZATOR_H
#define INITIALIZATOR_H

#include "GA_Operator.h"

class Initializator : GA_Operator {

public:

  Initializator(const FLM_Conf& flm_conf, int population_size, const string& seed_filename) :
    GA_Operator(flm_conf), population_size(population_size), seed_filename(seed_filename)
    {};

  void do_initialize(Population&);

private:

  string debug_seed(const string& seed) const;
  Chromosome from_seed(const string& seed);
  string random_seed();

  int population_size;
  const string seed_filename;

};

#endif
