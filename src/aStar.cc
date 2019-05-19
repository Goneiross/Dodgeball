/*!
  \file   aStar.cc
  \author Guillaume Pellerin & Vincent Miche
  \date   May 2019
  \brief  A* implementation
*/
#include <iostream>
#include <cmath>
#include <vector>

#include "define.h"
#include "obstacle.h"
#include "player.h"
#include "tools.h"

using namespace std;

typedef struct Tile {
    int l;
    int c;
} Tile;

bool operator==(Tile t1, Tile t2) {
    if (t1.c == t2.c && t1.l == t2.l) {
        return true;
    } else {
        return false;
    }
}

class Node {
  public:
    Node(Tile at, Node* from = nullptr);
    Node(Node* const& copy);
    ~Node() { delete parent; };
    Tile position;
    Node* parent;
    int totalCost;
    int endCost;
    int beginCost;
};

void getBestOpenedNode(vector<Node*>& openedList, vector<Node*>& closedList,
                       Node*& current);
bool won(vector<Tile>& path, Node* current, Node* start, Node* target,
         bool& foundSolution);
void getNewChildren(ObstacleMap* obstacles, vector<Node*>& children, Node* current);
void addChildrenToOpened(vector<Node*>& openedList, vector<Node*> closedList,
                         vector<Node*>& children, Node* current, Node* target);

Node::Node(Tile at, Node* from)
    : position(at), totalCost(0), endCost(0), beginCost(0), parent(from) {}

Node::Node(Node* const& copy)
    : parent(copy->parent), position(copy->position), totalCost(copy->totalCost),
      endCost(copy->endCost), beginCost(copy->beginCost) {}

double whichPath(PlayerMap* players, int startID, int targetID,
                 ObstacleMap* obstacles) {
    double pathAngle = 0;
    Node* start = new Node(
        {players->getPlayer(startID)->getL(), players->getPlayer(startID)->getC()});
    Node* target = new Node(
        {players->getPlayer(targetID)->getL(), players->getPlayer(targetID)->getC()});
    Node* current(start);
    vector<Node*> openedList;
    vector<Node*> closedList;
    vector<Tile> path;
    openedList.push_back(start);
    bool foundSolution = false;

    do {
        cout << 1 << endl;
        getBestOpenedNode(openedList, closedList, current);
        if (won(path, current, start, target, foundSolution)) { break; }
        vector<Node*> children;
        getNewChildren(obstacles, children, current);
        addChildrenToOpened(openedList, closedList, children, current, target);
    } while (openedList.size() > 0);

    long int nextStepID = path.size() - 1;
    if (nextStepID < 0) {
        if (foundSolution) {
            return angle(players->getPlayer(startID)->getX(),
                         players->getPlayer(startID)->getY(),
                         players->getPlayer(targetID)->getX(),
                         players->getPlayer(targetID)->getY());
        } else { return NO_SOLUTION; }
    }
    double S = SIDE / obstacles->getLNb();
    double X = (0.5 * S + path[nextStepID].c * S) - (DIM_MAX);
    double Y = -(0.5 * S + path[nextStepID].l * S) + (DIM_MAX);
    pathAngle = angle(players->getPlayer(startID)->getX(),
                      players->getPlayer(startID)->getY(), X, Y);
    return pathAngle;
}

void getBestOpenedNode(vector<Node*>& openedList, vector<Node*>& closedList,
                       Node*& current) {
    current = openedList[0];
    long unsigned int currentIndex = 0;
    for (long unsigned int o = 0; o < openedList.size(); o++) {
        if (openedList[o]->totalCost < current->totalCost) {
            current = openedList[o];
            currentIndex = o;
        }
    }
    openedList.erase(openedList.begin() + currentIndex);
    closedList.push_back(current);
}

bool won(vector<Tile>& path, Node* current, Node* start, Node* target,
         bool& foundSolution) {
    if (current->position == target->position) {
        Node* back = current;
        if (start->position == back->position){foundSolution = true; return true;}
        while (not(back->parent->position == start->position)) {
            path.push_back(back->parent->position);
            back = back->parent;
        }
        foundSolution = true;
        return true;
    }
    return false;
}

void getNewChildren(ObstacleMap* obstacles, vector<Node*>& children, Node* current) {
    for (short i = -1; i <= 1; i++) {
        for (short j = -1; j <= 1; j++) {
            if (not(abs(i) == abs(j))) {
                Tile newPosition = {current->position.l + i, current->position.c + j};
                if (newPosition.l < 0 || newPosition.l >= obstacles->getLNb()) {
                    continue;
                }
                if (newPosition.c < 0 || newPosition.c >= obstacles->getCNb()) {
                    continue;
                }
                if (obstacles->isObstacle(newPosition.l, newPosition.c)) { continue; }
                children.push_back(new Node(newPosition, current));
            }
        }
    }
}

void addChildrenToOpened(vector<Node*>& openedList, vector<Node*> closedList,
                         vector<Node*>& children, Node* current, Node* target) {
    for (long unsigned int childID = 0; childID < children.size(); childID++) {
        bool already = false;
        for (long unsigned int c = 0; c < closedList.size(); c++) {
            if (children[childID]->position == closedList[c]->position) {
                already = true;
            }
        }
        if (not already) {
            double lDistance = children[childID]->position.l - target->position.l;
            double cDistance = children[childID]->position.c - target->position.c;
            children[childID]->beginCost = current->beginCost + 1;
            children[childID]->endCost = pow(lDistance, 2) + pow(cDistance, 2);
            children[childID]->totalCost =
                children[childID]->beginCost + children[childID]->endCost;

            for (long unsigned int o = 0; o < openedList.size(); o++) {
                if (children[childID]->position == openedList[o]->position &&
                    children[childID]->beginCost > openedList[o]->beginCost) {
                    continue;
                }
            }

            openedList.push_back(children[childID]);
        }
    }
}