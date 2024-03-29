/*!
  \file   main.cc
  \author Guillaume Pellerin & Vincent Miche
  \date   March 2019
  \brief  main module
*/

#include <iostream>
#include <string>

#include "GUI.h"
#include "define.h"
#include "simulation.h"

using namespace std;

int main(int argc, char *argv[]) {
    int mode = NORMAL_MODE;
    string inputFile = "";
    string saveFile = "";
    if (argc == 1) {
        // cout << "Type '--help' for a list of commands" << endl;
    } else if (argc <= 4) {
        for (int i = 1; i < argc; i++) {
            if (string(argv[i]) == "Error" && mode == NORMAL_MODE) {
                mode = ERROR_MODE;
            } else if (string(argv[i]) == "Step" && mode == NORMAL_MODE) {
                mode = STEP_MODE;
            } else if (string(argv[i]) == "--help") {
                cout << "Type :" << endl
                     << "'Error' to check input file" << endl
                     << "'Step' to run only one step of the simulation" << endl
                     << "'filename.extension' to use this input file" << endl;
                exit(1);
            } else {
                if (inputFile == "") {
                    inputFile = argv[i];
                } else {
                    saveFile = argv[i];
                }
            }
        }
    } else {
        cout << "Error, too many arguments, type '--help' for help" << endl;
        exit(1);
    }
    if (saveFile == "") {
        simulation(mode, inputFile);
    } else {
        simulation(mode, inputFile, saveFile);
    }
    return 0;
}