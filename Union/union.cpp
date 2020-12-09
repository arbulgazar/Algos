#include <chrono>
#include "union.h"

using namespace std::chrono;

class MonteCarlo
{
private:
  unsigned int *_square;
  unsigned int _edge_size;
  unsigned int _size;

  // init members with probability p to be "conductive"
  void initMembers(unsigned int p) {
    for(unsigned int i = 0; i < _size; i++) {
      unsigned int prob = rand() % 1000 + 1;
      _square[i] = prob <= p ? 1 : 0;
    }
  }

public:
  MonteCarlo(unsigned int edge_size, unsigned int p) :_edge_size(edge_size), _size(edge_size*edge_size) {
    _square = new unsigned int[_size];
    initMembers(p);
  }

  ~MonteCarlo() {
    delete[] _square;
  }

  unsigned int getLeft(unsigned int node) const {
    if ((node+1) % _edge_size == 1) return node;
    else return (node - 1);
  }

  unsigned int getRight(unsigned int node) const {
    if ((node+1) % _edge_size == 0) return node;
    else return (node + 1);
  }

  unsigned int getTop(unsigned int node) const {
    if (node < _edge_size) return node;
    else return (node - _edge_size);
  }

  unsigned int getBottom(unsigned int node) const {
    if ((node+1) + _edge_size > _size) return node;
    else return (node + _edge_size);
  }

  void print_square() {
    for (unsigned int i = 0; i < _size; i++) {
      std::cout << _square[i] << " ";
      if ((i+1) % _edge_size == 0) std::cout << std::endl;
    }
  }

  bool is_conducting() {
    // Allocate 2 nodes as start and finish.
    FastWeightedUnion connections(_size + 2);
    unsigned int start = _size;
    unsigned int finish = _size + 1;

    // Connect start with the first row and finish with last row
    for (unsigned int i = 0; i < _edge_size; i++) {
      connections.unite(start, i);
      connections.unite(finish, _size - i - 1);
    }

    // Unite all connected members that are "on" (1)
    for (unsigned int i = 0; i < _size; i++) {
      if (_square[i]) {
        unsigned int top, bottom, left, right;
        top = getTop(i);
        bottom = getBottom(i);
        left = getLeft(i);
        right = getRight(i);
        if (_square[top]) connections.unite(i, top);
        if (_square[bottom]) connections.unite(i, bottom);
        if (_square[left]) connections.unite(i, left);
        if (_square[right]) connections.unite(i, right);
      }
    }

    return connections.is_united(start, finish);
  }

};

void evaluate(Union &algo, int seed)
{
  srand(seed);
  unsigned int size = algo.get_size();
  auto start = high_resolution_clock::now();
  for (unsigned int i = 0; i < 99999; i++) {
    algo.unite(rand() % size, rand() % size);
  }
  auto stop = high_resolution_clock::now();
  std::cout << "It took: " << duration_cast<milliseconds>(stop-start).count() << std::endl;
}

void monte_carlo_sim(int seed)
{
  std::cout << "Starting Monte Carlo simulation. It will take some time." << std::endl;
  srand(seed);
  for (unsigned int p = 500; p < 600; p++) {
    int average_conduct = 0;
    for (unsigned int i = 0; i < 200; i++) {
      MonteCarlo test(100, p);
      average_conduct = test.is_conducting() ? average_conduct + 1 : average_conduct - 1;
    }
    if (average_conduct < 10 && average_conduct > -10) {
      std::cout << "Average conduct of " << average_conduct << " achieved for P = " << p/1000.0 << std::endl;
    }
    if (average_conduct > 40) break;
  }
}

int main()
{
  unsigned int seed = 0, num_of_members = 0;
  std::cout << "Enter seed and number of members for unions ";
  std::cin >> seed;
  std::cin >> num_of_members;

  SlowUnion slow(num_of_members);
  FastUnion fast(num_of_members);
  FastWeightedUnion fast_weighted(num_of_members);

  std::cout << "Testing slow union with quick find" << std::endl;
  evaluate(slow, seed);

  std::cout << "Testing fast union with slow find" << std::endl;
  evaluate(fast, seed);

  std::cout << "Testing fast weighted union with quick find that is reducing amout of ancestors" << std::endl;
  evaluate(fast_weighted, seed);

  monte_carlo_sim(seed);

  return 0;
}
