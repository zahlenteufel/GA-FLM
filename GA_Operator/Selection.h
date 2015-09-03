#ifndef SELECTION_H
#define SELECTION_H

#include "GA_Operator.h"

class Selection : public GA_Operator {

public:
  
  Selection(int chromosome_length) : GA_Operator(chromosome_length) {};

  virtual Population do_selection(const Population& population) = 0;

};

class Roulette : public Selection {

  virtual Population do_selection(const Population& population) override;

};

class Tournament : public Selection {

  Tournament(int chromosome_length, int tournament_n) :
    Selection(chromosome_length), tournament_n(tournament_n) {};

  virtual Population do_selection(const Population& population) override;

private:

  int tournament_n;

};

class SUS : public Selection {

  virtual Population do_selection(const Population& population) override;

};

//
// Auxiliary Functions
//

float sum_fitness(const Population& population);

Population sample_with_repetition(int k, const Population& population);

bool compare_fitness(const Chromosome& c1, const Chromosome& c2);

void mean_and_standard_deviation(const vector<float>& v, float& mean, float& stddev);

vector<float> standardized_fitness(const Population& population);

#endif
