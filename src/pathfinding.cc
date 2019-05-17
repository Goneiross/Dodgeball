#include "floyd.h"
#include "obstacle.h"
#include "player.h"
#include <cmath>

#include <iostream>

using namespace std;

static vector<int> targets;
static vector<int> pathAngles;
static int infinityInit;
static int infinityDist;

void pathfinding(PlayerMap *players, ObstacleMap *obstacles) {
  bool firstInStep = true;
  infinityDist = pow(obstacles->getCNb(), 2);
  infinityInit = infinityDist + 1;
  targets.resize(players->getNb());
  pathAngles.resize(players->getNb());
  cout << "--------------------TARGETTING--------------------" << endl;
  targets = targetting(players, infinityInit, infinityDist);
  cout << "--------------------WHICH-PATH--------------------" << endl;
  for (int p = 0; p < players->getNb(); p++) {
    cout << "start ID: " << players->getPlayer(p)->getID()
         << " target ID: " << targets[p] << endl;
    pathAngles[p] = whichPath(players->getPlayer(p), players->getPlayer(targets[p]),
                              infinityInit, infinityDist, obstacles, firstInStep);
    cout << "Player " << players->getPlayer(p)->getID()
         << " angle: " << pathAngles[p] * 180 / M_PI << " degrés" << endl;
    cout << endl;
  }
}

double nearestPlayerAngle(int ID) { return pathAngles[ID]; }