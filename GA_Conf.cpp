#include "GA_Conf.h"
#include "util.h"
#include <algorithm>
#include <list>

GA_Conf::GA_Conf(const FLM_Conf& flm_conf, const string& gaparamfile, const string& seed_filename) {
  crossover = nullptr;
  selection = nullptr;
  mutation = nullptr;
  fitness_function = nullptr;
  parse_GA_PARAMS_file(flm_conf, gaparamfile);
  initializator = new Initializator(flm_conf, population_size, seed_filename);
}

GA_Conf::~GA_Conf() {
  delete crossover;
  delete selection;
  delete mutation;
  delete fitness_function;
  delete initializator;
}

void GA_Conf::parse_GA_PARAMS_file(const FLM_Conf& flm_conf, const string& gaparamfile) {
  ifstream params_file(gaparamfile.c_str());

  population_size = parse_int(extract_option(params_file));
  if (population_size >= MAXPOP) {
    cerr << "Error: Maximum allowable population size is " << MAXPOP << "\n";
    cerr << "       Please modify the PARAMS file accordingly and try again\n";
    exit(1);
  }

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

  mutation = new Mutation(flm_conf, mutation_probability);

  string crossover_type = extract_option(params_file);

  cout << crossover_type << endl;

  if (crossover_type == "1-point")
    crossover = new OnePoint(flm_conf, crossover_probability);
  else if (crossover_type == "2-point")
    crossover = new TwoPoint(flm_conf, crossover_probability);
  else if (crossover_type == "uniform")
    crossover = new Uniform(flm_conf, crossover_probability);
  else if (crossover_type == "hybrid")
    crossover = new Hybrid(flm_conf, crossover_probability);
  else
    throw "Invalid crossover";
  
  string selection_type = extract_option(params_file);
  int tournament_n = parse_int(extract_option(params_file));
  if (selection_type == "roulette")
    selection = new Roulette(flm_conf);
  else if (selection_type == "tournament")
    selection = new Tournament(flm_conf, tournament_n);
  else if (selection_type == "SUS")
    selection = new SUS(flm_conf);
  else
    throw "Invalid selection";
  
  float fitness_scaling_constant = parse_float(extract_option(params_file));
  string fitness_func = extract_option(params_file);
  int bic_k = parse_int(extract_option(params_file));
  if (fitness_func == "inversePPL")
    fitness_function = new InversePPL(fitness_scaling_constant);
  else if (fitness_func == "BIC")
    fitness_function = new BIC(fitness_scaling_constant, bic_k);
  else
    throw "Invalid fitness function";

  ga_path = extract_option(params_file);
  id = extract_option(params_file);

  //define a termination criterion
  terminate_fitness = 1065; // why this number ??? TODO: clarify, or parameterize it

  dump();
}

void GA_Conf::dump() const {
  cerr << "------------------------------------------------\n"
    << "Population size     = " << population_size << "\n"
    << "Maximum generations = " << maximum_number_of_generations << "\n";
    // << "P(crossover)        = " << crossover_probability << "\n"
    // << "P(mutation)         = " << mutation_probability << "\n"
    // << "crossover           = " << crossover << "\n"
    // << "selection           = " << selection << "\n";
  // if (selection != "tournament")
  //   cerr << "tournament_n        = (ignored)\n";
  // else
  //   cerr << "tournament_n        = " << tournament_n << "\n";
}

