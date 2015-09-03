#ifndef MUTATION_OPERATOR_H
#define MUTATION_OPERATOR_H

#include "GA_Operator.h"

class MutationOperator : GA_Operator {

public:

  MutationOperator(int chromosome_length, float probability_of_mutation) :
  	GA_Operator(chromosome_length), probability_of_mutation(probability_of_mutation) {};

  void do_mutation(vector<Chromosome>& population);

private:

  float probability_of_mutation;

  bool decide_to_mutate();

};

#endif
