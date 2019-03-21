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
  Player(double x0, double y0, int nbCell);
  Player(double x0, double y0, int t, double c, int nbCell);
  Player(double x0, double y0, int t, double c, double r, double v); 
  ~Player(){};
  double getX() const;
  double getY() const;
  double getRadius() const;
  double getCount() const;
  double getTimeTouched() const;
  Circle *getHitbox() const;
  void updatePosition(double angle);
  bool touchedAndDead();

private:
  int timeTouched;
  double count;
  double velocity;
  Circle *hitbox;
};

class PlayerMap {
  public:
    PlayerMap(int l, int c);
    void addPlayer(double x, double y, int t, double c, double r, double v, int ID);
    void reserveSpace(int nbPlayer);
    Player* getPlayer(int p) const;
  private:
    int lineNumber;
    int columnNumber;
    std::vector<Player*> players;
    std::vector<int>** playerGrid;
};