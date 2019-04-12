/*!
  \file   pathfinding.h
  \author Guillaume Pellerin & Vincent Miche
  \date   April 2019
  \brief  Pathfinding module implementation
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


double pathAngle (PlayerMap* players, int index, Map* obstacles );
int nearestPlayer (PlayerMap* players, int index);
bool openGridArea (bool[][] openGrid, bool[][] closedGrid, int l,int c);
void setupClosedGrid(bool[][] closedGrid , Map* mainMap);

double distanceCost(PlayerMap* players, int index);
double distanceApprox(PlayerMap* players, int enemyIndex);
