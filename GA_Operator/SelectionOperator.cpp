#include "SelectionOperator.h"

Population RouletteSelection::do_selection(const Population& population) {
  Population new_population;
  vector<float> roulette_wheel(population.size());
  float total_fitness = sum_fitness(population);

  for (int i = 0; i < int(population.size()); i++) {
    roulette_wheel[i] = population[i].fitness() / total_fitness;
    if (i > 0)
      roulette_wheel[i] += roulette_wheel[i - 1];
  }

  while (new_population.size() < population.size()) {
    float roulette = random_number_in_01();
    for (int j = 0; j < int(population.size()); j++) {
      if (roulette <= roulette_wheel[j]) {
        new_population.append(population[j]);
        break;
      }
    }
  }
  return new_population;
}

Population TournamentSelection::do_selection(const Population& population) {
  Population new_population;

  while (new_population.size() < original_population.size()) {
    Population nrandom = sample_with_repetition(tournament_n, population);
    Chromosome fittest = *max_element(nrandom.begin(), nrandom.end(), compare_fitness);
    new_population.append(fittest);
  }
  return new_population;
}

Population SUS_Selection::do_selection(const Population& population) {
  vector<float> standard_fitness = standardized_fitness(population);
  float flindex = random_number_in_01();
  int picked_index = 1;
  for (int i = 0; i < int(population.size()) && picked_index <= int(population.size()); i++) {
    flindex += standard_fitness[i];
    while (picked_index < flindex) {
      if (picked_index <= population.size())
        new_population.append(population[picked_index - 1]);
      picked_index++;
    }
  }
  return new_population;
}

//
// Auxiliary Functions
//

float sum_fitness(const Population& population) {
  float sum = 0;
  for (auto& chromosome : population)
    sum += chromosome.fitness();
  return sum;
}

void mean_and_standard_deviation(const vector<float>& v, float& mean, float& stddev) {
  mean = accumulate(v.begin(), v.end(), 0.0) / v.size();
  
  float accum = 0.0;
  for (int i : v)
    accum += (i - mean) * (i - meam);

  stddev = sqrt(accum / (v.size() - 1));
}

vector<float> standardized_fitness(const Population& population) {
  // TODO: check bibliography
  vector<float> fitness;
  float mean, standard_deviation;
  for (auto chromosome& : population)
    fitness.push_back(chromosome.fitness());
  
  mean_and_standard_deviation(fitness, mean, stddev);

  for (float fitness_value : fitness)
    linear_scaling.push_back((fitness_value - mean) / standard_deviation);

  return linear_scaling;
}

Population sample_with_repetition(int k, const Population& population) {
  Population result;
  for (int i = 0; i < k; i++)
    result.append(population[randint(population.size())]);
  return result;
}

bool compare_fitness(const Chromosome& c1, const Chromosome& c2) {
  return c1.fitness() < c2.fitness();
}