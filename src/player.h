/*!
  \file   player.h
  \author Guillaume Pellerin & Vincent Miche
  \date   March 2019
  \brief  "Player" module header
*/

#include <vector>
#ifndef TOOLS_H
#define TOOLS_H
#include "tools.h"
#endif

class Player {
public:
  Player(double xPosition, double yPosition, int timeTouched, double counter,
         double radius, int lPosition, int cPosition, double velocity, int ID); 
  ~Player(){};
  double getX() const;
  double getY() const;
  double getL() const;
  double getC() const;
  double getGX() const;
  double getGY() const;
  double getRadius() const;
  double getCount() const;
  double getTimeTouched() const;
  Circle *getHitbox() const;
  void updatePosition(double angle);
  bool touchedAndDead();

private:
  int timeTouched;
  int ID;
  double count;
  double velocity;
  Circle *hitbox;
  int lPosition, cPosition;
  double gXPosition, gYPosition;
};

class PlayerMap {
  public:
    PlayerMap(int lPosition, int cPosition);
    ~PlayerMap(){};
    void addPlayer(double xPosition, double yPosition, int timeTouched, double counter,
                   double radius, double velocity, int ID);
    void removePlayer(int ID);
    void removeAll();
    void reserveSpace(int nbPlayer);
    bool isPlayer(int lPosition, int cPosition);
    std::vector<int> whichPlayer(int lPosition, int cPosition);
    Player* getPlayer(int p) const;
    int getNb() const;
  private:
    int lineNumber, columnNumber;
    std::vector<Player *> players;
    std::vector<int>** playerGrid;
};