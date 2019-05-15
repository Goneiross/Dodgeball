#include <cmath>
#include "obstacle.h"
#include "player.h"
#include "floyd.h"

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
        pathAngles[p] = whichPath(players->getPlayer(p), players->getPlayer(targets[p]), infinityInit, infinityDist, obstacles); 
    }
}

double nearestPlayerAngle(int ID){
    return pathAngles[ID];
}