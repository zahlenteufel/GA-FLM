#ifndef _FLM_CONF_H
#define _FLM_CONF_H

#include <iostream>
#include "util.h"

typedef vector<int> Chromosome;


class FLM_Conf {

public:

  FLM_Conf(string s);
  
  string make_fngram_command(const string& filename, const string& ga_path) const;

  void create_factor_file(const Chromosome& chromosome, const string& ga_path) const;
  
  void dump() const;

  /* Constants that are set by FLM PARAM file */
  string data_path;
  string trainset;
  string devset;
  string fngramcount_options;
  string fngram_options;
  string factor_to_predict;
  string factorstring;
  int chromosome_length;
  int smooth_len;
  int max_factors; // number of available factors total
  vector<string> factors;
  int max; // total length of the initial factor + backoff graph
  vector<string> discounts;
  int cutoff_max; // max number of min number of counts to be included in LM
  string fngramcount_path;
  string fngram_path;
  string default_discount;

private:

  void parse_FLM_PARAMS_file(const string& paramfile);

};

string to_string(const Chromosome& chromosome);

#endif
