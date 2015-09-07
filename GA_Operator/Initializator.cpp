#include "Initializator.h"
#include <fstream>

Initializator::Initializator(int chromosome_length, int population_size, const string& seed_filename, 
    int smooth_len, int discounts_size, int cutoff_max) :
  GA_Operator(chromosome_length), population_size(population_size), seed_filename(seed_filename),
  smooth_len(smooth_len), discounts_size(discounts_size), cutoff_max(cutoff_max) 
  {};

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
  string s(chromosome_length, '0');
  // Initialize all bits for Initial Factors and Backoff Graph 
  int len1 = chromosome_length - smooth_len; 
  for (int i = 0; i < len1; i++)
    s[i] = encode(random_bit());
  // Initialize remaining portion of the gene with integers (for smoothing options) 
  for (int j = len1; j < chromosome_length; j += 2) { 
    // initialize discount method
    s[j] = encode(random_int(discounts_size));
    // initialize cutoff
    s[j + 1] = encode(random_int(cutoff_max)); 
  }
  return s;
}

Population Initializator::do_initialize() {
  Population result;
  ifstream infile(seed_filename.c_str());

  if (!infile)
    throw "Invalid seedfile";

  
  int seed_count = 0;

  vector<string> seeds;

  while (seed_count < population_size) {
    string seed;
    infile >> seed;
    if (seed.size() != chromosome_length)
      throw "Bad seed";
    // TODO: check seed format..
    result.push_back(from_seed(seed));
    seed_count++;
  }
  while (seeds.size() < population_size)
    seeds.push_back(random_seed());

  for (string seed : seeds)
    result.push_back(from_seed(seed)); 

  return result;
}
