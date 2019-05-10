/*!
  \file   aStar.h
  \author Guillaume Pellerin & Vincent Miche
  \date   May 2019
  \brief  add description
*/

#ifndef OBJECT_HEADER
#define OBJECT_HEADER
#include "player.h"
#include "ball.h"
#include "map.h"
#endif
#ifndef TOOLS_H
#define TOOLS_H
#include "tools.h"
#endif

typedef tile int[2];

double pathAngle (PlayerMap* players, int index, Map* obstacles );
<<<<<<< HEAD
=======
int nearestPlayer (PlayerMap* players, int index);
bool openGridArea (bool[][] openGrid, bool[][] closedGrid, int l,int c);
void setupOpenGrid(bool[][] openGrid , Map* mainMap);
tile lowestScoreTile(double[][] scoreGrid);
bool isInGrid(double[][] scoreGrid,int l, int c);

double tileScore(Player* players, int index, int enemyIndex, bool[][] openGrid)
double distanceCost(PlayerMap* players, int index);
double distanceApprox(PlayerMap* players, int enemyIndex);
>>>>>>> 24bbf58a6ea07a1c6217372d50a97a83a87871d1
