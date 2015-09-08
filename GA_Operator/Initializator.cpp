#include "Initializator.h"
#include <fstream>

bool all_zeros(string::iterator start, string::iterator end) {
  for (auto it = start; it != end; ++it) 
    if (*it != '0')
      return false;
  return true;
}

Chromosome Initializator::from_seed(const string& seed) {
  Chromosome result;
  for (char c : seed)
    result.push_back(c - '0');
  return result;
}

string Initializator::random_seed() {
  //
  // TODO: better format
  // !!! Assuming discounts_size < 10 and cutoff_max < 10 !!!
  //
  #define encode(i) char('0' + i)
  string s(flm_conf.chromosome_length, '0');

  // Initialize all bits for Initial Factors and Backoff Graph 
  int len1 = flm_conf.chromosome_length - flm_conf.smooth_len; 
  for (int i = 0; i < len1; i++)
    s[i] = encode(random_bit());

  // Ensure at least one activated factor in chromosome..
  if (all_zeros(s.begin(), s.begin() + flm_conf.factors.size())) {
    int i = random_int(flm_conf.factors.size());
    s[i] = '1';
  }

  // Initialize remaining portion of the gene with integers (for smoothing options) 
  for (int j = len1; j < flm_conf.chromosome_length; j += 2) { 
    // initialize discount method
    s[j] = encode(random_int(flm_conf.discounts.size()));
    // initialize cutoff
    s[j + 1] = encode(random_int(flm_conf.cutoff_max)); 
  }
  return s;
}

string Initializator::debug_seed(const string& seed) const {
  stringstream ss;
  int len1 = flm_conf.chromosome_length - flm_conf.smooth_len; 
  for (int i = 0; i < len1; i++)
    ss << seed[i];
  ss << "-";
  for (int j = len1; j < flm_conf.chromosome_length; j += 2) { 
    if (j > len1)
      ss << ".";
    ss << seed[j] << seed[j + 1];
  }
  return ss.str();
}

void Initializator::do_initialize(Population& population) {
  ifstream infile(seed_filename.c_str());

  if (!infile)
    throw "Invalid seedfile";

  vector<string> seeds;

  while (int(seeds.size()) < population_size) {
    string seed;
    infile >> seed;

    if (seed != "") {
      if (int(seed.size()) != flm_conf.chromosome_length)
        throw "Bad seed";
      // TODO: check seed format..
      seeds.push_back(seed);
    } else {
      seeds.push_back(random_seed());
    }
  }

  for (string seed : seeds)
    population.push_back(from_seed(seed)); 

  cerr << "Initial Population:\n";
  for (string seed : seeds)
    cerr << " " << debug_seed(seed) << "\n";
}
