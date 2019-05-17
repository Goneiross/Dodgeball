/*!
  \file   floyd.h
  \author Guillaume Pellerin & Vincent Miche
  \date   May 2019
  \brief  pathfinding, with the Roy-Floyd-Warschall algorithm
*/

#ifndef FLOYD_H
#define FLOYD_H

#include "obstacle.h"
#include "player.h"
#include <vector>

std::vector<int> targetting(PlayerMap *players, int infinityInit, int infinityDist);
double whichPath(Player *start, Player *target, int infinityInit, int infinityDist,
                 ObstacleMap *obstacles, bool &firstInStep);

#endif