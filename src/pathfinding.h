/*!
  \file   pathfinding.h
  \author Guillaume Pellerin & Vincent Miche
  \date   May 2019
  \brief  pathfinding header
*/

#ifndef PATHFINDING_H
#define PATHFINDING_H

#include "define.h"
#include "obstacle.h"
#include "player.h"

bool pathfinding(PlayerMap *players, ObstacleMap *obstacles, int type = ASTAR);
double nearestPlayerAngle(int ID);
bool isReadyToFire(int ID, PlayerMap *players, ObstacleMap *obstacles);

#endif