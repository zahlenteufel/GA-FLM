# GA-FLM: Genetic Algorithms for Learning Factored Language Model Structure

## (Modern C++11 Reimplementation)

## Introduction 

GA-FLM is a genetic algorithms program for automatically learning
factored language model structures. It is used as an extension to the
factored language model programs in the SRI Language Modeling
toolkit. 

The program takes as input some training/development text files and
some parameter files that specify the type of genetic algorithms and
factored language model desired by the user. It then uses standard
genetic algorithms search to build a population of factored language
models and optimizes for their development set perplexity.

For more information, refer to the coling2004.pdf paper in the doc directory.

---
> GA-FLM -- Genetic Algorithms for Learning Factored Language Model Structure
> Copyright (c) 2004 University of Washington
>
> 	Written by Kevin Duh and Sonia Parandekar
>
> NO WARRANTY
> THE PROGRAM IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OR
> CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED INCLUDING, WITHOUT
> LIMITATION, ANY WARRANTIES OR CONDITIONS OF TITLE, NON-INFRINGEMENT,
> MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE. Each Recipient is
> solely responsible for determining the appropriateness of using the Program
> and assumes all risks associated with such use, including but not limited
> to the risks and costs of program errors, compliance with applicable laws,
> damage to or loss of data, programs or equipment, and unavailability or
> interruption of operations.
>
> DISCLAIMER OF LIABILITY
> THE UNIVERSITY OF WASHINGTON, KEVIN DUH AND SONIA PARANDEKAR
> SHALL NOT HAVE ANY LIABILITY FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
> SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING WITHOUT 
> LIMITATION LOST PROFITS), HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
> WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
> (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE  OF
> THE PROGRAM, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES."
>
> For more information, bug reports, fixes, contact:
>    Kevin Duh
>    Dept. of Electrical Engineering, Paul Allen Center 
>    University of Washington
>    Seattle, WA 98195
>    USA
>    <duh@ee.washington.edu>
>    (http://ssli.ee.washington.edu/people/duh/ Homepage)
--

## General Requirements and Installation

 - Make sure you have the SRI Language Modeling Toolkit version 1.4.1 or above. The two executables that are needed in that package are "fngram-count" and "fngram", which builds and tests factored language models. The toolkit can be downloaded at: <http://www.speech.sri.com/projects/srilm>

 - To install, simply type "make" in this directory to create the executable "ga-flm"

## Running the program

There are two ways to run the program, either by running the executable "ga-flm" directly or running the "repeat-ga-flm.pl" Perl wrapper function.

> ga-flm [-g ga_param_file] [-f factoredLM_param_file] [-s seedfile]

> perl repeat-ga-flm.pl [num] [-g ga_param_file] [-f factoredLM_param_file] [-s seedfile]

Refer to the manual.pdf in the doc directory for more detailed explanation.

## Example runs of GA-FLM 

  TODO

## Credits

 - GA-FLM is the work of several people. The algorithm for automatically learning factored language model structure was developed with Katrin Kirchhoff. Jeff Bilmes gave invaluable support on the SRILM factored language model programs. Sonia Parandekar wrote almost all of the basic genetic algorithms code.

 - Published research using GA-FLM may cite the following paper:

  > Kevin Duh and Katrin Kirchhoff, "Automatic Learning of Language Model Structure", Proc. of the 20th International Conference on Computational Linguistics (COLING-2004), Geneva, Switzerland, August 2004.
