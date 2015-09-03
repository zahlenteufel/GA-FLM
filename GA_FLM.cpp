/*
 * "Copyright 2004, University of Washington. All Rights Reserved
 *
 *    Written by Kevin Duh and Sonia Parandekar
 *
 * NO WARRANTY
 * THE PROGRAM IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED INCLUDING, WITHOUT
 * LIMITATION, ANY WARRANTIES OR CONDITIONS OF TITLE, NON-INFRINGEMENT,
 * MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE. Each Recipient is
 * solely responsible for determining the appropriateness of using the Program
 * and assumes all risks associated with such use, including but not limited
 * to the risks and costs of program errors, compliance with applicable laws,
 * damage to or loss of data, programs or equipment, and unavailability or
 * interruption of operations.

 * DISCLAIMER OF LIABILITY
 * THE UNIVERSITY OF WASHINGTON, KEVIN DUH AND SONIA PARANDEKAR
 * SHALL NOT HAVE ANY LIABILITY FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING WITHOUT 
 * LIMITATION LOST PROFITS), HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE  OF
 * THE PROGRAM, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES."
*/

#include "GA_FLM.h"
#include "Gene.h"
#include "BackoffGraph.h"
#include "util.h"
#include <algorithm>

GA_FLM::GA_FLM(const GA_Conf& ga_conf, const FLM_Conf& flm_conf, float convergence_threshold) :
  ga_conf(ga_conf), flm_conf(flm_conf), convergence_threshold(convergence_threshold) {

  int population_size = ga_conf.population_size;
  int chromosome_length = flm_conf.chromosome_length;

  generation_number = 0;
  
  total_fitness = 0.0f;
  bestsofar_fitness = 0.0f;
  bestsofar_prev_fitness = 0.0f;
  bestsofar_ppl = 0.0f;
  number_of_genes_to_date = 1;

  fitness          = vector<float>(population_size, 0.0f);
  best_so_far      = vector<int>(chromosome_length, 0);
  best_so_far_prev = vector<int>(chromosome_length, 0);
  oldpop = vector<vector<int> >(population_size, vector<int>(chromosome_length, 0));
  newpop = vector<vector<int> >(population_size, vector<int>(chromosome_length, 0));

  ga_operator = new GA_Operator(*this);
}

GA_FLM::~GA_FLM() {
  delete ga_operator; // TODO: use smart pointer, or better: don't use pointer at all
}

void GA_FLM::search() {
  // TODO: look in the internet a good pseudocode
  generate_initialpop();
  compute_fitness_of_each_gene();
  while (!termination_criteria_satisfied()) {
    create_new_generation();
  }
  cerr << "Number of generations processed : " << generation_number << endl;
}

void execute_fngram_commands(queue<string>& fngram_commands) {
  // NOTE: I only have pmake parallel cluster implementation here. 
  // This section is specific to the computer platform in which ga-flm is run.
  // To write a new implementation for another computer platform, just use the fngram_commands as a starting point.
  // All necessary information is encapsulated in that data structure.
  // As long as one makes sure that all commands in fngram_commands are run in before this function exits,
  // no other part of the ga-flm code needs to be changed. 

  // Serial execution
  while (!fngram_commands.empty()) {
    string command = fngram_commands.front();
    fngram_commands.pop();
    sys(command);
  }
}

void GA_FLM::collect_ppl() {
  while (!files_to_evaluate.empty()) {
    string filename = files_to_evaluate.front();
    files_to_evaluate.pop();

    string indv_str = genes_to_evaluate.front();  
    genes_to_evaluate.pop();

    string evallog = ga_conf.ga_path + filename + ".EvalLog";
    ifstream evallogfile(evallog.c_str());

    float fitness;
    float ppl;
    float logprob;
    int complexity = 0;

    // TODO: avoid using sscanf
    
    if (evallogfile.good()) {
      string buffer;
      bool pplflag = false;
      bool logprobflag = false;
    
      while (evallogfile >> buffer) {
        if (pplflag) {
          sscanf(buffer.c_str(), "%f", &ppl);
          pplflag = false;
        }
        if (buffer.find("ppl=") != string::npos) {
          pplflag = true;
        }       
        if (logprobflag) {
          sscanf(buffer.c_str(), "%f", &logprob);
          logprobflag = false;
        }
        if (buffer.find("logprob=") != string::npos) {
          logprobflag = true;
        }       
      }
      
      string complexitylog = ga_conf.ga_path + filename + ".complexity";
      ifstream complexitylogfile(complexitylog.c_str());
      string complexitybuffer;
      

      complexitylogfile >> complexitybuffer;
      sscanf(complexitybuffer.c_str(), "%d", &complexity);
    }
    else {
      fitness = 0;
    }
    
    // TODO: use polymorphism
    
    fitness_table_map[indv_str] = fitness;
    ppl_table_map[indv_str] = ppl;
  }
}

void GA_FLM::create_factor_file(const vector<int>& indv, const string& basename) {
  flm_conf.create_factor_file(indv, basename, ga_conf.ga_path);
}

void GA_FLM::compute_fitness_of_each_gene() {
  queue<string> fngram_commands;
  for (int i = 0; i < ga_conf.population_size; i++) {
    string indv_str;
    for (int j = 0; j < flm_conf.chromosome_length; j++)
      indv_str += itos(oldpop[i][j]);

    map<string, float>::iterator it = fitness_table_map.find(indv_str);
    if (it == fitness_table_map.end()) {
      // indv_str is not found in map.
      // ... so it needs perplexity/fitness computations
      
      // add this gene to the map, with a dummy fitness value, which will be computed at a later step. 
      fitness_table_map[indv_str] = -2;
      
      int gene_id = number_of_genes_to_date + i;
      string basename = ga_conf.id + "ga" + itos(gene_id);
      cerr << basename << endl;
      
      create_factor_file(oldpop[i], basename);
      string command = flm_conf.make_fngram_command(basename, ga_conf.ga_path);
      fngram_commands.push(command);
        
      genes_to_evaluate.push(indv_str);
      files_to_evaluate.push(basename);
    }
  }

  execute_fngram_commands(fngram_commands);
  collect_ppl();
}

void GA_FLM::find_gene_with_best_fitness() {
  //save previous best_so_far fitness values and ppl values
  if (generation_number > 0) {
    if (bestsofar_fitness > bestsofar_prev_fitness) {
      for (int j = 0; j < flm_conf.chromosome_length; j++) 
        best_so_far_prev[j] = best_so_far[j];
      bestsofar_prev_fitness = bestsofar_fitness;
      bestsofar_prev_ppl = bestsofar_ppl;
      cerr << "bestsofar_prev_fitness: " << bestsofar_prev_fitness << "\n";
      cerr << "bestsofar_fitness: " << bestsofar_fitness << endl;
    }
  }

  for (int j = 0; j < flm_conf.chromosome_length; j++)
    best_so_far[j] = 0;

  bestsofar_fitness = 0;
  total_fitness = 0;
  // Read in fitness values for each gene of current population and find the best
  for (int i = 0; i < ga_conf.population_size; i++) {
    fitness[i] = fitness_lookup(oldpop[i]);
    total_fitness += fitness[i];
    if (fitness[i] > bestsofar_fitness) {
      bestsofar_fitness = fitness[i];
      for (int j = 0; j < flm_conf.chromosome_length; j++)
        best_so_far[j] = oldpop[i][j];
    }
  }

  // Find the corresponding perplexity of the best gene in current generation
  string bestsofar_str;
  for (int j = 0; j < flm_conf.chromosome_length; j++)
    bestsofar_str += itos(best_so_far[j]);
  bestsofar_ppl = ppl_table_map[bestsofar_str];
}

bool GA_FLM::has_converged() const {
  return (bestsofar_fitness - average_fitness()) < convergence_threshold;
}

void GA_FLM::debug_algorithm() const {
  cerr << "Generation : " << generation_number << "\n";
  cerr << "For this generation:\n";
  cerr << "Average fitness            = " << average_fitness() << "\n";
  cerr << "Best of this gen: " << bestsofar_fitness << "\n";
//   if (bestsofar_fitness > bestsofar_prev_fitness) {
//     cerr << "Fitness of best individual = " << bestsofar_fitness << ", PPL=" << bestsofar_ppl << "\n";
//     cerr << "Population convergence     = " << bestsofar_fitness - average_fitness << "\n";
//     cerr << "FITNESS OF BEST INDIVIDUAL = " << bestsofar_fitness << ", PPL= " << bestsofar_ppl << "\n";
//     cerr << "Best individual :\n";
//     for (int j = 0; j < flm_conf.chromosome_length; j++)
//       cerr << best_so_far[j];
//   }
//   else {
//     cerr << "Fitness of best individual = " << bestsofar_prev_fitness << ", PPL=" << bestsofar_prev_ppl << "\n";
//     cerr << "Population convergence     = " << bestsofar_prev_fitness - average_fitness << "\n";
//     cerr << "FITNESS OF BEST INDIVIDUAL = " << bestsofar_prev_fitness << ", PPL=" << bestsofar_prev_ppl << "\n";
//     cerr << "Best individual :\n";
//     for (int j = 0; j < flm_conf.chromosome_length; j++)
//       cerr << best_so_far_prev[j];
//   }
//   cerr << endl;
// }
}

float GA_FLM::average_fitness() const {
  double sum = accumulate(fitness.begin(), fitness.end(), 0.0);
  return  sum / fitness.size();
}

float GA_FLM::weakest_fitness() const {
  return *min_element(fitness.begin(), fitness.end());
}

int GA_FLM::weakest_index() const {
  return min_element(fitness.begin(), fitness.end()) - fitness.begin();
}

void GA_FLM::create_new_generation() {

  do_selection();

  ga_operator->shuffle_newpop();   // shuffle the newly selected population // TODO: what for??
  
  do_crossover();

  do_mutation();
  
  compute_fitness_of_each_gene();
  
  find_gene_with_best_fitness();

  // linear_scaling();
  
  generation_number++;
  number_of_genes_to_date += ga_conf.population_size; // TODO: this should go somewhere else

  debug_algorithm();
}

void GA_FLM::do_selection() {

  // TODO: use polymorphism

  if (ga_conf.selection == "roulette")
    ga_operator->do_roulette_selection(); 
  else if (ga_conf.selection == "tournament")
    ga_operator->do_tournament_selection();
  else // if (ga_conf.selection == "SUS")
    ga_operator->do_SUS_selection(weakest_fitness(), average_fitness());
}

void GA_FLM::do_crossover() {

  // TODO: use polymorphism

  if (ga_conf.crossover == "1-point")
    ga_operator->do_1_point_crossover();
  else if (ga_conf.crossover == "2-point")
    ga_operator->do_2_point_crossover();
  else // if (ga_conf.crossover == "uniform")
    ga_operator->do_uniform_crossover();
}

void GA_FLM::do_mutation() {
  ga_operator->mutate();

  //replace weakest member with the best individual from the previous generation
  ga_operator->be_elitist(); 
}


void GA_FLM::generate_initialpop() {
  // First, seed population by reading genes from file

  ifstream infile(ga_conf.seedfile.c_str());
  string buf;
  int seedcount = 0;

  // TODO: simply use >> to a string..

  while (getline(infile, buf, '\n')) {
    if (seedcount >= ga_conf.population_size) {
      break;
    }
    for (int j = 0; j < flm_conf.chromosome_length; j++) {
      const char* buf_char = buf.c_str();
      const char bit = buf_char[j];
      oldpop[seedcount][j] = atoi(&bit);
    }
    seedcount++;
  }

  // Second, initialize the remaining population randomly
  
  for (int i = seedcount; i < ga_conf.population_size; i++) { 
    // Initialize all bits for Initial Factors and Backoff Graph 
    int j;
    for (j = 0; j < (flm_conf.chromosome_length - flm_conf.smooth_len); j++)
      oldpop[i][j] = ga_operator->random_bit();
    // Initialize remaining portion of the gene with integers (for smoothing options) 
    int count = 0;
    for (; j < flm_conf.chromosome_length; j++) { 
      if (count % 2 == 0)
        // initialize discount method
        oldpop[i][j] = ga_operator->random_int(flm_conf.discounts.size());
      else
        // initialize cutoff
        oldpop[i][j] = ga_operator->random_int(flm_conf.cutoff_max); 
      count++;
    } 
  }

  cerr << "Initial population generated randomly." << endl;
}


bool GA_FLM::termination_criteria_satisfied() {
  string reason;
  if (bestsofar_fitness >= ga_conf.terminatefitness)
    reason = "desired fitness achieved";
  else if (has_converged())
    reason = "population has converged";
  else if (generation_number == ga_conf.maximum_number_of_generations)
    reason = "maximum number of generations reached";
  else
    return false;

  cerr << "terminating GA: " << reason << endl;
  return true;
}

float GA_FLM::fitness_lookup(const vector<int>& indv) {
  stringstream ss;
  // TODO: chromosome should be an object and 
  // we should ask the chromosome to give us its string representation.. 
  for (int i : indv)
    ss << i;
  string indv_str = ss.str();
  map<string, float>::iterator it = fitness_table_map.find(indv_str);
  if (it == fitness_table_map.end()) {
    cerr << "Error: Chromosome not in fitness lookup" << endl;
    exit(1);
    return 0;
  }
  else {
    return fitness_table_map[indv_str];
  }
}
