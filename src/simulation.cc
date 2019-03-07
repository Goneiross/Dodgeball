#include <iostream>
#include <fstream>
#include <string>
#include "simulation.h"
#include "ball.h"
#include "map.h"
#include "player.h"

using namespace std;

void simulation(std::string inputFile){
    int nbCell, nbPlayer, nbObstacle, nbBall;

    ifstream flux (inputFile, ios::in);
    if (!flux) {
        cout << "Unable to open file datafile.txt"; // Maybe better with cerr
        exit(0); 
    }
    
    string tmp0, tmp1, tmp2, tmp3;
    while(flux){
        flux >> tmp0;
        if (tmp0 == "#"){
            do {
                flux >> tmp0;
            } while (tmp0 != "#"); // Is there a # at the end of the comment in every case ???
        }
        flux >> nbCell;
        flux >> nbPlayer;
        Player* players[nbPlayer];
        for (int i = 0; i < nbPlayer; i++){
            flux >> tmp0;
            flux >> tmp1;
            flux >> tmp2;
            flux >> tmp3;
            players[i] = new Player(stod(tmp0), stod(tmp1), stoi(tmp2), stod(tmp3), nbCell);
        }
        flux >> nbObstacle;
        Map* mainMap = new Map(nbCell, nbCell); //Is this correct ? - could modify the class to take only one argument (squared map)
        for (int i = 0; i < nbObstacle; i++){
            flux >> tmp0;
            flux >> tmp1;
            mainMap->addObstacle(stod(tmp0), stod(tmp1));
        }
        flux >> nbBall;
        Ball* balls[nbBall];
        for (int i = 0; i < nbBall; i++){
            flux >> tmp0;
            flux >> tmp1;
            flux >> tmp2;
            balls[i] = new Ball(stod(tmp0), stod(tmp1), stod(tmp2), nbCell);
        }
    }
    flux.close();
}