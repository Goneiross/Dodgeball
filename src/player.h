/*!
  \file   player.h
  \author Guillaume Pellerin & Vincent Miche
  \date   March 2019
  \brief  "Player" module header
*/

#ifndef PLAYER_H
#define PLAYER_H
#include "map.h"
#include "tools.h"
#include <vector>

class Player {
  public:
    Player(double xPosition, double yPosition, int life, double counter, double radius,
           int lPosition, int cPosition, double velocity, int ID);
    ~Player(){};

    bool touchedAndDead();

    double getX() const;
    double getY() const;
    int getL() const;
    int getC() const;
    double getGX() const;
    double getGY() const;
    int getID() const;
    double getRadius() const;
    double getCount() const;
    double getLife() const;
    Circle *getHitbox() const;
    double getVelocity() const;

    void setL(int l);
    void setC(int c);
    void setGX(double gY);
    void setGY(double gY);
    void setCount(int c);
    void setLife(int t);

  private:
    int life;
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
    void addPlayer(double xPosition, double yPosition, int life, double counter,
                   double radius, double velocity, int ID);
    void removePlayer(int ID);
    void removeAll();
    void reserveSpace(int nbPlayer);
    bool isPlayer(int lgnPos, int colPos);
    bool isDifferentPlayer(int lgnPos, int colPos, int ID);
    std::vector<int> whichPlayer(int lgnPos, int colPos);
    Player *getPlayer(int p) const;
    int getNb() const;
    int getLNb() const;
    int getCNb() const;
    void updatePosition();

  private:
    bool isCollision(int newX, int newY, int ID, double delta);
    std::vector<Player *> players;
    std::vector<int> **playerGrid;
};

#endif
