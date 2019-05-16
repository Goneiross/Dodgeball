#include <cmath>
#include "obstacle.h"
#include "player.h"
#include "floyd.h"

#include <iostream>

using namespace std;

static vector<int> targets;
static vector<int> pathAngles;
static int infinityInit;
static int infinityDist;

void pathfinding(PlayerMap* players, ObstacleMap* obstacles){
    infinityDist = pow(obstacles->getCNb(), 2);
    infinityInit = infinityDist + 1;
    targets.resize(players->getNb());
    pathAngles.resize(players->getNb());
    targets = targetting(players, infinityInit, infinityDist);
    for (int p = 0; p < players->getNb(); p++){
        cout << "start ID: " << players->getPlayer(p)->getID() << " target ID: " << targets[p] << endl;
        pathAngles[p] = whichPath(players->getPlayer(p), players->getPlayer(targets[p]), infinityInit, infinityDist, obstacles); 
    }
    cout << "t" << endl;
}

double nearestPlayerAngle(int ID){
    return pathAngles[ID];
}