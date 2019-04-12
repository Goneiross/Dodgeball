/*!
  \file   ball.cc
  \author Guillaume Pellerin & Vincent Miche
  \date   March 2019
  \brief  Ball module implementation
*/

#include<vector>
#include<math.h>
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

typedef square int[2];

double pathAngle (PlayerMap* players, int index, Map* obstacles )
{
  int nearest = nearestPlayer(players,index);
  double scoreGrid[players->getLNb()][players->getCNb()]
    = {players->getLNb()*sqrt(2)};
  bool openGrid[players->getLNb()][players->getCNb()] = {};
  bool closedGrid[players->getLNb()][players->getCNb()] = {};
  square me;
  me[0] = players->getPlayer(index)->getL();
  me[1] = players->getPlayer(index)->getC();
  square enemy[2];
  enemy[0] = players->getPlayer(nearest)->getL();
  enemy[1] = players->getPlayer(nearest)->getC();

  setupClosedGrid(closedGrid,mainMap);
  closedGrid[me[0]][me[1]] = true;

  do{
    // A* algorithm under construction
  }while(!openGrid.empty())

  return angle;
}

int nearestPlayer (PlayerMap* players, int index)
{
  int nearest(-1);
  double distance = players->getLNb()*sqrt(2);

  for (int list = 0; list < players->getNb(); list++) {
    if(index != list){
      double dist = distance(players->getPlayer(index)->getHitbox(),
                             players->getPlayer(list)->getHitbox());
      if(dist < distance){
        distance = dist;
        nearest = list;
      }
    }
  }

  return nearest;
}

bool openGridArea (bool[][] openGrid, bool[][] closedGrid, int l,int c)
{

}
void setupClosedGrid(bool[][] closedGrid , Map* mainMap)
{
  for(auto line : closedGrid){
    for(auto column : line){
      if(mainMap->isObstacle(line,column)){
        openGrid[line][column] = true;
      }
    }
  }
}
