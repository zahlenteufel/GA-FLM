/*#ifndef CHROMOSOME_H
#define CHROMOSOME_H

#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

class Chromosome {

public:

  Chromosome(const vector<int>& v, float ppl, float fitness);

  string repr() const;

  float fitness() const;

  float perplexity() const;

  float logprob() const;

private:

  vector<int> genes;

  typedef struct {
  	float fitness,
  	float perplexity,
  	float logprob,
  } info;

  static unordered_map<string, info> cache;

  info fetch_or_calculate_info();

};

ostream& operator<<(ostream& stream, const Chromosome& chromosome) {
  return stream << chromosome.repr();
}

#endif
*/