/*!
  \file   player.cc
  \author Guillaume Pellerin & Vincent Miche
  \date   March 2019
  \brief  "Player" module implementation
*/

#include <iostream>
#include <math.h>
#include <vector>

#include "define.h"
#include "pathfinding.h"
#include "player.h"
#include "tools.h"
#include "map.h"

using namespace std;

Player::Player(double x0, double y0, int t, double c, double r, int line, int column,
               double v, int i) {
    life = t;
    ID = i;
    count = c;
    velocity = v;
    lgnPos = line;
    colPos = column;
    gXPosition = -1;
    gYPosition = -1;
    hitbox = new Circle(x0, y0, r);
}

bool Player::touchedAndDead() {
    life += 1;
    if (life == MAX_TOUCH) {
        return (true);
    }
    return (false);
}

double Player::getX() const { return hitbox->getX(); }
double Player::getY() const { return hitbox->getY(); }
int Player::getL() const { return lgnPos; }
int Player::getC() const { return colPos; }
double Player::getGX() const { return gXPosition; }
double Player::getGY() const { return gYPosition; }
int Player::getID() const { return ID; }
double Player::getVelocity() const { return velocity; }
void Player::setGX(double gX) { gXPosition = gX; }
void Player::setGY(double gY) { gYPosition = gY; }
void Player::setL(int l) { lgnPos = l; }
void Player::setC(int c) { colPos = c; }
double Player::getRadius() const { return hitbox->getRadius(); }
double Player::getCount() const { return count; }
void Player::setCount(int c) { count = c; }
double Player::getLife() const { return life; }
void Player::setLife(int t) { life = t; }

Circle *Player::getHitbox() const { return hitbox; }

PlayerMap::PlayerMap(int l, int c) : Map(l, c) {
    playerGrid = new vector<int> *[lineNumber];
    for (int i = 0; i < lineNumber; i++) {
        playerGrid[i] = new vector<int>[columnNumber];
        for (int j = 0; j < columnNumber; j++) {
            playerGrid[i][j].push_back(-1);
        }
    }
}

void PlayerMap::addPlayer(double x, double y, int t, double c, double r, double v,
                          int ID) {
    int colPos = ((x + DIM_MAX) / (SIDE / lineNumber)) - 1 / 2;
    int lgnPos = -((y - DIM_MAX) / (SIDE / lineNumber)) - 1 / 2;
    players.push_back(new Player(x, y, t, c, r, lgnPos, colPos, v, ID));
    if (playerGrid[lgnPos][colPos][0] == -1) {
        playerGrid[lgnPos][colPos][0] = ID;
    } else {
        playerGrid[lgnPos][colPos].push_back(ID);
    }
}

void PlayerMap::removePlayer(int ID) {
    int l = players[ID]->getL();
    int c = players[ID]->getC();
    int posSize = playerGrid[l][c].size();
    if (playerGrid[l][c][0] == ID) {
        playerGrid[l][c][0] = -1;
    } else {
        for (int i = 1; i < posSize; i++) {
            if (playerGrid[l][c][i] == ID) {
                playerGrid[l][c].erase(playerGrid[l][c].begin() + i);
            }
        }
    }
    players.erase(players.begin() + ID);
}

void PlayerMap::removeAll() {
    int playerNb = players.size();
    for (int i = 0; i < playerNb; i++) {
        removePlayer(0);
    }
}

bool PlayerMap::isPlayer(int lgnPos, int colPos) {
    if (playerGrid[lgnPos][colPos][0] == -1) {
        return false;
    } else {
        return true;
    }
}

bool PlayerMap::isDifferentPlayer(int lgnPos, int colPos, int ID) {
    if (playerGrid[lgnPos][colPos][0] == -1) {
        return false;
    } else if (playerGrid[lgnPos][colPos][0] == ID &&
               playerGrid[lgnPos][colPos].size() == 1) {
        return false;
    } else {
        return false;
    }
}

vector<int> PlayerMap::whichPlayer(int lgnPos, int colPos) {
    return playerGrid[lgnPos][colPos];
}

void PlayerMap::reserveSpace(int nbPlayer) { players.reserve(nbPlayer); }

Player *PlayerMap::getPlayer(int p) const { return players[p]; }

int PlayerMap::getNb() const { return players.size(); }

int PlayerMap::getLNb() const { return lineNumber; }
int PlayerMap::getCNb() const { return columnNumber; }

bool PlayerMap::isCollision(int newX, int newY, int ID, double delta) {
    for (int i = 0; i < players.size(); i++) {
        if (i != ID) {
            double d = distanceAbs(players[i]->getX(), players[i]->getY(), newX, newY);
            if (d < (delta + players[i]->getRadius() +
                     players[ID]->getRadius())) { 
                return true;
            }
        }
    }
}

void PlayerMap::updatePosition() {
    double delta = COEF_MARGE_JEU * (SIDE / (double)lineNumber);
    for (int p = 0; p < players.size(); p++) {
        if (players[p]->getCount() != MAX_COUNT) {
            players[p]->setCount(players[p]->getCount() + 1);
        }
        double angle = nearestPlayerAngle(p);
        double newX = players[p]->getX() + cos(angle) * players[p]->getVelocity();
        double newY = players[p]->getY() + sin(angle) * players[p]->getVelocity();
        int newC = ((newX + DIM_MAX) / (SIDE / lineNumber)) - 1 / 2;
        int newL = -((newY - DIM_MAX) / (SIDE / lineNumber)) - 1 / 2;
        
        if (newX > (DIM_MAX - players[p]->getHitbox()->getRadius()) ||
            newX < (-DIM_MAX + players[p]->getHitbox()->getRadius())) {continue;} // Et les marges ?
        if (newY > (DIM_MAX - players[p]->getHitbox()->getRadius()) ||
            newY < (-DIM_MAX + players[p]->getHitbox()->getRadius())) {continue;} // Et les marges ?
        if (isDifferentPlayer(newL, newC, players[p]->getID())) {continue;}
        if (isCollision(newX, newY, p, delta)) {continue;}
        players[p]->setL(newL); players[p]->setC(newC);
        players[p]->getHitbox()->setX(newX); players[p]->getHitbox()->setY(newY);

        if (newL < lineNumber && newL >= 0 && newC < columnNumber && newC >= 0) {
            int ID = players[p]->getID();
            if (playerGrid[newL][newC][0] == ID) {
                playerGrid[newL][newC][0] = -1;
            } else {
                for (int i = 1; i < playerGrid[newL][newC].size(); i++) {
                    if (playerGrid[newL][newC][i] == ID) {
                       playerGrid[newL][newC].erase(playerGrid[newL][newC].begin() +i);
                    }
                }
            }
            if (playerGrid[newL][newC][0] == -1) {playerGrid[newL][newC][0] = ID;
            } else { playerGrid[newL][newC].push_back(ID);}
        }
    }
}
