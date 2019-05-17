/*!
  \file   pathfinding.h
  \author Guillaume Pellerin & Vincent Miche
  \date   May 2019
  \brief  pathfinding header
*/

#ifndef PATHFINDING_H
#define PATHFINDING_H

#include "obstacle.h"
#include "player.h"

void pathfinding(PlayerMap *players, ObstacleMap *obstacles);
double nearestPlayerAngle(int ID);

#endif