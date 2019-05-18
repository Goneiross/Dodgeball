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
#include "define.h"

#include <iostream>

using namespace std;


typedef struct Tile {
  int l;
  int c;
} Tile;

bool operator==(Tile t1, Tile t2){
  if (t1.c == t2.c && t1.l == t2.l){
    return true;
  } else {
    return false;
  }
}

class Node {
  public :
    Node(Tile at);
    Node(Node* from, Tile at);
    Node(Node* const& copy);
    ~Node(){parent = nullptr;};
    Tile position;
    Node* parent;
    int totalCost;
    int endCost;
    int beginCost;
};
Node::Node(Tile at)
  : position (at),
    totalCost(0),
    endCost(0),
    beginCost(0) {parent = nullptr;}

Node::Node(Node* from, Tile at)
  : position (at),
    totalCost(0),
    endCost(0),
    beginCost(0) {parent = from;}

Node::Node(Node* const& copy)
  : parent (copy->parent),
    position (copy->position),
    totalCost(copy->totalCost),
    endCost(copy->endCost),
    beginCost(copy->beginCost) {}

/*
Node& Node::operator=(Node const& copy) {
    if(this != &copy) {
        position = copy.position;
        totalCost = copy.totalCost;
        endCost = copy.endCost;
        beginCost = copy.beginCost;
        delete parent;
        parent = new Node(copy.parent);
    }
    return *this;
} */

int nearestPlayer(PlayerMap *players, int ID);

double whichPath(PlayerMap *players, int startID, int targetID, ObstacleMap* obstacles) {
  std::cout << "-----aStar-----" << endl;
  double pathAngle = 0;
  // int targetID = nearestPlayer(players, startID);
  int LNb = players->getLNb(), CNb = players->getCNb();
  Node* start = new Node ({players->getPlayer(startID)->getL(),players->getPlayer(startID)->getC()});
  Node* target = new Node({players->getPlayer(targetID)->getL(),players->getPlayer(targetID)->getC()});

  Node* current(start);

  vector<Node*> openList;
  vector<Node*> closedList;
  vector<Tile> path;
  openList.push_back(start);

  std::cout << "Start " << startID << "(" << players->getPlayer(startID)->getL() << "," << players->getPlayer(startID)->getC() << ")" << endl;
  cout << "BEGIN" << endl;
  do {
    /*
    for (int i = 0; i < openList.size(); i++){
      Node back = openList[i];
      while(not (back.parent->position == start.position) && i < 10){
        i++;
        cout << back.parent->position.l << " " << back.parent->position.c << endl;
        path.push_back(back.position);
        back = back.parent;
      }
    }
    */
    current = openList[0];
    int currentIndex = 0;
    for (int i = 0; i < openList.size(); i++){
      if(openList[i]->totalCost < current->totalCost){
        current = openList[i];
        currentIndex = i;
      }
    }
    // cout << "Current: " << current->position.l << " " << current->position.c << endl;
    openList.erase(openList.begin() + currentIndex);
    closedList.push_back(current);
    if (current->position == target->position){
      // cout << "Current = target" << endl;
      Node* back = current;
      while(not (back->parent->position == start->position)){
        // cout << "Back: " << back->position.l << " " << back->position.c << " ";
        // cout << "Parent: " << back->parent->position.l << " " << back->parent->position.c << endl;
        path.push_back(back->parent->position);
        back = back->parent;
      }
      cout << "END" << endl;
      break;
    }
    vector<Node*> children;
    for (int i = -1; i <= 1; i++){
      for (int j = -1; j <= 1; j++){
        if (not(abs(i) == abs(j))){
          Tile newPosition = {current->position.l + i, current->position.c + j};
          if (newPosition.l < 0 || newPosition.l >= obstacles->getLNb()){continue;} // > or => ???
          if (newPosition.c < 0 || newPosition.c >= obstacles->getCNb()){continue;} // > or => ???
          if(obstacles->isObstacle(newPosition.l,newPosition.c)){continue;}
          children.push_back(new Node(current, newPosition));
          // cout << "PUSHED: " << children[children.size()-1]->position.l << " "  << children[children.size()-1]->position.c;
          // cout << " FROM: " << children[children.size()-1]->parent->position.l << " " << children[children.size()-1]->parent->position.c << endl;
        }
      }
    }
    for (int childID = 0; childID < children.size(); childID++){
      for (int closedChildID = 0; closedChildID < closedList.size(); closedChildID++){
        if (children[childID]->position == closedList[closedChildID]->position){continue;}
      }
      double lDistance = children[childID]->position.l - target->position.l;
      double cDistance = children[childID]->position.c - target->position.c;
      children[childID]->beginCost = current->beginCost + 1;
      children[childID]->endCost = pow(lDistance, 2) + pow(cDistance, 2);
      children[childID]->totalCost = children[childID]->beginCost + children[childID]->endCost;

      for (int openChildID = 0; openChildID < openList.size(); openChildID++){
        if (children[childID]->position == openList[openChildID]->position && children[childID]->beginCost > openList[openChildID]->beginCost){continue;}
      }

      openList.push_back(children[childID]);
    }
  } while (openList.size() > 0);
  cout << "DONE" << endl;
  int nextStepID= path.size() - 1;
  // cout << nextStepID << endl;
  if (nextStepID < 0){return angle(players->getPlayer(startID)->getX(), players->getPlayer(startID)->getY(), players->getPlayer(targetID)->getX(), players->getPlayer(targetID)->getY());}
  cout << "Going to " << path[nextStepID].l << " " << path[nextStepID].c << endl;
  double S = SIDE / obstacles->getLNb();
  double X = (0.5 * S + path[nextStepID].c * S) - (DIM_MAX);
  double Y = -(0.5 * S + path[nextStepID].l * S) + (DIM_MAX);
  pathAngle = angle(players->getPlayer(startID)->getX(), players->getPlayer(startID)->getY(), X, Y);
  // std::cout << "PATH ANGLE" << pathAngle << endl;
  // cout << "RETURN" << endl;
  return pathAngle;
}

int nearestPlayer(PlayerMap *players, int ID) {
  int nearestID = -1;
  double nearestDistance = SIDE * 2; // better than sqrt(2)
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