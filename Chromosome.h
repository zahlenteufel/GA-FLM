#ifndef CHROMOSOME_H
#define CHROMOSOME_H

#include <vector>
#include <string>

using namespace std;

class Chromosome {

public:

  Chromosome(const vector<int>& v, float ppl, float fitness);

  string repr() const;

private:

  vector<int> genes;

};

ostream& operator<<(ostream& stream, const Chromosome& chromosome) {
  return stream << chromosome.repr();
}

#endif
