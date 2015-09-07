#ifndef INITIALIZATOR_H
#define INITIALIZATOR_H

#include "GA_Operator.h"

class Initializator : GA_Operator {

public:

  Initializator(const FLM_Conf& flm_conf, int population_size, const string& seed_filename) :
    GA_Operator(flm_conf), population_size(population_size), seed_filename(seed_filename)
    {};

  Population do_initialize();

private:

  Chromosome from_seed(const string& seed);
  string random_seed();

  const string seed_filename;
  int population_size;

};

#endif
