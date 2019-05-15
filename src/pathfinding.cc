#include "obstacle.h"
#include "player.h"
#include "floyd.h"

using namespace std;

static vector<int> targets;
static vector<int> pathAngles;

void pathfinding(PlayerMap* players, ObstacleMap* obstacles){
    targets.resize(players.getNb());
    pathAngles.resize(players.getNb());
    targets = targetting(players);
    for (int p = 0; p < players.getNb(); p++){
        pathAngles[p] = whichPath(players->getPlayer(p), players->getPlayer(targets[p]), obstacles); 
    }
}

double nearestPlayerAngle(int ID){
    return pathAngles[ID];
}