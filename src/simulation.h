/*!
  \file   simulation.h
  \author Guillaume Pellerin & Vincent Miche
  \date   March 2019
  \brief  "Simulation" module header
*/

#ifndef SIMULATION_H
#define SIMULATION_H

#include <string>

#include "tools.h"

void simulation(int mode, std::string inputFile, std::string saveFile = "");
void newGame();
void save(std::string filename);
bool initialization(std::string inputFile, int mode);
bool isOut(double xPosision, double yPosition);
bool check();
void updateBalls();
bool updatePlayers();
void fireBall();

int getPlayerNb();
int getPlayerLife(int p);
double getPlayerRadius();
int getPlayerCount(int p);
Circle *getPlayerHitbox(int p);

int getBallNb();
double getBallRadius();
Circle *getBallHitbox(int b);

int getObstacleNb();
double getObstacleSize();
Square *getObstacleHitbox(int o);

#endif