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

/*-
 * GA_Operator.cpp
 *     Functions that implement genetic algorithm operators: 
 *     selection, crossover, and mutation
 */

#include "GA_Operator.h"
#include "../ga-flm.h"
#include "../util.h"
#include <cmath>
#include <ctime>
#include <random>
#include <chrono>

default_random_engine rng;

GA_Operator::GA_Operator(GA_FLM& ga_flm) : ga_flm(ga_flm) {
  unsigned seed = chrono::system_clock::now().time_since_epoch().count();
  cerr << "random seed: " << seed << endl; 
  rng = default_random_engine(seed);
}

//return a random number between 0 and 1
float GA_Operator::random_number_in_01() {
  uniform_real_distribution<float> rd(0, 1);
  return rd(rng);
}

int GA_Operator::random_bit() {
  bernoulli_distribution distribution(0.5);
  return distribution(rng);
}

int GA_Operator::random_int(int max) {
  uniform_int_distribution<int> distribution(0, max - 1);
  return distribution(rng);
}

void GA_Operator::do_roulette_selection() {
  int population_size = ga_flm.ga_conf.population_size;
  vector<float> roulette_wheel(population_size);

  // TODO: use stl algorithm accumulate

  for (int i = 0; i < population_size; i++) {
    roulette_wheel[i] = ga_flm.fitness[i] / ga_flm.total_fitness;
    if (i > 0)
      roulette_wheel[i] += roulette_wheel[i - 1];
  }

  for (int i = 0; i < population_size; i++) {
    float roulette = random_number_in_01();
    for (int j = 0; j < population_size; j++) {
      if (roulette <= roulette_wheel[j]) {
        for (int k = 0; k < ga_flm.flm_conf.chromosome_length; k++)
          ga_flm.newpop[i][k] = ga_flm.oldpop[j][k];
        break;
      }
    }
  }
}

void GA_Operator::do_tournament_selection() {
  vector<vector<int>> nrandom(ga_flm.ga_conf.tournament_n, vector<int>(ga_flm.flm_conf.chromosome_length, 0));

  //Conduct ga_flm.ga_conf.population_size tournaments:
  for (int i = 0; i < ga_flm.ga_conf.population_size; i++) {
    //select tournament_n individuals randomly
    for (int j = 0; j < ga_flm.ga_conf.tournament_n; j++) {
      int index = random_int(ga_flm.ga_conf.population_size);
      for (int k = 0; k < ga_flm.flm_conf.chromosome_length; k++)
        nrandom[j][k] = ga_flm.oldpop[index][k];
    }

    int fittest_index = 0;
    float current_fitness = 0, best_fitness = 0;
    //find fitness of the tournament_n individuals and hence find the fittest
    current_fitness = 0; best_fitness = 0; fittest_index = 0;
    for (int j = 0; j < ga_flm.ga_conf.tournament_n; j++) {
      current_fitness = ga_flm.fitness_lookup(nrandom[j]);
      if (current_fitness >= best_fitness) {
        best_fitness = current_fitness;
        fittest_index = j;
      }
    }
    
    // copy fittest_index individual into newpop
    for (int k = 0; k < ga_flm.flm_conf.chromosome_length; k++)
      ga_flm.newpop[i][k] = nrandom[fittest_index][k];
  }
} 

void GA_Operator::do_SUS_selection(float weakest_fitness, float average_fitness) {
  float scale_a = 1, scale_b = 0;
  const float expoff_max = 2.0f;
  vector<float> lin_scaling(ga_flm.ga_conf.population_size, 0.0f);
  
  // f(min) non-negative test:
  if (weakest_fitness > (expoff_max * average_fitness - ga_flm.bestsofar_fitness) / (expoff_max - 1.0)) {
    float difference = ga_flm.bestsofar_fitness - average_fitness;
    if (difference != 0) {
      scale_a = (expoff_max - 1.0) * average_fitness / difference;
      scale_b = average_fitness * (ga_flm.bestsofar_fitness - expoff_max * average_fitness) / difference;
    }
  }
  else {
    float difference = average_fitness - weakest_fitness;
    if (difference != 0) {
      scale_a = average_fitness / difference;
      scale_b = -weakest_fitness * average_fitness / difference;
    }
  }
   
  for (int i = 0; i < ga_flm.ga_conf.population_size; i++)
    lin_scaling[i] = (scale_a*ga_flm.fitness[i] + scale_b) / average_fitness; // expected offspring(s)

  // selection:
  float flindex = random_number_in_01();
  int picked_index = 1;
  for (int i = 0; i < ga_flm.ga_conf.population_size && picked_index <= ga_flm.ga_conf.population_size; i++) {
    flindex += lin_scaling[i];
    while (float(picked_index) < flindex) {
      if (picked_index <= ga_flm.ga_conf.population_size)
        for (int j = 0; j < ga_flm.flm_conf.chromosome_length; j++)
          ga_flm.newpop[picked_index - 1][j] = ga_flm.oldpop[i][j];
      picked_index++;
    }
  }
}

void GA_Operator::shuffle_newpop() {
  shuffle(ga_flm.newpop.begin(), ga_flm.newpop.end(), rng);
}
  
void GA_Operator::be_elitist() {
  int weakest_index = 0, flag = 0;
  int bestsofar_dec = 0, oldpop_dec = 0;
  float weakest_fitness = 0;

  cerr << "Elitist: gen_number " << ga_flm.generation_number << "\n";
  cerr << "fitness bestsofar_prev=" << ga_flm.bestsofar_prev_fitness << "\n";
  cerr << "fitness bestof thisgen = " << ga_flm.bestsofar_fitness << "\n";
  
  if (ga_flm.generation_number > 0) {
    //check if last best already present in the population:
    bestsofar_dec = 0;
    for (int j = ga_flm.flm_conf.chromosome_length - 1, k = 0; j >= 0; j--, k++) 
      bestsofar_dec += ga_flm.best_so_far_prev[j] * (1 << k); // ::pow((int)2,k);
    flag = 0;
    for (int i = 0; i < ga_flm.ga_conf.population_size; i++) {
      oldpop_dec = 0;
      for (int j = ga_flm.flm_conf.chromosome_length - 1, k = 0; j >= 0; j--, k++)
        oldpop_dec += int(ga_flm.oldpop[i][j] * ::pow(2, k));
      if (oldpop_dec == bestsofar_dec) {
        flag = 1;
        break;
      }
    }    
    if (flag == 0) {
      // get index of weakest individual
      weakest_fitness = ga_flm.fitness[0];
      for (int i = 0; i < ga_flm.ga_conf.population_size; i++) {
        if (weakest_fitness >= ga_flm.fitness[i]) {
          weakest_fitness = ga_flm.fitness[i];
          weakest_index = i;
        } 
      }
      cerr << "will replace: " << weakest_fitness << "\n";
      
      if (ga_flm.bestsofar_prev_fitness > weakest_fitness) {
        // replace weakest with best from prev. generation
        for (int j = 0; j < ga_flm.flm_conf.chromosome_length; j++)
          ga_flm.oldpop[weakest_index][j] = ga_flm.best_so_far_prev[j];
        cerr << "Restoring best at: " << ga_flm.bestsofar_prev_fitness << "\n";
      }
      else 
        cerr << ga_flm.bestsofar_prev_fitness << " (bestsofar_prev) worse than " << weakest_fitness << "(weakest in present generation)\n";
    }
  }
}
