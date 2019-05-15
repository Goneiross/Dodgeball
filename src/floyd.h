/*!
  \file   floyd.h
  \author Guillaume Pellerin & Vincent Miche
  \date   May 2019
  \brief  pathfinding, with the Roy-Floyd-Warschall algorithm
*/

#ifndef FLOYD_H
#define FLOYD_H

#include "player.h"
#include "obstacle.h"

vector<int> targetting(PlayerMap* players);
double whichPath(Player* start, Player* target, ObstacleMap* obstacles);

#endif