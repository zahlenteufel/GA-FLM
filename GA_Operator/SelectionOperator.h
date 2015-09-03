#ifndef SELECTION_OPERATOR_H
#define SELECTION_OPERATOR_H

#include "GA_Operator.h"

typedef vector<Chromosome> Population;

class SelectionOperator : public GA_Operator {

public:
  
  SelectionOperator(int chromosome_length) : GA_Operator(chromosome_length) {};

  virtual Population do_selection(const Population& population) = 0;

};

class RouletteSelection : public SelectionOperator {

  virtual Population do_selection(const Population& population) override;

};

class TournamentSelection : public SelectionOperator {

  TournamentSelection(int chromosome_length, int tournament_n) :
    SelectionOperator(chromosome_length), tournament_n(tournament_n) {};

  virtual Population do_selection(const Population& population) override;

private:

  int tournament_n;

};

class SUS_Selection : public SelectionOperator {

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
