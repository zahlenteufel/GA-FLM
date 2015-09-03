#ifndef CROSSOVER_OPERATOR_H
#define CROSSOVER_OPERATOR_H

#include "GA_Operator.h"
#include <vector>

using namespace std;

typedef std::vector<int> Chromosome;

class Crossover : public GA_Operator {

  Crossover(int chromosome_length, float crossover_probability) : 
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

class Uniform : public Crossover {

  virtual void do_crossover(Chromosome&, Chromosome&) override;

};

class OnePoint : public Crossover {

  virtual void do_crossover(Chromosome&, Chromosome&) override;

};

class TwoPoint : public Crossover {

  virtual void do_crossover(Chromosome&, Chromosome&) override;

};

#endif
