#include "CrossoverOperator.h"
#include <algorithm>

void copy(const Chromosome& c1, Chromosome& c2) {
  copy(c1.begin(), c1.end(), c2.begin());
}

bool CrossoverOperator::decide_to_do_crossover() {
  return random_number_in_01() < crossover_probability;
}

// do crossover of pop1 and leave the result in pop2...
void CrossoverOperator::do_crossover(const vector<Chromosome>& pop1, vector<Chromosome>& pop2) {
  int population_size = pop1.size(), chromosome_length = pop1[0].size();
  for (int index = 0; index + 1 < population_size; index += 2) {
    for (int j = 0; j < chromosome_length; j++) {
      const Chromosome& new1 = pop1[index], new2 = pop1[index + 1];
      Chromosome& old1 = pop2[index], old2 = pop2[index + 1];
      copy(new1, old1);
      copy(new2, old2);
      if (decide_to_do_crossover())
        do_crossover(old1, old2);
    }
  }
}

void UniformCrossover::do_crossover(Chromosome& c1, Chromosome& c2) {
  int chromosome_length = c1.size();
  for (int j = 0; j < chromosome_length; j++)
    if (random_bit())
      swap(c1[j], c2[j]);
}

void OnePointCrossover::do_crossover(Chromosome& c1, Chromosome& c2) {
  int chromosome_length = c1.size();
  // swap tails
  for (int i = pick_point(); i < chromosome_length; i++)
    swap(c1[i], c2[i]);
}

void TwoPointCrossover::do_crossover(Chromosome& c1, Chromosome& c2) {
  pair<int, int> range = pick_range();
   // swap range
  for (int i = range.first; i <= range.second; i++)
    swap(c1[i], c2[i]);
}

int CrossoverOperator::pick_point() {
  return random_int(chromosome_length);
}

pair<int, int> CrossoverOperator::pick_range() {
  int i1 = pick_point(), i2 = pick_point();
  while (i1 == i2)
    i2 = pick_point();
  return make_pair(min(i1, i2), max(i1, i2));
}
