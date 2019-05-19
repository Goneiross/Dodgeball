
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
#include "define.h"
#include "floyd.h"
#include "obstacle.h"
#include "player.h"

using namespace std::chrono;

using namespace std;

static vector<int> targets;
static vector<double> pathAngles;
static int infinityInit;
static int infinityDist;

vector<int> targetting(PlayerMap *players, int infinityInit, int infinityDist);

bool pathfinding(PlayerMap *players, ObstacleMap *obstacles, int type) {
    bool firstInStep = true;
    infinityDist = pow(obstacles->getCNb(), 2);
    infinityInit = infinityDist + 1;
    targets.resize(players->getNb());
    pathAngles.resize(players->getNb());
    targets = targetting(players, infinityInit, infinityDist);
    auto start = high_resolution_clock::now();
    for (int p = 0; p < players->getNb(); p++) {
        if (type == FLOYD) {
            pathAngles[p] =
                whichPath(players->getPlayer(p), players->getPlayer(targets[p]),
                          infinityInit, infinityDist, obstacles, firstInStep);
        } else if (type == ASTAR) {
            pathAngles[p] = whichPath(players, p, targets[p], obstacles);
        }
        if (pathAngles[p] == 42) { return 1; }
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

vector<int> targetting(PlayerMap *players, int infinityInit, int infinityDist) {
    vector<vector<double>> dBP(players->getNb(),
                               vector<double>(players->getNb(), infinityInit));
    for (int i = 0; i < players->getNb(); i++) {
        for (int j = 0; j < i; j++) {
            dBP[i][j] = distanceAbs(
                players->getPlayer(i)->getL(), players->getPlayer(i)->getC(),
                players->getPlayer(j)->getL(), players->getPlayer(j)->getC());
            dBP[j][i] = dBP[i][j];
        }
    }
    vector<double> minDistance(players->getNb());
    vector<int> target(players->getNb());
    for (int i = 0; i < players->getNb(); i++) {
        minDistance[i] = infinityDist;

        for (int j = 0; j < players->getNb(); j++) {
            if (i != j) {
                if (dBP[i][j] < minDistance[i]) {
                    minDistance[i] = dBP[i][j];
                    target[i] = j;
                }
            }
        }
    }
    return target;
}