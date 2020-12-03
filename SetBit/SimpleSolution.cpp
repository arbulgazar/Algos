#include <iostream>
#include <chrono>

using namespace std::chrono;

static const char table[256] = {
#define D0(n) n, n+1, n+1, n+2
#define D1(n) D0(n), D0(n+1), D0(n+1), D0(n+2)
#define D2(n) D1(n), D1(n+1), D1(n+1), D1(n+2)
D2(0), D2(1), D2(1), D2(2)
};

unsigned int NaiveSolution(const int number)
{
  unsigned int result = 0;
  int input = number;
  while(input) {
    result += input & 1;
    input >>= 1;
  }
  return result;
}

unsigned int BrianKernigham(const int number)
{
  unsigned int result = 0;
  int input = number;

  while(input) {
    input &= input-1;
    result++;
  }

  return result;
}

unsigned int LookUp(const int number)
{
  unsigned int result = 0;
  
  result = table[number & 0xFF] +
           table[(number >> 8) & 0xFF] +
           table[(number >> 16) & 0xFF] +
           table[(number >> 24) & 0xFF];

  return result;
}

void evaluate(const int seed, unsigned int (*foo)(const int))
{
  srand(seed);
  auto start = high_resolution_clock::now();
  for (unsigned int i = 0; i < 9999999; i++) {
    foo(rand());
  }
  auto stop = high_resolution_clock::now();
  std::cout << "It took: " << duration_cast<milliseconds>(stop-start).count() << std::endl;
}

int main()
{
  int seed;

  std::cout << "Enter a seed ";
  std::cin >> seed;

  std::cout << "Testing Naive" << std::endl;
  evaluate(seed, NaiveSolution);
  std::cout << "Testing BrianKernigham" << std::endl;
  evaluate(seed, BrianKernigham);
  std::cout << "Testing LookUp" << std::endl;
  evaluate(seed, LookUp);

  return 0;
}
