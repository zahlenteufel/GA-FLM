#include "FLM_Conf.h"
#include "Representation/BackoffGraph.h"
#include "util.h"

FLM_Conf::FLM_Conf(string s) {
  parse_FLM_PARAMS_file(s);
}

void FLM_Conf::parse_FLM_PARAMS_file(const string& paramfile) {
  ifstream flm_param_file(paramfile.c_str());
  data_path = extract_option(flm_param_file);
  trainset = extract_option(flm_param_file);
  devset = extract_option(flm_param_file);
  fngramcount_path = extract_option(flm_param_file);
  fngram_path = extract_option(flm_param_file);
  fngramcount_options = extract_option(flm_param_file);
  fngram_options = extract_option(flm_param_file);
  factor_to_predict = extract_option(flm_param_file);
  vector<string> custom_factors = split(extract_option(flm_param_file), ","); // not done getting factors yet. Have to read in Context next.

  // Context of factors
  vector<string> list_of_context = split(extract_option(flm_param_file), ",");

  if (list_of_context[0] == "0") {
    // Specifying context == 0 indicates that user wants to use Total_Available_Factors directly as FACTORS.
    // In other words, user specified something like "W1, W2, P1, S2" and don't need the context specification.
    factors = custom_factors;
  }
  else {
    // User specified the context and only indicated "W, P, S" for Total_Available_Factors.
    // Thus we need to build "W1,P1,S1,W2,P2,S2", for example, from context
    for (string order : list_of_context)
      for (string factor : custom_factors)
        factors.push_back(factor + order);
  }

  // Smoothing Option length
  smooth_len = parse_int(extract_option(flm_param_file));
  
  // Calculating max and max_factors and Total Chromosome Length
  max_factors = int(factors.size());
  max = max_factors;
  for (int summand = max_factors; summand >= 2; summand--)
    max += summand;
  chromosome_length = max + smooth_len;

  // List of discount options (e.g. kndiscount, wbdiscount)
  discounts = split(extract_option(flm_param_file), ",");

  // Default discount
  default_discount = extract_option(flm_param_file);

  // Max cutoff for smoothing option
  cutoff_max = parse_int(extract_option(flm_param_file));

  // Print out read-in parameters
  dump();
}

void FLM_Conf::dump() const {
  cerr << "Data path: " << data_path << "\n";
  cerr << "Trainset: " << trainset << "\n";
  cerr << "Devset: " << devset << "\n";
  cerr << "Fngramcount_options: " << fngramcount_options << "\n";
  cerr << "Fngram_options: " << fngram_options << "\n";
  cerr << "Factor_to_predict: " << factor_to_predict << "\n";
  cerr << "Total Set of Conditioning Factors: ";
  for (string factor : factors)
    cerr << factor << " ";
  cerr << "\n";
  cerr << "Total number of factors available: " << max_factors << "\n";
  cerr << "Max / length of init factors+backoff graph gene : " << max << "\n";
  cerr << "Smoothing options length : " << smooth_len << "\n";
  cerr << "Total Chromosome length : " << chromosome_length << "\n";
  cerr << "List of Discounting methods : " << "\n" << "    ";
  for (string discount : discounts)
    cerr << discount << " ";
  cerr << "\n";
  cerr << "Max cutoff (gtmin) for inclusion in LM: " << cutoff_max << "\n";
  cerr << "------------------------------------------------" << endl; 
}

string FLM_Conf::make_fngram_command(const string& filename, const string& ga_path) const {
  // TODO: verify that the path are working.. 

  string fngram_count_call = fngramcount_path + " -factor-file " + ga_path + filename + ".flm" +
    " -lm -write-counts -unk -text " + data_path + trainset + " " + fngramcount_options + "; ";
  
  string fngram_call = fngram_path + " -factor-file " + ga_path + filename + ".flm" +
    " -unk -ppl " + data_path + devset + " " + fngram_options + " > " + ga_path + filename + ".EvalLog; ";

  // Command for calculating perplexity. Necessary if fitness function includes a complexity term (e.g. BIC)
  string evaluate_perplexity = "zcat " + ga_path + filename + ".lm.gz | perl getcomplexity.pl > " + ga_path + filename + ".complexity; "; 

  // string clean = "rm " + ga_path + filename + ".lm.gz; rm " + ga_path + filename + ".count.gz ";
  
  return fngram_count_call + fngram_call + evaluate_perplexity; // + clean;
}

string to_string(const Chromosome& chromosome) {
  stringstream ss;
  for (int gene : chromosome)
    ss << gene;
  return ss.str();
}

void FLM_Conf::create_factor_file(const Chromosome& chromosome, const string& ga_path) const {
  BackoffGraph backoff_graph(*this, chromosome);
  ofstream outfile((ga_path + to_string(chromosome) + ".flm").c_str());
  outfile << backoff_graph.factor_file(ga_path, to_string(chromosome));
}
