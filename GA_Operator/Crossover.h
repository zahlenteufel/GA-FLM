#ifndef CROSSOVER_H
#define CROSSOVER_H

#include "GA_Operator.h"
#include <vector>

using namespace std;

typedef vector<int> Chromosome;

class Crossover : public GA_Operator {

public:

  Crossover(int chromosome_length, float crossover_probability) : 
    GA_Operator(chromosome_length),
    crossover_probability(crossover_probability) {};

  void do_crossover(Population&);

  virtual void do_crossover(Chromosome&, Chromosome&) = 0;

protected:

  float crossover_probability;

  bool decide_to_do_crossover();

  int pick_point();

  pair<int, int> pick_range();

};

class Uniform : public Crossover {

public:

  Uniform(int chromosome_length, float crossover_probability) : 
    Crossover(chromosome_length, crossover_probability) {};

  void do_crossover(Chromosome&, Chromosome&) override;

};

class OnePoint : public Crossover {

public:

  OnePoint(int chromosome_length, float crossover_probability) : 
    Crossover(chromosome_length, crossover_probability) {};

  void do_crossover(Chromosome&, Chromosome&) override;

};

class TwoPoint : public Crossover {

public:

  TwoPoint(int chromosome_length, float crossover_probability) : 
    Crossover(chromosome_length, crossover_probability) {};

  void do_crossover(Chromosome&, Chromosome&) override;

};

#endif
