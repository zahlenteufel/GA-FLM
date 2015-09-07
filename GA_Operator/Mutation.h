#ifndef MUTATION_H
#define MUTATION_H

#include "GA_Operator.h"

class Mutation : GA_Operator {

public:

  Mutation(const FLM_Conf& flm_conf, float probability_of_mutation) :
  	GA_Operator(flm_conf), probability_of_mutation(probability_of_mutation) {};

  void do_mutation(Population& population);

private:

  void do_mutation(Chromosome& c);

  int perturb_smooth_option(int smooth_option);

  float probability_of_mutation;

  bool decide_to_mutate();

};

#endif
