/*!
  \file   aStar.cc
  \author Guillaume Pellerin & Vincent Miche
  \date   May 2019
  \brief  add description
*/

// GOOD LUCK CORRECTING MY CODE

#include <math.h>
#include <vector>
#ifndef OBJECT_HEADER
#define OBJECT_HEADER
#include "ball.h"
#include "map.h"
#include "player.h"
#endif
#ifndef TOOLS_H
#define TOOLS_H
#include "tools.h"
#endif

#include <iostream>

using namespace std;

typedef struct Tile {
  int l;
  int c;
} Tile;

class Path {
public:
  Path(int nbCell);
  int getParent(int child);
  Tile getTile(int TileIndex);

private:
  int **way;
};

Path::Path(int nbCell) {
  int size = nbCell * nbCell;
  way = new int *[size];
  for (int i = 0; i < size; i++) {
    way[i] = new int[3];
    for (int j = 0; j < 3; j++) {
      way[i][j] = -1;
    }
  }
}

int nearestPlayer(PlayerMap *players, int index);
bool openGridArea(bool **openGrid, bool **closedGrid, int l, int c);
void setupOpenGrid(bool **openGrid, Map *mainMap);
Tile lowestScoreTile(double **scoreGrid);
bool isInGrid(double **scoreGrid, int l, int c);
double TileScore(Player *players, int index, int enemyIndex, bool **openGrid);
double distanceCost(PlayerMap *players, int index);
double distanceApprox(PlayerMap *players, int enemyIndex);
bool emptyOpenGrid(bool **openGrid, int tabsize);

double pathAngle(PlayerMap *players, int index, Map *obstacles) {
  double angle = 0;
  int nearest = nearestPlayer(players, index);
  int LNb = players->getLNb(), CNb = players->getCNb();
  double **scoreGrid = new double *[LNb]; // call LNb and CNb outside !
  for (int i = 0; i < LNb; i++) {
    scoreGrid[i] = new double[CNb];
    for (int j = 0; j < CNb; j++) {
      scoreGrid[i][j] = LNb * sqrt(2); // Compute outside !
    }
  }
  bool **openGrid = new bool *[LNb];
  for (int i = 0; i < LNb; i++) {
    openGrid[i] = new bool[CNb];
    for (int j = 0; j < CNb; j++) {
      openGrid[i][j] = true;
    }
  }

  Tile me;
  me.l = players->getPlayer(index)->getL();
  me.c = players->getPlayer(index)->getC();
  Tile enemy;
  enemy.l = players->getPlayer(nearest)->getL();
  enemy.c = players->getPlayer(nearest)->getC();
  Tile current;
  current.l = me.l;
  current.c = me.c;
  Tile last;

  setupOpenGrid(openGrid, obstacles);

  do { // A* algorithm under construction
    std::cout << "Hello" << std::endl;
    break;
  } while (!openGrid.empty());

  return angle;
}

bool emptyOpenGrid(bool **openGrid, int tabsize) {
  bool isempty = true;
  for (int i = 0; i < tabsize; i++) {
    for (int j = 0; j < tabsize; j++) {
      if (!openGrid[i][j]) {
        isempty = false;
      }
    }
  }

  return isempty;
}

int nearestPlayer(PlayerMap *players, int index) {
  int nearest(-1);
  double distance = players->getLNb() * sqrt(2);

  for (int list = 0; list < players->getNb(); list++) {
    if (index != list) {
      double dist = distance(players->getPlayer(index)->getHitbox(),
                             players->getPlayer(list)->getHitbox());
      if (dist < distance) {
        distance = dist;
        nearest = list;
      }
    }
  }

  return nearest;
}

Tile lowestScoreTile(double **scoreGrid, Tile lastTile, int tabsize) {
  int gridSize = scoreGrid.size();
  double lowestScore = tabsize * sqrt(2);
  Tile lowest = {-1, -1};
  double value;
  for (int radius = 1; radius < gridSize; radius++) {
    for (int i = 0; i < (2 * radius) - 1; i++) {
      if (isInGrid(scoreGrid, l + radius, i)) { // sensé éviter le seg fault.
        value = scoreGrid[l + radius][i];
        if (value < lowestScore) {
          lowest.l = l + radius;
          lowest.c = i;
        }
        if (isInGrid(scoreGrid, i, l + radius)) { // sensé éviter le seg fault.
          value = scoreGrid[i][l + radius];
          if (value < lowestScore) {
            lowest.l = i;
            lowest.c = l + radius;
          }
        }
      }
    }
    return lowest;
  }

  bool isInGrid(double **scoreGrid, int l, int c) {
    int height = scoreGrid.size();
    int width = scoreGrid[].size();
    if ((l < heigt) and (c < width)) {
      return true;
    }
    return false;
  }

  bool openGridArea(bool **openGrid, bool **closedGrid, int l, int c) {}

  void setupOpenGrid(bool **openGrid, Map *mainMap) {
    for (auto line : openGrid) {
      for (auto column : line) {
        if (mainMap->isObstacle(line, column)) {
          openGrid[line][column] = false;
        }
      }
    }
  }
