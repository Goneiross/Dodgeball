#include <iostream>
#include <string>

#include "GUI.h"
#include "simulation.h"

using namespace std;

int main(int argc, char *argv[]) {
  int mode = 0;
  string inputFile;
  if (argc == 1) {
    cout << "Type '--help' for a list of commands" << endl;
  } else if (argc < 4) {
    for (int i = 1; i < argc; i++) {
      if (string(argv[i]) == "Error") {
        mode = 1;
      } else if (string(argv[i]) == "Step") {
        mode = 2;
      } else if (string(argv[i]) == "--help") {
        cout << "Type :" << endl
             << "'Error' to check input file" << endl
             << "'Step' to run only one step of the simulation" << endl
             << "'filename.extension' to use this input file" << endl;
        exit(1);
      } else {
        inputFile = argv[i];
      }
    }
  } else {
    cout << "Error, too many arguments, type '--help' for help" << endl;
  }
  simulation(inputFile, mode);
  return 0;
}