/*!
  \file   aStar.h
  \author Guillaume Pellerin & Vincent Miche
  \date   May 2019
  \brief  add description
*/

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

typedef tile int[2];

double pathAngle(PlayerMap *players, int index, Map *obstacles);
int nearestPlayer(PlayerMap *players, int index);
bool openGridArea(bool[][] openGrid, bool[][] closedGrid, int l, int c);
void setupOpenGrid(bool[][] openGrid, Map *mainMap);
tile lowestScoreTile(double[][] scoreGrid);
bool isInGrid(double[][] scoreGrid, int l, int c);

double tileScore(Player *players, int index, int enemyIndex,
                 bool[][] openGrid) double distanceCost(PlayerMap *players, int index);
double distanceApprox(PlayerMap *players, int enemyIndex);