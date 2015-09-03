#ifndef FITNESS_H
#define FITNESS_H

#include "Chromosome.h"
#include <string>

using namespace std;

class Fitness {

public:

  virtual float evaluate(Chromosome& c) const = 0;

  virtual string name() const = 0;

};


class BIC : public Fitness {

public:

  BIC(float fitness_scaling_constant, int k) :
    fitness_scaling_constant(fitness_scaling_constant), k(k) {};

  virtual float evaluate(Chromosome& c) const overrides;

  virtual string name() const overrides;

private:

  float fitness_scaling_constant;

  int k;

};


class InversePPL : public Fitness {

public:

  InversePPL(float fitness_scaling_constant) :
    fitness_scaling_constant(fitness_scaling_constant) {};

  virtual float evaluate(Chromosome& c) const overrides;

  virtual string name() const overrides;

private:

  float fitness_scaling_constant;

};

#endif
