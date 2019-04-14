/*!
  \file   ball.cc
  \author Guillaume Pellerin & Vincent Miche
  \date   March 2019
  \brief  Ball module implementation
*/

// GOOD LUCK CORRECTING MY CODE

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

typedef tile int[2];

class Path {
public:
  Path(){
    path = {-1};
  }
  int getParent(int child);
  tile getTile(int tileIndex);

private:
//voulais: int[taille map n^2][], pas sûr de mon code.
//logique: path[n]=>(parent,coord l,coord c)
  int[(Map*->getLNb())*(Map*->getLNb())][3] path;
}

double pathAngle (PlayerMap* players, int index, Map* obstacles )
{
  int nearest = nearestPlayer(players,index);
  double scoreGrid[players->getLNb()][players->getCNb()]
    = {players->getLNb()*sqrt(2)};
  bool openGrid[players->getLNb()][players->getCNb()] = {true};
  tile me;
  me[0] = players->getPlayer(index)->getL();
  me[1] = players->getPlayer(index)->getC();
  tile enemy[2];
  enemy[0] = players->getPlayer(nearest)->getL();
  enemy[1] = players->getPlayer(nearest)->getC();
  tile current;
  current[0] = me[0];
  current[1] = me[1];
  tile last;

  setupClosedGrid(closedGrid,mainMap);

  do{                                       // A* algorithm under construction

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

tile lowestScoreTile(double[][] scoreGrid, tile lastTile)
{
  int gridSize = scoreGrid.size();
  double lowestScore = scoreGrid*sqrt(2);
  tile lowest = {-1,-1};
  double value;
  for (int radius = 1; radius < gridSize; radius++) {
    for (int i = 0; i < (2*radius)-1 ; i++) {
      if(isInGrid(scoreGrid,l+radius,i)){       //sensé éviter le seg fault.
        value = scoreGrid[l+radius][i];
        if(value < lowestScore){
          lowest[0] = l+radius;
          lowest[1] = i;
        }
      if(isInGrid(scoreGrid,i,l+radius)){       //sensé éviter le seg fault.
        value = scoreGrid[i][l+radius];
        if(value < lowestScore){
          lowest[0] = i;
          lowest[1] = l+radius;
        }
      }
    }
  }
  return lowest;
}

bool isInGrid(double[][] scoreGrid,int l, int c)
{
  int height = scoreGrid.size();
  int width = scoreGrid[].size();
  if((l < heigt) and (c < width)){
    return true;
  }
  return false;
}

bool openGridArea (bool[][] openGrid, bool[][] closedGrid, int l,int c)
{

}
void setupOpenGrid(bool[][] openGrid , Map* mainMap)
{
  for(auto line : openGrid){
    for(auto column : line){
      if(mainMap->isObstacle(line,column)){
        openGrid[line][column] = false;
      }
    }
  }
}
