/*!
  \file   aStar.h
  \author Guillaume Pellerin & Vincent Miche
  \date   May 2019
  \brief  add description
*/

#ifndef ASTAR_H
#define ASTAR_H

#include "ball.h"
#include "obstacle.h"
#include "player.h"
#include "tools.h"

double whichPath(PlayerMap *players, int startID, int targetID, ObstacleMap* obstacles);

#endif