#include "Crossover.h"
#include <algorithm>

bool Crossover::decide_to_do_crossover() {
  return random_number_in_01() < crossover_probability;
}

void show_crossing(vector<bool>& v) {
  cerr << "Crossing: ";
  for (bool b : v)
    cerr << (b ? "X" : "_");
  cerr << endl;
}

void Crossover::do_crossover(Population& population) {
  shuffle(population.begin(), population.end(), rng);
  for (int index = 0; index + 1 < int(population.size()); index += 2)
    if (decide_to_do_crossover()) {
      auto crossed = do_crossover(population[index], population[index + 1]);
      show_crossing(crossed);
    }
}

vector<bool> Hybrid::do_crossover(Chromosome& c1, Chromosome& c2) {
  vector<bool> crossed(flm_conf.chromosome_length, false);
  // Do two point crossover in first part
  int smoothing_beginning = flm_conf.chromosome_length - flm_conf.smooth_len;
  pair<int, int> range = pick_range(smoothing_beginning);
  for (int i = range.first; i < range.second; i++) {
    swap(c1[i], c2[i]);
    crossed[i] = true;
  }
  // Do uniform crossover in the smoothing part
  for (int j = smoothing_beginning; j < flm_conf.chromosome_length; j++) {
    if (random_bit()) {
      swap(c1[j], c2[j]);
      crossed[j] = true;
    }
  }
  return crossed;
}

vector<bool> Uniform::do_crossover(Chromosome& c1, Chromosome& c2) {
  vector<bool> crossed(flm_conf.chromosome_length, false);
  for (int j = 0; j < flm_conf.chromosome_length; j++) {
    if (random_bit()) {
      swap(c1[j], c2[j]);
      crossed[j] = true;
    }
  }
  return crossed;
}

vector<bool> OnePoint::do_crossover(Chromosome& c1, Chromosome& c2) {
  vector<bool> crossed(flm_conf.chromosome_length, false);
  // swap tails
  for (int i = pick_point(); i < flm_conf.chromosome_length; i++) {
    swap(c1[i], c2[i]);
    crossed[i] = true;
  }
  return crossed;
}

vector<bool> TwoPoint::do_crossover(Chromosome& c1, Chromosome& c2) {
  vector<bool> crossed(flm_conf.chromosome_length, false);
  pair<int, int> range = pick_range();
   // swap range
  for (int i = range.first; i <= range.second; i++) {
    swap(c1[i], c2[i]);
    crossed[i] = true;
  }
  return crossed;
}

int Crossover::pick_point(int _max /*= 1000*/) {
  return random_int(min(_max, flm_conf.chromosome_length));
}

pair<int, int> Crossover::pick_range(int _max /*= 1000*/) {
  int i1 = pick_point(_max), i2 = pick_point(_max);
  while (i1 == i2)
    i2 = pick_point(_max);
  return make_pair(min(i1, i2), max(i1, i2));
}
