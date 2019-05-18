
/*!
  \file   pathfinding.h
  \author Guillaume Pellerin & Vincent Miche
  \date   May 2019
  \brief  pathfinding implementation
*/

#include <chrono>
#include <cmath>
#include <iostream>

#include "aStar.h"
#include "floyd.h"
#include "obstacle.h"
#include "player.h"

using namespace std::chrono;

using namespace std;

static vector<int> targets;
static vector<double> pathAngles;
static int infinityInit;
static int infinityDist;

bool pathfinding(PlayerMap *players, ObstacleMap *obstacles, int type = 1) {
    bool firstInStep = true;
    infinityDist = pow(obstacles->getCNb(), 2);
    infinityInit = infinityDist + 1;
    targets.resize(players->getNb());
    pathAngles.resize(players->getNb());
    targets = targetting(players, infinityInit, infinityDist);
    auto start = high_resolution_clock::now();
    for (int p = 0; p < players->getNb(); p++) {
        if (type == 0) {
            pathAngles[p] =
                whichPath(players->getPlayer(p), players->getPlayer(targets[p]),
                          infinityInit, infinityDist, obstacles, firstInStep);
        } else {
            pathAngles[p] = whichPath(players, p, targets[p], obstacles);
        }
        if (pathAngles[p] == 42) {
            return 1;
        }
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
    }
    return 0;
}

double nearestPlayerAngle(int ID) { return pathAngles[ID]; }

bool isReadyToFire(int ID, PlayerMap *players, ObstacleMap *obstacles) {
    int l1 = players->getPlayer(ID)->getL();
    int c1 = players->getPlayer(ID)->getC();
    int l2 = players->getPlayer(targets[ID])->getL();
    int c2 = players->getPlayer(targets[ID])->getC();
    if (isThereObstacleBetween(l1, c1, l2, c2, obstacles)) {
        return false;
    } else {
        return true;
    }
}