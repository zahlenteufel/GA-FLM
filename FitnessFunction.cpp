#include "FitnessFunction.h"
#include <cmath>
/// BIC

float BIC::evaluate(Chromosome& c) {
  if (c.complexity() == 0) // Can this ever happen?
    return 0;

  float bic = -c.logprob() + k / 2 * log(float(c.complexity()));
  return fitness_scaling_constant / bic;
}

string BIC::name() const {
  return "BIC";
}

/// InversePPL



float InversePPL::evaluate(Chromosome& c) {
  return fitness_scaling_constant / c.perplexity();
}

string InversePPL::name() const {
  return "inversePPL";
}