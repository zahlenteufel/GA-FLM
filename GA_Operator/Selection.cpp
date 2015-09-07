#include "Selection.h"

Population Roulette::do_selection(const Population& population, const vector<float>& fitness) {
  Population new_population;
  vector<float> roulette_wheel(population.size());
  float total_fitness = sum(fitness);

  for (int i = 0; i < int(population.size()); i++) {
    roulette_wheel[i] = fitness[i] / total_fitness;
    if (i > 0)
      roulette_wheel[i] += roulette_wheel[i - 1];
  }

  while (new_population.size() < population.size()) {
    float roulette = random_number_in_01();
    for (int j = 0; j < int(population.size()); j++) {
      if (roulette <= roulette_wheel[j]) {
        new_population.push_back(population[j]);
        break;
      }
    }
  }
  return new_population;
}

Population Tournament::do_selection(const Population& population, const vector<float>& fitness) {
  Population new_population;

  while (new_population.size() < population.size()) {
    vector<int> random_indexes = sample_with_repetition(tournament_n, population.size());

    float best_index = 0;
    for (int i = 1; i < int(random_indexes.size()); i++)
      if (fitness[random_indexes[i]] > fitness[random_indexes[best_index]])
        best_index = i;
    new_population.push_back(population[random_indexes[best_index]]);
  }
  return new_population;
}

Population SUS::do_selection(const Population& population, const vector<float>& fitness) {
  vector<float> standard_fitness = standardize(fitness);
  Population new_population;
  float flindex = random_number_in_01();
  int picked_index = 1;
  for (int i = 0; i < int(population.size()) && picked_index <= int(population.size()); i++) {
    flindex += standard_fitness[i];
    while (picked_index < flindex) {
      if (picked_index <= int(population.size()))
        new_population.push_back(population[picked_index - 1]);
      picked_index++;
    }
  }
  return new_population;
}

//
// Auxiliary Functions
//

void mean_and_standard_deviation(const vector<float>& v, float& mean, float& stddev) {
  mean = sum(v) / v.size();
  
  float accum = 0.0;
  for (int i : v)
    accum += (i - mean) * (i - mean);

  stddev = sqrt(accum / (v.size() - 1));
}

vector<float> standardize(const vector<float>& v) {
  // TODO: check bibliography
  vector<float> linear_scaling;
  float mean, standard_deviation;
  
  mean_and_standard_deviation(v, mean, standard_deviation);

  for (float value : v)
    linear_scaling.push_back((value - mean) / standard_deviation);

  return linear_scaling;
}

vector<int> Selection::sample_with_repetition(int k, int size) {
  vector<int> result;
  for (int i = 0; i < k; i++)
    result.push_back(random_int(size));
  return result;
}

float sum(vector<float> v) {
  return accumulate(v.begin(), v.end(), 0);
}
