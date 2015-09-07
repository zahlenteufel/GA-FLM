#include "Fitness.h"
#include <cmath>


/// BIC

float BIC::evaluate(float logprob, float perplexity, int complexity) const {
  float bic = -logprob + k / 2 * log(complexity);
  return fitness_scaling_constant / bic;
}

/// InversePPL

float InversePPL::evaluate(float logprob, float perplexity, int complexity) const {
  return fitness_scaling_constant / perplexity;
}
