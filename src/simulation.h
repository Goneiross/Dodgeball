/*!
  \file   simulation.h
  \author Guillaume Pellerin & Vincent Miche
  \date   March 2019
  \brief  "Simulation" module header
*/

#ifndef OBJECT_HEADER
#define OBJECT_HEADER
#include "player.h"
#include "ball.h"
#include "map.h"
#endif

void simulation(std::string inputFile, int mode);
void save(std::string filename, int nbCell, PlayerMap* players, Map* mainMap, BallMap* balls);
void initialization(std::string inputFile, PlayerMap* &players, Map *&mainMap, BallMap* &balls) ;
bool isOut(double xPosision, double yPosition);
void check(BallMap* &balls, PlayerMap* &players);
void update(BallMap* &balls, PlayerMap* &players);
//bool isOut(Obstacle* obstacle);