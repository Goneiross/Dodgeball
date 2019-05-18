#include "floyd.h"
#include "aStar.h"
#include "obstacle.h"
#include "player.h"
#include <cmath>

#include <chrono>
#include <iostream>

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
  cout << "--------------------TARGETTING--------------------" << endl;
  targets = targetting(players, infinityInit, infinityDist);
  auto start = high_resolution_clock::now();
  cout << "--------------------WHICH-PATH--------------------" << endl;
  for (int p = 0; p < players->getNb(); p++) {
    cout << "start ID: " << players->getPlayer(p)->getID() << "(" << players->getPlayer(p)->getL() << "," << players->getPlayer(p)->getC() << ")"
         << " target ID: " << targets[p] << "(" << players->getPlayer(targets[p])->getL() << "," << players->getPlayer(targets[p])->getC() << ")" << endl;
    
    if (type == 0){
      pathAngles[p] = whichPath(players->getPlayer(p), players->getPlayer(targets[p]),
                              infinityInit, infinityDist, obstacles, firstInStep);
    } else {
      pathAngles[p] = whichPath(players, p, targets[p], obstacles);
    }
    if (pathAngles[p] == 42){return 1;}
    cout << "Player " << players->getPlayer(p)->getID()
         << " angle: " << pathAngles[p] * 180 / M_PI << " degrés" << endl;
    cout << endl;
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    cout << "Time taken by Path: " << duration.count() / 1000 << " ms" << endl;
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