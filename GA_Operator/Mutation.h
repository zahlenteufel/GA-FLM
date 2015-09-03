#ifndef MUTATION_H
#define MUTATION_H

#include "GA_Operator.h"

class Mutation : GA_Operator {

public:

  Mutation(int chromosome_length, float probability_of_mutation) :
  	GA_Operator(chromosome_length), probability_of_mutation(probability_of_mutation) {};

  void do_mutation(vector<Chromosome>& population);

private:

  float probability_of_mutation;

  bool decide_to_mutate();

};

#endif
