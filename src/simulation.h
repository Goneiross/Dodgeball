/*!
  \file   simulation.h
  \author Guillaume Pellerin & Vincent Miche
  \date   March 2019
  \brief  "Simulation" module header
*/

#ifndef SIMULATION_H
#define SIMULATION_H
#include "tools.h"
#include <string>

void simulation(std::string inputFile, int mode);
void simulation(std::string inputFile, std::string saveFile, int mode);
void save(std::string filename);
bool initialization(std::string inputFile, int mode);
bool isOut(double xPosision, double yPosition);
void check();
void updateBalls();
void updatePlayers();
void dracarys();

int getPlayerNb();
int getPlayerTimeTouched(int p);
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