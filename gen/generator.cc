#include "define.h"
#include <fstream>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
  int n = 99;
  std::ofstream flux("testG.txt", std::fstream::out);
  int nbObjects = (n / 3) * n;
  double S = SIDE / n;

  flux << n << endl;

  flux << nbObjects << endl;
  int tmp = 0;
  for (int j = 0; j < n; j++) {
    for (int i = 0; i < n; i += 3) {
      flux << (0.5 * S + j * S) - (DIM_MAX) << "  " << (0.5 * S + i * S) - (DIM_MAX)
           << " " << 0 << "  " << 0 << endl;
    }
  }

  flux << nbObjects << endl;
  for (int j = 0; j < n; j++) {
    for (int i = 1; i < n; i += 3) {
      flux << i << "  " << j << endl;
    }
  }

  flux << nbObjects << endl;
  for (int j = 0; j < n; j++) {
    for (int i = 2; i < n; i += 3) {
      flux << (0.5 * S + j * S) - (DIM_MAX) << "  " << (0.5 * S + i * S) - (DIM_MAX)
           << " " << 0 << endl;
    }
  }
  return 0;
}