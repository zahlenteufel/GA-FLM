#include "Crossover.h"
#include <algorithm>

void copy(const Chromosome& c1, Chromosome& c2) {
  copy(c1.begin(), c1.end(), c2.begin());
}

bool Crossover::decide_to_do_crossover() {
  return random_number_in_01() < crossover_probability;
}

// TODO: create a custom Crossover that uses a mix of these

void Crossover::do_crossover(Population& population) {
  shuffle(population.begin(), population.end(), rng);
  for (int index = 0; index + 1 < population.size(); index += 2)
    if (decide_to_do_crossover())
      do_crossover(population[index], population[index + 1]);
}

void Uniform::do_crossover(Chromosome& c1, Chromosome& c2) {
  for (int j = 0; j < flm_conf.chromosome_length; j++)
    if (random_bit())
      swap(c1[j], c2[j]);
}

void OnePoint::do_crossover(Chromosome& c1, Chromosome& c2) {
  // swap tails
  for (int i = pick_point(); i < flm_conf.chromosome_length; i++)
    swap(c1[i], c2[i]);
}

void TwoPoint::do_crossover(Chromosome& c1, Chromosome& c2) {
  pair<int, int> range = pick_range();
   // swap range
  for (int i = range.first; i <= range.second; i++)
    swap(c1[i], c2[i]);
}

int Crossover::pick_point() {
  return random_int(flm_conf.chromosome_length);
}

pair<int, int> Crossover::pick_range() {
  int i1 = pick_point(), i2 = pick_point();
  while (i1 == i2)
    i2 = pick_point();
  return make_pair(min(i1, i2), max(i1, i2));
}
