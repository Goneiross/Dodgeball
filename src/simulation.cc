#include <iostream>
#include <fstream>
#include "simulation.h"
#include "ball.h"
#include "map.h"
#include "player.h"

using namespace std;

void simulation(string inputFile){
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
            players[i] = new Player(tmp, tmp1, tmp2, tmp3);
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
}