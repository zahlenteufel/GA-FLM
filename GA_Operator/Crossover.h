#ifndef CROSSOVER_H
#define CROSSOVER_H

#include "GA_Operator.h"
#include <vector>

using namespace std;

typedef vector<int> Chromosome;

class Crossover : public GA_Operator {

public:

  Crossover(const FLM_Conf& flm_conf, float crossover_probability) : 
    GA_Operator(flm_conf), crossover_probability(crossover_probability) {};

  void do_crossover(Population&);

  virtual vector<bool> do_crossover(Chromosome&, Chromosome&) = 0;

protected:

  float crossover_probability;

  bool decide_to_do_crossover();

  int pick_point(int _max = 1000);

  pair<int, int> pick_range(int _max = 1000);

};

class Hybrid : public Crossover {

public:

  Hybrid(const FLM_Conf& flm_conf, float crossover_probability) : 
    Crossover(flm_conf, crossover_probability) {};

  vector<bool> do_crossover(Chromosome&, Chromosome&) override;

};

class Uniform : public Crossover {

public:

  Uniform(const FLM_Conf& flm_conf, float crossover_probability) : 
    Crossover(flm_conf, crossover_probability) {};

  vector<bool> do_crossover(Chromosome&, Chromosome&) override;

};

class OnePoint : public Crossover {

public:

  OnePoint(const FLM_Conf& flm_conf, float crossover_probability) : 
    Crossover(flm_conf, crossover_probability) {};

  vector<bool> do_crossover(Chromosome&, Chromosome&) override;

};

class TwoPoint : public Crossover {

public:

  TwoPoint(const FLM_Conf& flm_conf, float crossover_probability) : 
    Crossover(flm_conf, crossover_probability) {};

  vector<bool> do_crossover(Chromosome&, Chromosome&) override;

};

#endif
