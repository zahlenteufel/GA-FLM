#include "Mutation.h"

void Mutation::do_mutation(vector<Chromosome>& population) {
  // TODO: how to mutate non-boolean genes!!??
  for (Chromosome& chromosome : population)
    for (int& gene : chromosome)
      if (decide_to_mutate())
        gene = 1 - gene;	
}

bool Mutation::decide_to_mutate() {
  return random_number_in_01() < probability_of_mutation;
}
