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

#include "GA_Operator.h"

default_random_engine GA_Operator::rng;

void GA_Operator::set_random_seed(unsigned seed) {
  GA_Operator::rng = default_random_engine(seed);
}

//return a random number between 0 and 1
float GA_Operator::random_number_in_01() {
  uniform_real_distribution<float> rd(0, 1);
  return rd(GA_Operator::rng);
}

int GA_Operator::random_bit() {
  bernoulli_distribution distribution(0.5);
  return distribution(GA_Operator::rng);
}

int GA_Operator::random_int(int max) {
  uniform_int_distribution<int> distribution(0, max - 1);
  return distribution(GA_Operator::rng);
}


// void GA_Operator::be_elitist() {
//   int weakest_index = 0, flag = 0;
//   int bestsofar_dec = 0, oldpop_dec = 0;
//   float weakest_fitness = 0;

//   cerr << "Elitist: gen_number " << ga_flm.generation_number << "\n";
//   cerr << "fitness bestsofar_prev=" << ga_flm.bestsofar_prev_fitness << "\n";
//   cerr << "fitness bestof thisgen = " << ga_flm.bestsofar_fitness << "\n";
  
//   if (ga_flm.generation_number > 0) {
//     //check if last best already present in the population:
//     bestsofar_dec = 0;
//     for (int j = ga_flm.flm_conf.chromosome_length - 1, k = 0; j >= 0; j--, k++) 
//       bestsofar_dec += ga_flm.best_so_far_prev[j] * (1 << k); // ::pow((int)2,k);
//     flag = 0;
//     for (int i = 0; i < ga_flm.ga_conf.population_size; i++) {
//       oldpop_dec = 0;
//       for (int j = ga_flm.flm_conf.chromosome_length - 1, k = 0; j >= 0; j--, k++)
//         oldpop_dec += int(ga_flm.oldpop[i][j] * ::pow(2, k));
//       if (oldpop_dec == bestsofar_dec) {
//         flag = 1;
//         break;
//       }
//     }    
//     if (flag == 0) {
//       // get index of weakest individual
//       weakest_fitness = ga_flm.fitness[0];
//       for (int i = 0; i < ga_flm.ga_conf.population_size; i++) {
//         if (weakest_fitness >= ga_flm.fitness[i]) {
//           weakest_fitness = ga_flm.fitness[i];
//           weakest_index = i;
//         } 
//       }
//       cerr << "will replace: " << weakest_fitness << "\n";
      
//       if (ga_flm.bestsofar_prev_fitness > weakest_fitness) {
//         // replace weakest with best from prev. generation
//         for (int j = 0; j < ga_flm.flm_conf.chromosome_length; j++)
//           ga_flm.oldpop[weakest_index][j] = ga_flm.best_so_far_prev[j];
//         cerr << "Restoring best at: " << ga_flm.bestsofar_prev_fitness << "\n";
//       }
//       else 
//         cerr << ga_flm.bestsofar_prev_fitness << " (bestsofar_prev) worse than " << weakest_fitness << "(weakest in present generation)\n";
//     }
//   }
// }
