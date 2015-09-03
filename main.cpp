#include <iostream>
#include "ga-flm.h"
#include "util.h"

void parse_arguments(int argc, char* argv[]);
string seedfile = "SEED", flmparamfile = "FLM-PARAMS", gaparamfile = "GA-PARAMS"; 

int main(int argc, char* argv[]) {
  parse_arguments(argc, argv);
  
  sys("date");
  
  GA_Conf ga_conf(gaparamfile, seedfile);
  FLM_Conf flm_conf(flmparamfile);

  GA_FLM ga_flm(ga_conf, flm_conf);

  ga_flm.search();
  
  sys("date");
  return 0;
}

void print_usage(char* argv[]) {
  cerr << "Usage: " << argv[0] << " [-s SEEDFILE] [-g GA-PARAMS-FILE] [-f FLM-PARAMS-FILE]" << endl;
  exit(1);
}

void parse_arguments(int argc, char* argv[]) {
  char flag = ' ';

  for (int i = 1; i < argc; ++i) {
    char *pchar = argv[i];

    if (pchar[0] == '-') {
      flag = pchar[1];
      if (flag == 'h')
        print_usage(argv);
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
