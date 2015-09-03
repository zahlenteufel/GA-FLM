#include "Chromosome.h"

string Chromosome::repr() const {
  string s; 
  for (auto gene : genes)
    s += itos(gene);
  return s
}

float Chromosome::fitness() {
  auto inf = fetch_or_calculate(); // can we write this in one line?
  return inf.fitness;
}

float Chromosome::perplexity() {
  auto inf = fetch_or_calculate();
  return inf.perplexity;
}

float Chromosome::logprob() {
  auto inf = fetch_or_calculate();
  return inf.logprob;
}

info Chromosome::fetch_or_calculate_info() {
  string key = repr();
  auto it = cache.find(key);
  if (it != cache.end())
    return *it;

  // Call fngram-count, etc. etc..
  ngram_compute_info();

  return cache[key] = info{1, 2, 3};
}

info Chromosome::ngram_compute_info(string key) {
  filename = ga_path + key
  ostream script("_script.sh");

  // generate flm file -> .flm

  // run fngram-count -> .count.gz, lm.gz

  // run fngram -ppl -> .EvalLog
  
  string command = flm_conf.make_fngram_command(basename, ga_conf.ga_path);
  fngram_commands.push(command);
  execute_fngram_commands(fngram_commands);
  collect_ppl();
}
