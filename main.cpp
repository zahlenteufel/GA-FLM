#include <iostream>
#include "GA.h"
#include "GA_Conf.h"
#include "util.h"


#define DEFAULT_SEED "example/SEED"
#define DEFAULT_FLMPARAM "example/FLM-PARAMS"
#define DEFAULT_GAPARAM "example/GA-PARAMS"

void parse_arguments(int argc, char* argv[]);
string seedfile = DEFAULT_SEED, flmparamfile = DEFAULT_FLMPARAM, gaparamfile = DEFAULT_GAPARAM; 

int main(int argc, char* argv[]) {
  try {
    parse_arguments(argc, argv);
    FLM_Conf flm_conf(flmparamfile);
    GA_Conf ga_conf(flm_conf, gaparamfile, seedfile);
    GA ga(ga_conf, flm_conf);

    // ga_flm.search();
  } catch (char const* s) {
    cerr << "ERROR: " << s << endl;
    return 1;
  }

  return 0;
}

void print_usage(char* argv[]) {
  cerr << "Usage: " << argv[0] << " [-s SEEDFILE] [-g GA-PARAMS-FILE] [-f FLM-PARAMS-FILE]" << "\n\n"
    << "  SEEDFILE: genes to be injected in initial population (default: " << DEFAULT_SEED << ")\n"
    << "  GA-PARAMS-FILE: parameters for the algorithm (default: " << DEFAULT_GAPARAM << ")\n"
    << "  FLM-PARAMS-FILE: parameters for the language models (default: " << DEFAULT_FLMPARAM << ")\n" << endl;
  exit(0);
}

void parse_arguments(int argc, char* argv[]) {
  char flag = ' ';

  for (int i = 1; i < argc; ++i) {
    char *pchar = argv[i];

    if (pchar[0] == '-') {
      flag = pchar[1];
      if (flag == 'h')
        print_usage(argv);
      if (flag != 's' and flag != 'g' and flag != 'f' and flag != 'p') {
        cerr << "unrecognized option, for help use -h" << endl;
        exit(1);
      }
    } else {
      if (flag == 's') {
        seedfile = pchar;
      } else if (flag == 'g') {
        gaparamfile = pchar;
      } else if (flag == 'f') {
        flmparamfile = pchar;
      } else if (flag == 'p') {
        cout << "WARNING: PMAKE is no longer supported" << endl;
      }
      flag = ' ';
    }
  }
}
