#include "Mutation.h"

void show_mutation(vector<bool>& v) {
  cerr << "Mutation: ";
  for (bool b : v)
    cerr << (b ? "X" : "_");
  cerr << endl;
}

int Mutation::perturb_smooth_option(int smooth_option) {
  int diff = random_bit() ? 1 : -1;
  smooth_option += diff;
  if (smooth_option == -1)
    smooth_option = 1;
  if (smooth_option > flm_conf.cutoff_max)
    smooth_option = flm_conf.cutoff_max - 1;
  return smooth_option;
}

void Mutation::do_mutation(Chromosome& c) {
  vector<bool> mutated(flm_conf.chromosome_length, false);
  for (int i = 0; i < flm_conf.chromosome_length; i++) {
    if (decide_to_mutate()) {
      mutated[i] = true;
      int beginning_smoothing_options = flm_conf.chromosome_length - flm_conf.smooth_len;
      if (i < beginning_smoothing_options) {
        c[i] = 1 - c[i];
      } else {
        int j = i - beginning_smoothing_options; 
        if (j % 2 == 0) // discount method
          c[i] = random_int(flm_conf.discounts.size());
        else // smooth option
          c[i] = perturb_smooth_option(c[i]);
      }
    }
  }
  show_mutation(mutated);
}

void Mutation::do_mutation(Population& population) {
  for (Chromosome& chromosome : population)
    do_mutation(chromosome);
}

bool Mutation::decide_to_mutate() {
  return random_number_in_01() < probability_of_mutation;
}
