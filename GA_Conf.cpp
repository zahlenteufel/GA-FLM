#include "GA_Conf.h"
#include "util.h"
#include <algorithm>
#include <list>

#define isIn(c, e) (find((c).begin(), (c).end(), e) != (c).end())

const list<string> selection_types = {"roulette", "tournament", "SUS"};
const list<string> crossover_types = {"1-point", "2-point", "uniform"};
const list<string> fitness_types = {"inversePPL", "BIC"};

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

  float crossover_probability = parse_float(extract_option(params_file));
  if (crossover_probability > 1) {
    cerr << "Error: Maximum allowable value of P(Crossover) is 1\n";
    cerr << "       Suggested value for P(Crossover) is > 0.6\n";
    cerr << "       Please modify the PARAMS file accordingly and try again\n";
    exit(1);
  }

  float mutation_probability = parse_float(extract_option(params_file));
  if (mutation_probability > 1) {
    cerr << "Error: Maximum allowable value of P(Mutation) is 1\n";
    cerr << "       Suggested value for P(Mutation) is < 0.1\n";
    cerr << "       Please modify the PARAMS file accordingly and try again\n";
    exit(1);
  }

  mutation = MutationOperator(chromosome_length, mutation_probability);

  string crossover_type = extract_enum("crossover", params_file, crossover_types);
  if (crossover_type == "1-point")
    crossover = OnePointCrossover(chromosome_length);
  else if (crossover_type == "2-point")
    crossover = TwoPointCrossover(chromosome_length);
  else if (crossover_type == "uniform")
    crossover = UniformCrossover(chromosome_length);
  
  string selection_type = extract_enum("selection", params_file, selection_types);
  tournament_n = parse_int(extract_option(params_file));
  if (selection_type == "roulette")
    selection = RouletteSelection(chromosome_length);
  else if (selection_type == "tournament")
    selection = TournamentSelection(chromosome_length, tournament_n);
  else if (selection_type == "SUS")
    selection = SUS_Selection(chromosome_length);
  
  float fitness_scaling_constant = parse_float(extract_option(params_file));
  string fitness_func = extract_enum("fitness", params_file, fitness_types);
  int bic_k = parse_int(extract_option(params_file));
  if (fitness_func == "inversePPL")
    fitness_function = InversePPL(fitness_scaling_constant);
  else if (fitness_func == "BIC")
    fitness_function = BIC(fitness_scaling_constant, bic_k);

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
    // << "P(crossover)        = " << crossover_probability << "\n"
    // << "P(mutation)         = " << mutation_probability << "\n"
    // << "crossover           = " << crossover << "\n"
    // << "selection           = " << selection << "\n";
  // if (selection != "tournament")
  //   cerr << "tournament_n        = (ignored)\n";
  // else
  //   cerr << "tournament_n        = " << tournament_n << "\n";
}

