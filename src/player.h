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
  Player(double x0, double y0, int timeTouched, 
        double counter, double radius, int l,int col,double v, int i); 
  ~Player(){};
  double getX() const;
  double getY() const;
  double getL() const;
  double getC() const;
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
  int lPosition;
  int cPosition;
};

class PlayerMap {
  public:
    PlayerMap(int l, int c);
    ~PlayerMap(){};
    void addPlayer(double x, double y, int t, double c, double r, double v, int ID);
    void reserveSpace(int nbPlayer);
    bool isPlayer(int lPosition, int cPosition);
    std::vector<int> whichPlayer(int lPosition, int cPosition);
    Player* getPlayer(int p) const;
  private:
    int lineNumber;
    int columnNumber;
    std::vector<Player *> players;
    std::vector<int>** playerGrid;
};