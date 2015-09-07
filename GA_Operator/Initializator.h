#ifndef INITIALIZATOR_H
#define INITIALIZATOR_H

#include "GA_Operator.h"

class Initializator : GA_Operator {

public:

  Initializator(
  	int chromosome_length,
  	int population_size,
  	const string& seed_filename,
  	int smooth_len,
  	int discounts_size,
  	int cutoff_max
  );

  Population do_initialize();

private:

  Chromosome from_seed(const string& seed);
  string random_seed();

  int population_size, smooth_len, discounts_size, cutoff_max;
  const string& seed_filename;

};

#endif
