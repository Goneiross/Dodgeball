#include "obstacle.h"
#include "player.h"
#include "floyd.h"

using namespace std;

static vector<int> targets;
static vector<int> pathAngles;
static int infinityInit = pow(nbCell, 2) + 1;
static int infinityDist = pow(nbCell, 2);

void pathfinding(PlayerMap* players, ObstacleMap* obstacles){
    targets.resize(players.getNb());
    pathAngles.resize(players.getNb());
    targets = targetting(players, infinityInit, infinityDist);
    for (int p = 0; p < players.getNb(); p++){
        pathAngles[p] = whichPath(players->getPlayer(p), players->getPlayer(targets[p]), obstacles, infinityInit, infinityDist); 
    }
}

double nearestPlayerAngle(int ID){
    return pathAngles[ID];
}