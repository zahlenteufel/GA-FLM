/*
 * "Copyright 2004, University of Washington. All Rights Reserved
 *
 *    Written by Kevin Duh <duh@ee.washington.edu>
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
 * THE UNIVERSITY OF WASHINGTON AND KEVIN DUH
 * SHALL NOT HAVE ANY LIABILITY FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING WITHOUT 
 * LIMITATION LOST PROFITS), HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE  OF
 * THE PROGRAM, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES."
*/

/*
 * Gene.cpp --
 *     Class that encodes the specifications of a factored language model
 *     in a string (gene) format
 */

#include "Gene.h"


// gets the discount and gtmin according to a bit/integer position i in the gene
string Gene::getSmoothOpt(int i) const {
  string opt1, opt2;
      
  if (i >= 0 && i < flm_conf.smooth_len && i < int(flm_conf.discounts.size())) {
    opt1 = flm_conf.discounts[_smooth_opt[i]];
    opt2 = itos(_smooth_opt[i + 1] + 1);
  }
  else {
    opt1 = flm_conf.default_discount;
    opt2 = "1";
  }
  return opt1 + "  gtmin " + opt2;
}

// Sets the bit of a intial-factor and backoff graph gene sequence directly. Used for debugging purposes.
void Gene::setSeq(int i, int bit) {
  _seq[i] = bit;
}

vector<int> Gene::seq() const {
  return _seq;
} 

Gene::Gene(const FLM_Conf& flm_conf, const vector<int>& geneArray) : flm_conf(flm_conf) {
  for (int i = 0; i < flm_conf.max; i++)
    _seq.push_back(geneArray[i]);
  for (int i = flm_conf.max; i < (flm_conf.max + flm_conf.smooth_len); i++)
    _smooth_opt.push_back(geneArray[i]);
}

// Returns the gene sequence for a particular level in the backoff graph. This is used to find out which graph production rules at that level are activated.
vector<int> Gene::seqAtLevel(int lev) const {
  // Level n represents the level that has n conditioning factors

  // Error Check
  if (lev > flm_conf.max_factors || lev < 2)
    cerr << "error: level out-of-bounds in Gene::seqAtLevel" << "Level: "<< lev << endl;
  
  int currentLev = flm_conf.max_factors; 
  int index = flm_conf.max_factors; // begin index after Segment 1 of gene
  
  // Change index to until correct level
  while (currentLev != lev) {
    index += currentLev;
    currentLev--;
  }
  
  vector<int> subvec;
  for (int i = index; i < index + lev; i++)
    subvec.push_back(_seq[i]);
  
  return subvec;
}

// Returns the Initial Conditioning Factors for this gene.
vector<string> Gene::allFactors() const {
  vector<string> subvec;
  for (int i = 0; i < flm_conf.max_factors; i++)
    if (_seq[i] == 1)
      subvec.push_back(flm_conf.factors[i]);
  return subvec;
}
