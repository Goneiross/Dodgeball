#include <iostream>
#include <string>

#include "GUI.h"
#include "simulation.h"

using namespace std;

int main(int argc, char* argv[]){
    int mode = 0; // Maybe create a function returning struct with Mode and Inputfile
    string inputFile = ""; // Better with string or char* ?
    if (argc == 1){
        cout << "Type '--help' for a list of commands" << endl; // #AskBoulic for cout
    } else if (argc < 4) {
        for (int i = 1; i < argc; i++){
            if (string(argv[i]) == "Error"){
                mode = 1;
            
            } else if (string(argv[i]) == "Step"){
                mode = 2;
            } else if(string(argv[i]) == "--help") { // Maybe quit program after --help ? break ?
                cout << "" << endl; // To do
            } else {          // #AskBoulic for extension txt
                inputFile = argv[i];
            }
        }
    } else {
        cout << "Error, too many arguments, type '--help' for help" << endl;
    }
    simulation(inputFile);
    return 0;
}