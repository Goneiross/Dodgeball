/*!
  \file   player.cc
  \author Guillaume Pellerin & Vincent Miche
  \date   March 2019
  \brief  "Player" module implementation
*/

#include <math.h>
#include <iostream>
#include <vector>
#include "player.h"
#ifndef TOOLS_H
#define TOOLS_H
#include "tools.h"
#endif
#include "define.h"

using namespace std;

Player::Player(double x0, double y0, int t, double c, double r, 
              	int line, int column, double v, int i) {
  	timeTouched = t;
  	ID = i;
  	count = c;
  	velocity = v;
  	lgnPos = line;
  	colPos = column;
  	gXPosition = -1;
  	gYPosition = -1;
  	hitbox = new Circle(x0, y0, r);
}

void Player::updatePosition(double angle) {
  	hitbox->setX(hitbox->getX() + cos(angle) * velocity);
  	hitbox->setY(hitbox->getY() + sin(angle) * velocity);
}

bool Player::touchedAndDead() {
  	timeTouched += 1;
  	if (timeTouched == MAX_TOUCH) { // Dead when equal or sup ? //#askBoulic
    	return (true);
  	}
  	return (false);
}

double Player::getX() const { return hitbox->getX(); }
double Player::getY() const { return hitbox->getY(); }
double Player::getL() const { return lgnPos; }
double Player::getC() const { return colPos; }
double Player::getGX() const { return gXPosition; }
double Player::getGY() const { return gYPosition; }
void Player::setGX(double gX) { gXPosition = gX; }
void Player::setGY(double gY) { gYPosition = gY; }
double Player::getRadius() const { return hitbox->getRadius(); }
double Player::getCount() const { return count; }
void Player::setCount(int c) {count = c;}
double Player::getTimeTouched() const { return timeTouched; }
void Player::setTimeTouched(int t) {timeTouched = t;}

Circle* Player::getHitbox() const { return hitbox; }

PlayerMap::PlayerMap(int l, int c){
  	lineNumber = l;
  	columnNumber = c;
  
  	playerGrid = new vector<int>* [lineNumber];
  	for (int i = 0; i < lineNumber; i++) {
    	playerGrid[i] = new vector<int>[columnNumber];
    	for (int j = 0; j < columnNumber; j++) {
      		playerGrid[i][j].push_back(-1);
    	}
  	}
}

void PlayerMap::addPlayer(double x, double y, int t, double c, double r, double v, 
							int ID){
  	int colPos = ((x + DIM_MAX ) / (SIDE / lineNumber) ) - 1 / 2;
  	int lgnPos = - ((y - DIM_MAX) / (SIDE / lineNumber) ) - 1 / 2;
  	players.push_back(new Player(x, y, t, c, r, lgnPos, colPos, v, ID));
  	if (playerGrid[lgnPos][colPos][0] == -1){
    	playerGrid[lgnPos][colPos][0] = ID;
  	} else {
    	playerGrid[lgnPos][colPos].push_back(ID);
  	}
}

void PlayerMap::removePlayer(int ID){
  	int l = players[ID]->getL();
  	int c = players[ID]->getC();
  	int posSize = playerGrid[l][c].size();
  	if (playerGrid[l][c][0] == ID){
      	playerGrid[l][c][0] = -1;
  	} else {
    	for (int i = 1; i < posSize; i++){
      		if (playerGrid[l][c][i] == ID){
        		playerGrid[l][c].erase(playerGrid[l][c].begin()+i);
      		}
    	}
  	}
  	players.erase(players.begin()+ ID );
}

void PlayerMap::removeAll(){
  	int playerNb = players.size();
  	for (int i = 0; i < playerNb; i++){
    	removePlayer(0);
  	}
}

bool PlayerMap::isPlayer(int lgnPos, int colPos){
  	if (playerGrid[lgnPos][colPos][0] == -1){
    	return false;
  	} else {
    	return true;
  	}
}

vector<int> PlayerMap::whichPlayer(int lgnPos, int colPos){
  	return playerGrid[lgnPos][colPos];
}

void PlayerMap::reserveSpace(int nbPlayer){
  	players.reserve(nbPlayer);
}

Player* PlayerMap::getPlayer(int p) const {
  	return players[p];
}

int PlayerMap::getNb() const{
  	return players.size();
}

void PlayerMap::updatePosition(){
  	for (int i = 0; i < players.size(); i++){
    	int c = players[i]->getCount();
    	if (c == MAX_COUNT){
      		players[i]->setCount(0);
    	} else {
      		players[i]->setCount(c+1);
    	}
  	}
  	//TO do
}