#ifndef CROSSOVER_OPERATOR_H
#define CROSSOVER_OPERATOR_H

#include "GA_Operator.h"

typedef vector<int> Chromosome;

class CrossoverOperator : GA_Operator {

  CrossoverOperator(float crossover_probability) : crossover_probability(crossover_probability) {};

  void do_crossover(vector<Chromosome>& v1, vector<Chromosome>& v2); // TODO: do it inline

  // (How to perform crossover of two chromosomes)
  virtual void do_crossover(Chromosome&, Chromosome&) = 0;

private:

  float crossover_probability;

  bool decide_to_do_crossover();

  int pick_crosssite();

  pair<int, int> pick_2_crosssites() 

};

class UniformCrossover : CrossoverOperator {

  void do_crossover(Chromosome&, Chromosome&);

};

class OnePointCrossover : CrossoverOperator {

  void do_crossover(Chromosome&, Chromosome&)

};

class TwoPointCrossover : CrossoverOperator {

  void do_crossover(Chromosome&, Chromosome&)

};

#endif
