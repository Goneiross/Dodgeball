#include "obstacle.h"
#include "player.h"
#include "floyd.h"

using namespace std;

static vector<int> targets;
static vector<int> pathAngle;

void pathfinding(PlayerMap* players, ObstacleMap* obstacles){
    targets.resize(players.getNb())
    pathAngle.resize(players.getNb());
    targets = targetting(players);
    for (int p = 0; p < players.getNb(); p++){
        pathAngle[p] = whichPath(players->getPlayer(p), players->getPlayer(targets[p])); 
    }
}

double angle(int ID){
    return pathAngle[ID];
}