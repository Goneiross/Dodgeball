#include <iostream>
#include <string>
#include <fstream>

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
    
    int nbCell, nbPlayer, nbObstacle, nbBall;

    ifstream flux (inputFile, ios::in);
    if (!flux) {
        cout << "Unable to open file datafile.txt"; // Maybe better with cerr
        exit(0); 
    }
    string tmp, tmp1, tmp2, tmp3;
    while(flux){
        flux >> tmp;
        if (tmp == "#"){
            do {
                flux >> tmp;
            } while (tmp != "#"); // Is there a # at the end of the comment in every case ???
        }
        flux >> nbCell;
        flux >> nbPlayer;
        Player* players[nbPlayer];
        for (int i = 0; i < nbPlayer; i++){
            flux >> tmp;
            flux >> tmp1;
            flux >> tmp2;
            flux >> tmp3;
            Players[i] = new Player(tmp, tmp1, tmp2, tmp3);
        }
        flux >> nbObstacle;
        Map mainMap = new Map(nbCell, nbCell);
        for (int i = 0; i < nbObstacle; i++){
            flux >> tmp;
            flux >> tmp1;
            addObstacle(tmp, tmp1);
        }
        flux >> nbBall;
        Ball* balls[nbBall];
        for (int i = 0; i < nbBall; i++){
            flux >> tmp;
            flux >> tmp1;
            flux >> tmp2;
            balls[i] = new Ball(tmp, tmp1, tmp2, nbCell);
        }
    }
    flux.close();


    return 0;
}