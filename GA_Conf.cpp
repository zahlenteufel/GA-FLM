#include "GA_Conf.h"
#include "util.h"
#include <algorithm>
#include <list>

#define isIn(c, e) (find((c).begin(), (c).end(), e) != (c).end())

list<string> selection_types = {"roulette", "tournament", "SUS"};
list<string> crossover_types = {"1-point", "2-point", "uniform"};
// TODO: add a fitness list? 

GA_Conf::GA_Conf(string gaparamfile, string seedfile) : seedfile(seedfile) {
  parse_GA_PARAMS_file(gaparamfile);
}

void GA_Conf::parse_GA_PARAMS_file(const string& gaparamfile) {
  ifstream params_file(gaparamfile.c_str());

  // Population size
  population_size = parse_int(extract_option(params_file));
  if (population_size >= MAXPOP) {
    cerr << "Error: Maximum allowable population size is " << MAXPOP << "\n";
    cerr << "       Please modify the PARAMS file accordingly and try again\n";
    exit(1);
  }

  // Max generation
  maximum_number_of_generations = parse_int(extract_option(params_file)); 
  if (maximum_number_of_generations >= MAXGEN) {
    cerr << "Error: Maximum allowable number of generations is " << MAXGEN << "\n";
    cerr << "       Please modify the PARAMS file accordingly and try again\n";
    exit(1);
  }

  crossover_probability = parse_float(extract_option(params_file));
  if (crossover_probability > 1) {
    cerr << "Error: Maximum allowable value of P(Crossover) is 1\n";
    cerr << "       Suggested value for P(Crossover) is > 0.6\n";
    cerr << "       Please modify the PARAMS file accordingly and try again\n";
    exit(1);
  }

  mutation_probability = parse_float(extract_option(params_file));
  if (mutation_probability > 1) {
    cerr << "Error: Maximum allowable value of P(Mutation) is 1\n";
    cerr << "       Suggested value for P(Mutation) is < 0.1\n";
    cerr << "       Please modify the PARAMS file accordingly and try again\n";
    exit(1);
  }

  crossover = extract_option(params_file);
  if (!isIn(crossover_types, crossover)) {
    cerr << "Error: Allowable options for Crossover Type are:\n";
    for (string crossver_type : crossover_types)
      cerr << crossver_type << "\n";
    cerr << "       Please modify the PARAMS file accordingly and try again\n";
    exit(1);
  }
  
  selection = extract_option(params_file);
  if (!isIn(selection_types, selection)) {
    cerr << "Error: Allowable options for Selection type are:\n";
    for (string selection_type : selection_types)
      cerr << selection_type << "\n";
    cerr << "       Please modify the PARAMS file accordingly and try again\n";
    exit(1);
  }

  tournament_n = parse_int(extract_option(params_file));
  
  fitness_scaling_constant = parse_float(extract_option(params_file));
  fitness_func = extract_option(params_file);
  bic_k = parse_int(extract_option(params_file));
  ga_path = extract_option(params_file);
  id = extract_option(params_file);

  //define a termination criterion
  terminatefitness = 1065; // why this number ???????? TODO: clarify, or parameterize it

  dump();
}

void GA_Conf::dump() const {
  cerr << "------------------------------------------------\n"
    << "Population size     = " << population_size << "\n"
    << "Maximum generations = " << maximum_number_of_generations << "\n"
    << "P(crossover)        = " << crossover_probability << "\n"
    << "P(mutation)         = " << mutation_probability << "\n"
    << "crossover           = " << crossover << "\n"
    << "selection           = " << selection << "\n";
  if (selection != "tournament")
    cerr << "tournament_n        = (ignored)\n";
  else
    cerr << "tournament_n        = " << tournament_n << "\n";
}

float GA_Conf::compute_fitness(float ppl, float logprob, float complexity) const {
  if (fitness_func == "BIC")
    return (complexity == 0) ? 0 : fitness_by_BIC(logprob, complexity);
  else if (fitness_func == "inversePPL")
    return fitness_by_inverse_ppl(ppl);
  
  cerr << "No proper fitness function specified" << endl;
  exit(1);
}

float GA_Conf::fitness_by_BIC(float logprob, int complexity) const {
  float bic = -logprob + bic_k / 2 * log(float(complexity));
  return fitness_scaling_constant / bic;
}

float GA_Conf::fitness_by_inverse_ppl(float ppl) const {
  return fitness_scaling_constant / ppl;
}
