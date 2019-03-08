#include <iostream>
#include <fstream>
#include <string>
#include "simulation.h"
#include "ball.h"
#include "map.h"
#include "player.h"
#include "error.h"

using namespace std;

void simulation(std::string inputFile){
    int nbCell, nbPlayer, nbObstacle, nbBall;
    string tmp0, tmp1, tmp2, tmp3;
    int part = 0, p = 0, o = 0, b = 0; //Use enum instead of part
    char tmp;
    Map* mainMap = new Map(nbCell, nbCell); //Could modify the class to take only one argument (squared map)
    ifstream flux (inputFile, ios::in);
    if (!flux) {
        cout << "Unable to open file datafile.txt"; // Maybe better with cerr
        exit(0); 
    }while(flux >> tmp0){
        if (tmp0 == "#"){
            do {flux.get(tmp);} while (tmp != '\n');
        } else if(part == 0){
            nbCell = stoi(tmp0);
            part++;
        } else if (part == 1){
            nbPlayer = stoi(tmp0);
            part++;
        } else if (part == 2){
            Player* players[nbPlayer];
            flux >> tmp1 >> tmp2 >> tmp3;
            if((stod(tmp0) > double(nbCell)) || (stod(tmp1) > double(nbCell))){
                cout << PLAYER_OUT(p+1) << endl;
                exit(1);
            } else {
                players[p] = new Player(stod(tmp0), stod(tmp1), stoi(tmp2), stod(tmp3), nbCell);
            }
            p ++;
            if (p == nbPlayer){part++;}
        } else if (part == 3){
            nbObstacle = stoi(tmp0);
            part++;
        } else if (part == 4){
            flux >> tmp1;
            if(stoi(tmp0) > nbCell){
                cout << OBSTACLE_VALUE_INCORRECT(stoi(tmp0)) << endl;
                exit(1);
            } else if(stoi(tmp1) > nbCell){
                cout << OBSTACLE_VALUE_INCORRECT(stoi(tmp1)) << endl;
                exit(1);
            } else if(mainMap->isObstacle(stoi(tmp0), stoi(tmp1))){
                cout << MULTI_OBSTACLE(stoi(tmp0), stoi(tmp1)) << endl;
            } else {
                mainMap->addObstacle(stod(tmp0), stod(tmp1));
            }
            o++;
            if (o == nbObstacle){part++;}
        } else if (part == 5){
            nbBall = stoi(tmp0);
            part++;
        } else if (part == 6){
            Ball* balls[nbBall];
            flux >> tmp1 >> tmp2;
            if(stod(tmp0) > double(nbCell) || stod(tmp1) > double(nbCell)){
                cout << BALL_OUT(b+1) << endl;
                exit(1);
            } else {
                balls[b] = new Ball(stod(tmp0), stod(tmp1), stod(tmp2), nbCell);
            }
            b++;
            if (b == nbBall){part++;}
        } else {
            cout << "t" << endl;
            flux.get(tmp);
        }
    }
    flux.close();
}