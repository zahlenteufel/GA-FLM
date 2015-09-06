#ifndef FITNESS_H
#define FITNESS_H

#include <string>

using namespace std;


class Fitness {

public:

  virtual float evaluate(float logprob, float perplexity, float complexity) const = 0;

  virtual string name() const = 0;

};


class BIC : public Fitness {

public:

  BIC(float fitness_scaling_constant, int k) :
    fitness_scaling_constant(fitness_scaling_constant), k(k) {};

  float evaluate(float logprob, float perplexity, float complexity) const override;

  string name() const override;

private:

  float fitness_scaling_constant;

  int k;

};


class InversePPL : public Fitness {

public:

  InversePPL(float fitness_scaling_constant) :
    fitness_scaling_constant(fitness_scaling_constant) {};

  float evaluate(float logprob, float perplexity, float complexity) const override;

  string name() const override;

private:

  float fitness_scaling_constant;

};

#endif
