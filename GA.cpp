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

#include "GA.h"
#include "Representation/Gene.h"
#include "Representation/BackoffGraph.h"
#include "util.h"
#include <algorithm>

#define isIn(c, e) (find((c).begin(), (c).end(), (e)) != (c).end())
#define open(s, f) ifstream f((s).c_str())

GA::GA(const GA_Conf& ga_conf, const FLM_Conf& flm_conf, float convergence_threshold) :
  ga_conf(ga_conf), flm_conf(flm_conf) {

  convergence_threshold = convergence_threshold;
}

void GA::search() {
  do {
    create_new_generation();
  } while (!termination_criteria_satisfied());

  cerr << "Number of generations processed : " << generation_number << endl;
}

GA::info GA::parse_info(const string& evallog_filename, const string& complexity_filename) const {
  GA::info chromosome_info;
  open(evallog_filename, eval_logfile);
  
  if (!eval_logfile.good())
    throw "Bad .EvalLog";

  string buffer;
  bool pplflag = false, logprobflag = false;
    
  while (eval_logfile >> buffer) {
    if (pplflag) {
      chromosome_info.perplexity = parse_float(buffer);
      pplflag = false;
    }
    if (buffer.find("ppl=") != string::npos)
      pplflag = true;

    if (logprobflag) {
      chromosome_info.logprob = parse_float(buffer);
      logprobflag = false;
    }
    if (buffer.find("logprob=") != string::npos)
      logprobflag = true;
  }
      
  open(complexity_filename, complexity_logfile);

  if (!complexity_logfile.good())
    throw "Bad .complexity";

  complexity_logfile >> chromosome_info.complexity;

  return chromosome_info;
}

GA::info GA::evaluate(const Chromosome& chromosome) const {
  string chromosome_str = to_string(chromosome);
  auto it = cache.find(chromosome_str);
  if (it != cache.end()) {
    return it->second;
  }

  create_factor_file(chromosome);
  sys(flm_conf.make_fngram_command(chromosome_str, ga_conf.ga_path));

  info chromosome_info = parse_info(
    ga_conf.ga_path + chromosome_str + ".EvalLog",
    ga_conf.ga_path + chromosome_str + ".complexity"
  );
  
  chromosome_info.fitness = ga_conf.fitness_function->evaluate(
    chromosome_info.logprob, chromosome_info.perplexity, chromosome_info.complexity);
  
  return cache[chromosome_str] = chromosome_info;
}

void GA::create_factor_file(const Chromosome& chromosome) const {
  flm_conf.create_factor_file(chromosome, ga_conf.ga_path);
}

void GA::compute_fitness_of_each_gene() {
  for (int i = 0; i < int(population.size()); i++) {
    fitness[i] = evaluate(population[i]).fitness;
    // evaluate() calls fngram and all that stuff if necessary..
    cerr << "fitness[" << i << "] = " << fitness[i] << endl;
    if (fitness[i] > historic_best_fitness) {
      historic_best = population[i];
      historic_best_fitness = fitness[i];
    }
  }
}

bool GA::has_converged() const {
  return (historic_best_fitness - average_fitness()) < convergence_threshold;
}

void GA::debug_algorithm() const {
  cerr << "Generation : " << generation_number << endl;
  cerr << " Best Fitness: " << historic_best_fitness << endl;
  cerr << " Best Chromosome: " << to_string(historic_best) << endl;
}

float GA::average_fitness() const {
  return accumulate(fitness.begin(), fitness.end(), 0.0) / fitness.size();
}

void GA::create_new_generation() {
  if (population.empty()) {
    create_initial_population();
  } else {
    ga_conf.selection->do_selection(population, fitness);
    ga_conf.crossover->do_crossover(population);
    ga_conf.mutation->do_mutation(population);
    // mutation.be_elitist();
  }

  compute_fitness_of_each_gene();  
  // linear_scaling();
  generation_number++;
  debug_algorithm();
}


void GA::create_initial_population() {
  generation_number = 0;
  ga_conf.initializator->do_initialize(population);
  fitness = vector<float>(population.size());
}


bool GA::termination_criteria_satisfied() const {
  string reason;
  if (historic_best_fitness >= ga_conf.terminate_fitness)
    reason = "desired fitness achieved";
  else if (has_converged())
    reason = "population has converged";
  else if (generation_number == ga_conf.maximum_number_of_generations)
    reason = "maximum number of generations reached";
  else
    return false;

  cerr << "Search Finished: " << reason << endl;
  return true;
}
