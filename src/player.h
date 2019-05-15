/*!
  \file   player.h
  \author Guillaume Pellerin & Vincent Miche
  \date   March 2019
  \brief  "Player" module header
*/

#ifndef PLAYER_H
#define PLAYER_H
#include <vector>
#include "tools.h"
#include "map.h"


class Player {
public:
		Player(double xPosition, double yPosition, int timeTouched, double counter,
  	       	double radius, int lgnPos, int colPos, double velocity, int ID); 
  	~Player(){};

  	bool touchedAndDead();

  	double getX() const;
  	double getY() const;
  	double getL() const;
 		double getC() const;
  	double getGX() const;
  	double getGY() const;
		int getID() const;
  	double getRadius() const;
  	double getCount() const;
  	double getTimeTouched() const;
	  Circle *getHitbox() const;
		
    void setL(int l);
    void setC(int c);
  	void setGX(double gY);
  	void setGY(double gY);
  	void setCount(int c);
		void setTimeTouched(int t);

  	void updatePosition(double angle);

private:
  	int timeTouched;
  	int ID;
  	int lgnPos, colPos;
  	double count;
  	double velocity;
  	double gXPosition, gYPosition;
  	Circle *hitbox;
};

class PlayerMap : public Map {
public:
    PlayerMap(int lgnPos, int colPos);
    ~PlayerMap(){};
    void addPlayer(double xPosition, double yPosition, int timeTouched, double counter,
                   	double radius, double velocity, int ID);
    void removePlayer(int ID);
    void removeAll();
    void reserveSpace(int nbPlayer);
    bool isPlayer(int lgnPos, int colPos);
    std::vector<int> whichPlayer(int lgnPos, int colPos);
    Player* getPlayer(int p) const;
    int getNb() const;
    void updatePosition();

private:
    std::vector<Player *> players;
    std::vector<int>** playerGrid;
};

#endif