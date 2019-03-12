//RAJOUTER IFNDEF
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector> 

#include "simulation.h"
#include "ball.h"
#include "map.h"
#include "player.h"
#include "error.h"

using namespace std;

void checkCollisions(vector<Player*> players, int p, int pmax, double delta){ //And if not during ini ?
    for (int i = 0; i <= pmax; i++){
        if(i != p){
            double d = distance(players[i]->getHitbox()->getX(), players[i]->getHitbox()->getY(), players[p]->getHitbox()->getX(),players[p]->getHitbox()->getY());
            //Try doing with circle and not coordinates
            if (d < (players[i]->getRadius() + players[p]->getRadius() + delta)){
                cout << PLAYER_COLLISION(i+1, p+1) << endl; //p or p+1 ?
                exit(1);
            }
        }
    }
}

void checkCollisions(vector<Ball*> balls, int b, int bmax, double delta){ //And if not during ini ?
    for (int i = 0; i <= bmax; i++){
        if(i != b){
            double d = distance(balls[i]->getHitbox()->getX(), balls[i]->getHitbox()->getY(), balls[b]->getHitbox()->getX(),balls[b]->getHitbox()->getY());
            //Try doing with circle and not coordinates
            if (d < (balls[i]->getRadius() + balls[b]->getRadius() + delta)){
                cout << BALL_COLLISION(i+1, b+1) << endl; //p or p+1 ?
                exit(1);
            }
        }
    }
}

void checkCollisions(vector<Player*> players, vector<Ball*> balls, int p, int b, double delta){ //And if not during ini ?
    double d = distance(players[p]->getHitbox()->getX(), players[p]->getHitbox()->getY(), balls[b]->getHitbox()->getX(), balls[b]->getHitbox()->getY());
    if (d < (players[p]->getRadius() + balls[b]->getRadius() + delta)){
        cout << PLAYER_BALL_COLLISION(p+1, b+1) << endl; //p or p+1 ?
        exit(1);
    }
}

void checkCollisions(vector<Player*> players, Map* map, int p, int o, double delta){ //And if not during ini ?
    int m = map->getObstacle().size();
    double d = distance(map->getObstacle()[o]->getHitbox(), players[p]->getHitbox());
    double X = map->getObstacle()[o]->getHitbox()->getX() - players[p]->getHitbox()->getX();
    double Y = map->getObstacle()[o]->getHitbox()->getY() - players[p]->getHitbox()->getY();
    double angle = atan(Y/X);
    // cout << "angle : "<< angle <<endl;
    double rayon = map->getObstacle()[o]->getHitbox()->getSide() / 2;
    double included = included = rayon / X * d;
    // cout << p << " " << o << endl;
    // cout << d << endl;
    // cout << players[p]->getRadius() << " " << map->getObstacle()[o]->getHitbox()->getSide() << " " << included << " " << delta << endl << endl;
    if (d < (players[p]->getRadius() + included + delta)){
        cout << COLL_OBST_PLAYER(o + 1, p + 1) << endl; //p or p+1 ?
        exit(1);
    }
} //NOT WORKING YET

void simulation(std::string inputFile){
    int nbCell, nbPlayer, nbObstacle, nbBall;
    string tmp0, tmp1, tmp2, tmp3;
    int part = 0, p = 0, o = 0, b = 0; //Use enum instead of part
    char tmp;

    vector<Player*> players;
    vector<Ball*> balls;

    double MJ, ML;

    Map* mainMap; //Could modify the class to take only one argument (squared map)
    
    ifstream flux (inputFile, ios::in);
    if (!flux) {
        cout << "Unable to open file " << inputFile << endl; // Maybe better with cerr
        exit(0); 
    }while(flux >> tmp0){
        if (tmp0 == "#"){
            do {flux.get(tmp);} while (tmp != '\n');
        } else if(part == 0){
            nbCell = stoi(tmp0);
            mainMap = new Map(nbCell, nbCell);
            if ((nbCell > MAX_CELL) || (nbCell < MIN_CELL)){
                cout << "Error, wrong cell number" << endl; //AskBoulic
                exit(1);
            }
            MJ = COEF_MARGE_JEU * (SIDE/nbCell);
            ML = MJ / 2;
            part++;
        } else if (part == 1){
            nbPlayer = stoi(tmp0);
            players.reserve(nbPlayer);
            part++;
        } else if (part == 2){
            flux >> tmp1 >> tmp2 >> tmp3;
            if(((abs(stoi(tmp0)) > DIM_MAX) || (abs((stoi(tmp1)) > DIM_MAX)))){ //must be doubles ? (vabs(double) ambiguous)
                cout << PLAYER_OUT(p+1) << endl;
                exit(1);
            } else {
                players.push_back(new Player(stod(tmp0), stod(tmp1), stoi(tmp2), stod(tmp3), nbCell));
                checkCollisions(players, p, p, ML);
            }
            p ++;
            if (p == nbPlayer){part++;}
        } else if (part == 3){
            nbObstacle = stoi(tmp0);
            part++;
        } else if (part == 4){
            flux >> tmp1;
            if(stoi(tmp0) >= nbCell){
                cout << OBSTACLE_VALUE_INCORRECT(stoi(tmp0)) << endl;
                exit(1);
            } else if(stoi(tmp1) >= nbCell){
                cout << OBSTACLE_VALUE_INCORRECT(stoi(tmp1)) << endl;
                exit(1);
            } else if(stoi(tmp0) < 0){
                cout << OBSTACLE_VALUE_INCORRECT(stoi(tmp0)) << endl;
                exit(1);
            } else if(stoi(tmp1) < 0){
                cout << OBSTACLE_VALUE_INCORRECT(stoi(tmp1)) << endl;
                exit(1);
            } else if(mainMap->isObstacle(stoi(tmp0), stoi(tmp1))){
                cout << MULTI_OBSTACLE(stoi(tmp0), stoi(tmp1)) << endl;
            } else {
                mainMap->addObstacle(stod(tmp0), stod(tmp1));
                for (int i = 0; i < nbPlayer; i++){
                    checkCollisions(players, mainMap, i, o, ML);
                }
            }
            o++;
            if (o == nbObstacle){part++;} //Check Collision with players
        } else if (part == 5){
            nbBall = stoi(tmp0);
            balls.reserve(nbBall+nbPlayer);
            part++;
        } else if (part == 6){
            flux >> tmp1 >> tmp2;
            if((abs(stoi(tmp0)) > DIM_MAX) || (abs(stoi(tmp1)) > DIM_MAX)){     //est-ce que tmp0-1 must be d (marcherait pas)
                cout << BALL_OUT(b+1) << endl; //b or b+1 ?
                exit(1);
            } else {
                balls.push_back(new Ball(stod(tmp0), stod(tmp1), stod(tmp2), nbCell));
                checkCollisions(balls, b, b, ML);
                for (int i = 0; i < nbPlayer; i++){
                    checkCollisions(players, balls, i, b, ML);
                }
                // CHeck Collision with obstacles
            }
            b++;
            if (b == nbBall){part++;}
        } else {
            flux.get(tmp);
        }
    }
    flux.close();
}