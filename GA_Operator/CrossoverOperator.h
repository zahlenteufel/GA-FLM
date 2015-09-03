#ifndef CROSSOVER_OPERATOR_H
#define CROSSOVER_OPERATOR_H

#include "GA_Operator.h"
#include <vector>

using namespace std;

typedef std::vector<int> Chromosome;

class CrossoverOperator : public GA_Operator {

  CrossoverOperator(int chromosome_length, float crossover_probability) : 
    GA_Operator(chromosome_length),
    crossover_probability(crossover_probability) {};

  void do_crossover(const vector<Chromosome>&, vector<Chromosome>&); // TODO: do it inline

  virtual void do_crossover(Chromosome&, Chromosome&) = 0;

protected:

  float crossover_probability;

  bool decide_to_do_crossover();

  int pick_point();

  pair<int, int> pick_range();

};

class UniformCrossover : public CrossoverOperator {

  virtual void do_crossover(Chromosome&, Chromosome&) override;

};

class OnePointCrossover : public CrossoverOperator {

  virtual void do_crossover(Chromosome&, Chromosome&) override;

};

class TwoPointCrossover : public CrossoverOperator {

  virtual void do_crossover(Chromosome&, Chromosome&) override;

};

#endif
