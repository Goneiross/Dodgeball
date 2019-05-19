/*!
  \file   aStar.cc
  \author Guillaume Pellerin & Vincent Miche
  \date   May 2019
  \brief  A* implementation
*/

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
    Node(Tile at);
    Node(Node *from, Tile at);
    Node(Node *const &copy);
    ~Node() { delete parent; };
    Tile position;
    Node *parent;
    int totalCost;
    int endCost;
    int beginCost;
};
Node::Node(Tile at) : position(at), totalCost(0), endCost(0), beginCost(0) {
    parent = nullptr;
}

Node::Node(Node *from, Tile at)
    : position(at), totalCost(0), endCost(0), beginCost(0) {
    parent = from;
}

Node::Node(Node *const &copy)
    : parent(copy->parent), position(copy->position), totalCost(copy->totalCost),
      endCost(copy->endCost), beginCost(copy->beginCost) {}

double whichPath(PlayerMap *players, int startID, int targetID,
                 ObstacleMap *obstacles) {
    double pathAngle = 0;
    int LNb = players->getLNb(), CNb = players->getCNb();
    Node *start = new Node(
        {players->getPlayer(startID)->getL(), players->getPlayer(startID)->getC()});
    Node *target = new Node(
        {players->getPlayer(targetID)->getL(), players->getPlayer(targetID)->getC()});

    Node *current(start);

    vector<Node *> openList;
    vector<Node *> closedList;
    vector<Tile> path;
    openList.push_back(start);

    bool foundSolution = false;

    do {
        current = openList[0];
        int currentIndex = 0;
        for (int i = 0; i < openList.size(); i++) {
            if (openList[i]->totalCost < current->totalCost) {
                current = openList[i];
                currentIndex = i;
            }
        }
        openList.erase(openList.begin() + currentIndex);
        closedList.push_back(current);
        if (current->position == target->position) {
            Node *back = current;
            while (not(back->parent->position == start->position)) {
                path.push_back(back->parent->position);
                back = back->parent;
            }
            foundSolution = true;
            break;
        }
        vector<Node *> children;
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                if (not(abs(i) == abs(j))) {
                    Tile newPosition = {current->position.l + i,
                                        current->position.c + j};
                    if (newPosition.l < 0 || newPosition.l >= obstacles->getLNb()) {
                        continue;
                    }
                    if (newPosition.c < 0 || newPosition.c >= obstacles->getCNb()) {
                        continue;
                    }
                    if (obstacles->isObstacle(newPosition.l, newPosition.c)) {
                        continue;
                    }
                    children.push_back(new Node(current, newPosition));
                }
            }
        }
        for (int childID = 0; childID < children.size(); childID++) {
            bool already = false;
            for (int closedChildID = 0; closedChildID < closedList.size();
                 closedChildID++) {
                if (children[childID]->position ==
                    closedList[closedChildID]->position) {
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

                for (int openChildID = 0; openChildID < openList.size();
                     openChildID++) {
                    if (children[childID]->position ==
                            openList[openChildID]->position &&
                        children[childID]->beginCost >
                            openList[openChildID]->beginCost) {
                        continue;
                    }
                }

                openList.push_back(children[childID]);
            }
        }
    } while (openList.size() > 0);
    int nextStepID = path.size() - 1;
    if (nextStepID < 0) {
        if (foundSolution) {
            return angle(players->getPlayer(startID)->getX(),
                         players->getPlayer(startID)->getY(),
                         players->getPlayer(targetID)->getX(),
                         players->getPlayer(targetID)->getY());
        } else {
            return NO_SOLUTION;
        }
    }
    double S = SIDE / obstacles->getLNb();
    double X = (0.5 * S + path[nextStepID].c * S) - (DIM_MAX);
    double Y = -(0.5 * S + path[nextStepID].l * S) + (DIM_MAX);
    pathAngle = angle(players->getPlayer(startID)->getX(),
                      players->getPlayer(startID)->getY(), X, Y);
    return pathAngle;
}