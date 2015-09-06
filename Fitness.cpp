#include "Fitness.h"
#include <cmath>


/// BIC


float BIC::evaluate(float logprob, float perplexity, float complexity) const {
  if (complexity == 0) // Can this ever happen?
    return 0;

  float bic = -logprob + k / 2 * log(float(complexity));
  return fitness_scaling_constant / bic;
}

string BIC::name() const {
  return "BIC";
}


/// InversePPL


float InversePPL::evaluate(float logprob, float perplexity, float complexity) const {
  return fitness_scaling_constant / perplexity;
}

string InversePPL::name() const {
  return "inversePPL";
}