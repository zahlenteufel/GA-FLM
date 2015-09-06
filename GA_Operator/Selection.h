#ifndef SELECTION_H
#define SELECTION_H

#include "GA_Operator.h"

class Selection : public GA_Operator {

public:
  
  Selection(int chromosome_length) : GA_Operator(chromosome_length) {};

  virtual Population do_selection(const Population& population, const vector<float>& fitness) = 0;

protected:

  vector<int> sample_with_repetition(int k, int size);

};

class Roulette : public Selection {

public:

  Roulette(int chromosome_length) :
    Selection(chromosome_length) {};

  virtual Population do_selection(const Population& population, const vector<float>& fitness) override;

};

class Tournament : public Selection {

public:

  Tournament(int chromosome_length, int tournament_n) :
    Selection(chromosome_length), tournament_n(tournament_n) {};

  virtual Population do_selection(const Population& population, const vector<float>& fitness) override;

private:

  int tournament_n;

};

class SUS : public Selection {

public:

  SUS(int chromosome_length) :
    Selection(chromosome_length) {};

  virtual Population do_selection(const Population& population, const vector<float>& fitness) override;

};

//
// Auxiliary Functions
//

void mean_and_standard_deviation(const vector<float>& v, float& mean, float& stddev);

vector<float> standardize(const vector<float>& v);

float sum(vector<float> v);

#endif
