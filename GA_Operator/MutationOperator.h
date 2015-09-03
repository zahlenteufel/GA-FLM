#ifndef MUTATION_OPERATOR_H
#define MUTATION_OPERATOR_H

include "GA_Operator.h"

class MutationOperator : GA_Operator {

public:

  MutationOperator(float probability_of_mutation) : probability_of_mutation(probability_of_mutation) {};

  void do_mutation(vector<Chromosome>& population);

private:

  float probability_of_mutation;

};

#endif
