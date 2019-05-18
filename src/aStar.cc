/*!
  \file   aStar.cc
  \author Guillaume Pellerin & Vincent Miche
  \date   May 2019
  \brief  add description
*/

#include <math.h>
#include <vector>
#include "obstacle.h"
#include "player.h"
#include "tools.h"

#include <iostream>

using namespace std;

typedef struct Tile {
  int l;
  int c;
} Tile;

class Node {
  public :
    Node(Node* from, Tile at);
    Node(Node* const& copy);
    ~Node(){parent = nullptr;};
    Tile position;
    Node* parent;
    int totalCost;
    int endCost;
    int beginCost;
};
Node::Node(Node* from, Tile at)
  : parent (from),
    position (at),
    totalCost(0),
    endCost(0),
    beginCost(0) {}

Node::Node(Node* const& copy)
  : parent (copy->parent),
    position (copy->position),
    totalCost(copy->totalCost),
    endCost(copy->endCost),
    beginCost(copy->beginCost) {}

double whichPath(PlayerMap *players, int startID, ObstacleMap* obstacles) {
  double pathAngle = 0;
  int targetID = nearestPlayer(players, startID);
  int LNb = players->getLNb(), CNb = players->getCNb();

  double **scoreGrid = new double *[LNb]; // call LNb and CNb outside !
  for (int i = 0; i < LNb; i++) {
    scoreGrid[i] = new double[CNb];
    for (int j = 0; j < CNb; j++) {
      scoreGrid[i][j] = LNb * sqrt(2); // maybe squared ? Do outisde
    }
  }
  bool **openGrid = new bool *[LNb];
  for (int i = 0; i < LNb; i++) {
    openGrid[i] = new bool[CNb];
    for (int j = 0; j < CNb; j++) {
      openGrid[i][j] = true;
    }
  }

  Node* start = new Node(nullptr,{players->getPlayer(startID)->getL(),players->getPlayer(startID)->getC()});
  Node* target = new Node(nullptr,{players->getPlayer(targetID)->getL(),players->getPlayer(targetID)->getC()});
  Node* current;

  vector<Node*> openList;
  vector<Node*> closedList;
  vector<Tile> path;
  openList.push_back(start);

  while (openList.size() > 0) {
    current = new Node(openList[0]);
    int currentIndex = 0;
    for (int i = 0; i < openList.size(); i++){
      if(openList[i]->totalCost < current->totalCost){
        current = new Node(openList[i]);
        currentIndex = i;
      }
    }
    openList.erase(openList.begin() + currentIndex);
    closedList.push_back(current);

    if (current == target){
      Node* back = new Node(current);
      while(back->parent != nullptr){
        path.push_back(back->position);
        back = new Node(current->parent);
      }
      // END
      delete back;
    }
    vector<Node*> children;
    for (int i = -1; i <= 1; i++){
      for (int j = -1; j <= 1; j++){
        if (not (i == 0 && j == 0)){
          Tile newPosition = {current->position.l + i, current->position.c + j};
          if (newPosition.l < 0 || newPosition.l > obstacles->getLNb()){continue;} // > or => ???
          if (newPosition.c < 0 || newPosition.c > obstacles->getCNb()){continue;} // > or => ???
          if(obstacles->isObstacle(newPosition.l,newPosition.c)){continue;}
          children.push_back(new Node(current, newPosition));
        }
      }
    }
    for (int childID = 0; childID < children.size(); childID++){
      for (int closedChildID = 0; closedChildID < closedList.size(); closedChildID++){
        if (children[childID] == closedList[closedChildID]){continue;}
      }
      double lDistance = children[childID]->position.l - target->position.l;
      double cDistance = children[childID]->position.c - target->position.c;
      children[childID]->beginCost = current->beginCost + 1;
      children[childID]->endCost = pow(lDistance, 2) + pow(cDistance, 2);
      children[childID]->totalCost = children[childID]->beginCost + children[childID]->endCost;
    }
  }
  int nextStepID= path.size() - 1;
  pathAngle = angle(path[nextStepID].c, path[nextStepID].l, path[nextStepID].c -1, path[nextStepID].l -1);
  cout << "PATH ANGLE" << pathAngle << endl;
  delete start;
  delete target;
  delete current;
  return pathAngle;
}

int nearestPlayer(PlayerMap *players, int ID) {
  int nearestID = -1;
  double nearestDistance = players->getLNb() * 2; // better than sqrt(2)
  for (int p = 0; p < players->getNb(); p++) {
    if (ID != p) {
      double dist = distance(players->getPlayer(ID)->getHitbox(),
                             players->getPlayer(p)->getHitbox());
      if (dist < nearestDistance) {
        nearestDistance = dist;
        nearestID = p;
      }
    }
  }
  return nearestID;
}


/*
bool isGridEmpty(bool ** grid, ObstacleMap* obstacles) {
  for (int l = 0; l < obstacles->getLNb(); l++) {
    for (int c = 0; c < obstacles->getCNb(); c++) {
      if (grid[l][c]) {
        return false;
      }
    }
  }
  return true;
}

Tile lowestScoreTile(double **scoreGrid, Tile lastTile, int tabsize, ObstacleMap* obstacles) {
  double lowestScore = tabsize * sqrt(2);
  Tile lowest = {-1, -1};
  double value;
  for (int radius = 1; radius < gridSize; radius++) {
    for (int i = 0; i < (2 * radius) - 1; i++) {
      if (isInGrid(scoreGrid, l + radius, i)) { // sensé éviter le seg fault.
        value = scoreGrid[l + radius][i];
        if (value < lowestScore) {
          lowest.l = l + radius;
          lowest.c = i;
        }
        if (isInGrid(scoreGrid, i, l + radius)) { // sensé éviter le seg fault.
          value = scoreGrid[i][l + radius];
          if (value < lowestScore) {
            lowest.l = i;
            lowest.c = l + radius;
          }
        }
      }
    }
    return lowest;
  }

  bool isInGrid(double **scoreGrid, int l, int c) {
    int height = scoreGrid.size();
    int width = scoreGrid[].size();
    if ((l < heigt) and (c < width)) {
      return true;
    }
    return false;
  }

  bool openGridArea(bool **openGrid, bool **closedGrid, int l, int c) {}

  void setupOpenGrid(bool** &openGrid, ObstacleMap* obstacles) {
    for (int l = 0; l < obstacles->getLNb(); l++) {
      for (int c = 0; c < obstacles->getCNb(); c++) {
        if (obstacles->isObstacle(l, c)) {
          openGrid[l][c] = false;
        }
      }
    }
  }
*/