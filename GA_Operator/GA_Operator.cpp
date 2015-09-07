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
