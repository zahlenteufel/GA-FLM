#ifndef SELECTION_H
#define SELECTION_H

#include "GA_Operator.h"

class Selection : public GA_Operator {

public:
  
  Selection(const FLM_Conf& flm_conf) : GA_Operator(flm_conf) {};

  virtual Population do_selection(const Population& population, const vector<float>& fitness) = 0;

protected:

  vector<int> sample_with_repetition(int k, int size);

};

class Roulette : public Selection {

public:

  Roulette(const FLM_Conf& flm_conf) :
    Selection(flm_conf) {};

  virtual Population do_selection(const Population& population, const vector<float>& fitness);

};

class Tournament : public Selection {

public:

  Tournament(const FLM_Conf& flm_conf, int tournament_n) :
    Selection(flm_conf), tournament_n(tournament_n) {};

  virtual Population do_selection(const Population& population, const vector<float>& fitness);

private:

  int tournament_n;

};

class SUS : public Selection {

public:

  SUS(const FLM_Conf& flm_conf) :
    Selection(flm_conf) {};

  virtual Population do_selection(const Population& population, const vector<float>& fitness);

};

//
// Auxiliary Functions
//

void mean_and_standard_deviation(const vector<float>& v, float& mean, float& stddev);

vector<float> standardize(const vector<float>& v);

float sum(vector<float> v);

#endif
