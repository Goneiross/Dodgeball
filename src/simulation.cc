/*!
  \file   ball.cc
  \author Guillaume Pellerin & Vincent Miche
  \date   March 2019
  \brief  Ball module implementation
*/

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>

#include "simulation.h"
#include "ball.h"
#include "map.h"
#include "player.h"
#include "error.h"

using namespace std;

static void initialization(string inputFile, int &nbCell, int &nbPlayer,
                    PlayerMap* &players, int &nbObstacle, Map *&mainMap,
                    int &nbBall, BallMap* &balls) ;
static void largeCollisionCheck(PlayerMap* players, Map* map, int p, 
                        vector<int> &toCheck);
static void largeCollisionCheck(BallMap* balls, Map* map, int b, 
                                vector<int> &toCheck);
static void collisionCheck(PlayerMap* players, int p, int pmax, double delta);
static void collisionCheck(BallMap* balls, int b, int bmax, double delta);
static void collisionCheck(PlayerMap* players, BallMap* balls, int p,
                            int b, double delta);
static void collisionCheck(PlayerMap* players, Map *map, int p, int o, 
                            double delta);
static void collisionCheck(BallMap* balls, Map *map, int b, int o, 
                            double delta);
static void parseData(Map *&mainMap, int &nbCell, double &ingameMargin, 
                      double &parsingMargin, string inputData0);
static void parseData(PlayerMap* &players, int p, double parsingMargin, 
                      double playerRadius, double playerVelocity, string inputData0, 
                      string inputData1, string inputData2, string inputData3);
static void parseData(Map *&mainMap, int nbCell, int o, string inputData0, 
                            string inputData1);
static void parseData(BallMap* &balls, PlayerMap* &players, Map *&mainMap,
               int nbPlayer, int nbObstacle, double parsingMargin, int b,
               double ballRadius, double ballVelocity, string inputData0,
               string inputData1, string inputData2);
void simulation(std::string inputFile, int mode) {
  int nbCell = 0, nbPlayer = 0, nbObstacle = 0, nbBall = 0;
  PlayerMap* players;
  BallMap* balls;
  Map *mainMap;

  initialization(inputFile, nbCell, nbPlayer, players, nbObstacle, mainMap,
                 nbBall, balls);
  if (mode == 1) {
    cout << FILE_READING_SUCCESS << endl;
    delete mainMap;
    for (int i = 0; i < nbPlayer; i++) {
      delete players;
    }
    for (int i = 0; i < nbBall; i++) {
      delete balls;
    }
    return;
  } else {
    delete mainMap;
    for (int i = 0; i < nbPlayer; i++) {
      delete players;
    }
    for (int i = 0; i < nbBall; i++) {
      delete balls;
    }
    return;
  }
}

static void initialization(string inputFile, int &nbCell, int &nbPlayer,
                    PlayerMap* &players, int &nbObstacle, Map *&mainMap,
                    int &nbBall, BallMap* &balls) {
  string inputData[4];
  char charBin;
  int parseType = 0;
  int p = 0, o = 0, b = 0;  // ID of the currently selected object/player/ball
  double ingameMargin, parsingMargin;

  ifstream flux(inputFile, ios::in);
  if (!flux) {
    cout << "Unable to open file " << inputFile << endl;
    exit(0);
  }
  while (flux >> inputData[0]) {
    if (inputData[0] == "#") {
      do {
        flux.get(charBin);
      } while (charBin != '\n');
    } else if (parseType == 0) {
      parseData(mainMap, nbCell, ingameMargin, parsingMargin, inputData[0]);
      parseType++;
    } else if (parseType == 1) {
      nbPlayer = stoi(inputData[0]);
      players->reserveSpace(nbPlayer);
      parseType++;
    } else if (parseType == 2) {
      flux >> inputData[1] >> inputData[2] >> inputData[3];
      double playerRadius = COEF_RAYON_JOUEUR * (SIDE / nbCell);
      double playerVelocity = COEF_VITESSE_JOUEUR * (SIDE / nbCell);
      parseData(players, p, parsingMargin, playerRadius, playerVelocity, inputData[0], 
                inputData[1], inputData[2], inputData[3]);
      p++;
      if (p == nbPlayer) {
        parseType++;
      }
    } else if (parseType == 3) {
      nbObstacle = stoi(inputData[0]);
      parseType++;
    } else if (parseType == 4) {
      flux >> inputData[1];
      parseData(mainMap, nbCell, o, inputData[0], inputData[1]);

      o++;
      if (o == nbObstacle) {
        for (int i = 0; i < nbPlayer; i++) {
          vector<int> toCheck;
          largeCollisionCheck(players, mainMap, i, toCheck);
          int nbToCheck = toCheck.size();
          for (int j = 0; j < nbToCheck; j++){
            collisionCheck(players, mainMap, i, toCheck[j], parsingMargin);
          }
        }
        parseType++;
      }
    } else if (parseType == 5) {
      nbBall = stoi(inputData[0]);
      balls->reserveSpace(nbBall + nbPlayer);
      parseType++;
    } else if (parseType == 6) {
      flux >> inputData[1] >> inputData[2];
      double ballRadius = COEF_RAYON_BALLE * (SIDE / nbCell);
      double ballVelocity = COEF_VITESSE_BALLE * (SIDE / nbCell);
      parseData(balls, players, mainMap, nbPlayer, nbObstacle, parsingMargin, b, 
                ballRadius, ballVelocity, inputData[0], inputData[1], inputData[2]);
      b++;
      if (b == nbBall) {
        for (int i = 0; i < nbBall; i++) {
          vector<int> toCheck;
          largeCollisionCheck(balls, mainMap, i, toCheck);
          for (auto o : toCheck){
            collisionCheck(balls, mainMap, i, o, parsingMargin);
          }
        }
        parseType++;
      }
    } else {
      flux.get(charBin);
    }
  }
  flux.close();
}

static void largeCollisionCheck(PlayerMap* players, Map* map, int p, 
                        vector<int> &toCheck){
  int cPosition = ((players->getPlayer(p)->getX() + DIM_MAX ) /
                    map->getObstacle()[0]->getHitbox()->getSide() )
                    - 1 / 2;
  int lPosition = - ((players->getPlayer(p)->getY() - DIM_MAX) /
                    map->getObstacle()[0]->getHitbox()->getSide() )
                    - 1 / 2;
  int leftL = -1, leftC = -1, rightL = 1, rightC = 1;
  if (lPosition == 0){leftL = 0;}
  if (cPosition == 0){leftC = 0;}
  if (lPosition == map->getX() - 1){rightL = 0;}
  if (cPosition == map->getY() - 1){rightC = 0;}
  for (int i = leftL; i <= rightL; i++){
    for (int j = leftC; j <= rightC; j++){
      if(map->isObstacle(lPosition + i, cPosition + j)){
        toCheck.push_back(map->whichObstacle(lPosition + i, cPosition + j));
      }
    }
  }
}

static void largeCollisionCheck(BallMap* balls, Map* map, int b, 
                                vector<int> &toCheck){
  int cPosition = ((balls->getBall(b)->getX() + DIM_MAX ) /
                    map->getObstacle()[0]->getHitbox()->getSide() )
                    - 1 / 2;
  int lPosition = - ((balls->getBall(b)->getY() - DIM_MAX) /
                    map->getObstacle()[0]->getHitbox()->getSide() )
                    - 1 / 2;
  int leftL = -1, leftC = -1, rightL = 1, rightC = 1;
  if (lPosition == 0){leftL = 0;}
  if (cPosition == 0){leftC = 0;}
  if (lPosition == map->getX() - 1){rightL = 0;}
  if (cPosition == map->getY() - 1){rightC = 0;}
  for (int i = leftL; i <= rightL; i++){
    for (int j = leftC; j <= rightC; j++){
      if(map->isObstacle(lPosition + i, cPosition + j)){
        toCheck.push_back(map->whichObstacle(lPosition + i, cPosition + j));
      }
    }
  }
}

static void collisionCheck(PlayerMap* players, int p, int pmax, double delta) {
  for (int i = 0; i <= pmax; i++) {
    if (i != p) {
      double d = distance(players->getPlayer(i)->getHitbox(), players->getPlayer(p)->getHitbox());
      if (d < (players->getPlayer(i)->getRadius() + players->getPlayer(p)->getRadius() + delta)) {
        cout << PLAYER_COLLISION(i + 1, p + 1) << endl;
        exit(1);
      }
    }
  }
}

static void collisionCheck(BallMap* balls, int b, int bmax, double delta) {
  for (int i = 0; i <= bmax; i++) {
    if (i != b) {
      double d = distance(balls->getBall(i)->getHitbox(), balls->getBall(b)->getHitbox());
      if (d < (balls->getBall(i)->getRadius() + balls->getBall(b)->getRadius() + delta)) {
        cout << BALL_COLLISION(i + 1, b + 1) << endl;
        exit(1);
      }
    }
  }
}

static void collisionCheck(PlayerMap* players, BallMap* balls, int p,
                     int b, double delta) {
  double d = distance(players->getPlayer(p)->getHitbox(), balls->getBall(b)->getHitbox());
  if (d < (players->getPlayer(p)->getRadius() + balls->getBall(b)->getRadius() + delta)) {
    cout << PLAYER_BALL_COLLISION(p + 1, b + 1) << endl;
    exit(1);
  }
}

static void collisionCheck(PlayerMap* players, Map *map, int p, int o, 
                            double delta) {
  double d = distance(map->getObstacle()[o]->getHitbox(), players->getPlayer(p)->getHitbox());
  double X = map->getObstacle()[o]->getX() - players->getPlayer(p)->getX();
  double Y = map->getObstacle()[o]->getY() - players->getPlayer(p)->getY();
  double angle;
  if (X == 0) {
    angle = M_PI_2;
  } else if (Y == 0) {
    angle = 0;
  } else {
    angle = atan(Y / X);
  }
  double rayon = map->getObstacle()[o]->getHitbox()->getSide() / 2;
  double squareRadius;
  if ((abs(angle) == M_PI) || (abs(angle) == M_PI / 2) || (angle == 0)) {
    squareRadius = rayon;
  } else if ((abs(angle) == M_PI / 4) || (abs(angle) == 2 * M_PI / 4)) {
    squareRadius = sqrt(2) * rayon;
  } else if ((2 * M_PI / 4 < angle) && (angle < M_PI)) {
    squareRadius = (rayon / X) * d;
  } else if ((-M_PI / 4 < angle) && (angle < M_PI / 4)) {
    squareRadius = (rayon / X) * d;
  } else if ((-2 * M_PI / 4 > angle) && (angle > -M_PI)) {
    squareRadius = (rayon / X) * d;
  } else {
    squareRadius = (rayon / Y) * d;
  }
  if (d < (players->getPlayer(p)->getRadius() + abs(squareRadius) + delta)) {
    cout << COLL_OBST_PLAYER(o + 1, p + 1) << endl;
    exit(1);
  }
}

static void collisionCheck(BallMap* balls, Map *map, int b, int o, 
                            double delta) {
  double d = distance(map->getObstacle()[o]->getHitbox(), balls->getBall(b)->getHitbox());
  double X = map->getObstacle()[o]->getX() - balls->getBall(b)->getX();
  double Y = map->getObstacle()[o]->getY() - balls->getBall(b)->getY();
  double angle;
  if (X == 0) {
    angle = M_PI_2;
  } else if (Y == 0) {
    angle = 0;
  } else {
    angle = atan(Y / X);
  }
  double rayon = map->getObstacle()[o]->getHitbox()->getSide() / 2;
  double squareRadius;
  if ((abs(angle) == M_PI) || (abs(angle) == M_PI / 2) || (angle == 0)) {
    squareRadius = rayon;
  } else if ((abs(angle) == M_PI / 4) || (abs(angle) == 2 * M_PI / 4)) {
    squareRadius = sqrt(2) * rayon;
  } else if ((2 * M_PI / 4 < angle) && (angle < M_PI)) {
    squareRadius = (rayon / X) * d;
  } else if ((-M_PI / 4 < angle) && (angle < M_PI / 4)) {
    squareRadius = (rayon / X) * d;
  } else if ((-2 * M_PI / 4 > angle) && (angle > -M_PI)) {
    squareRadius = (rayon / X) * d;
  } else {
    squareRadius = (rayon / Y) * d;
  }
  if (d < (balls->getBall(b)->getRadius() + abs(squareRadius) + delta)) {
    cout << COLL_BALL_OBSTACLE(b + 1) << endl;
    exit(1);
  }
}

static void parseData(Map *&mainMap, int &nbCell, double &ingameMargin, 
                      double &parsingMargin, string inputData0) {
  nbCell = stoi(inputData0);
  mainMap = new Map(nbCell, nbCell);
  ingameMargin = COEF_MARGE_JEU * (SIDE / nbCell);
  parsingMargin = ingameMargin / 2;
}

static void parseData(PlayerMap* &players, int p, double parsingMargin, 
                      double playerRadius, double playerVelocity, string inputData0,
                      string inputData1, string inputData2, string inputData3) {
  if (((abs(stod(inputData0)) > DIM_MAX) ||
       (abs(stod(inputData1)  > DIM_MAX)))) {
    cout << PLAYER_OUT(p + 1) << endl;
    exit(1);
  } else {
    players->addPlayer(stod(inputData0), stod(inputData1), stoi(inputData2), 
                    stod(inputData3), playerRadius, playerVelocity, p);
    collisionCheck(players, p, p, parsingMargin);
  }
}

static void parseData(Map *&mainMap, int nbCell, int o, string inputData0, 
                      string inputData1) {
  if (stoi(inputData0) >= nbCell) {
    cout << OBSTACLE_VALUE_INCORRECT(stoi(inputData0)) << endl;
    exit(1);
  } else if (stoi(inputData1) >= nbCell) {
    cout << OBSTACLE_VALUE_INCORRECT(stoi(inputData1)) << endl;
    exit(1);
  } else if (stoi(inputData0) < 0) {
    cout << OBSTACLE_VALUE_INCORRECT(stoi(inputData0)) << endl;
    exit(1);
  } else if (stoi(inputData1) < 0) {
    cout << OBSTACLE_VALUE_INCORRECT(stoi(inputData1)) << endl;
    exit(1);
  } else if (mainMap->isObstacle(stoi(inputData0), stoi(inputData1))) {
    cout << MULTI_OBSTACLE(stoi(inputData0), stoi(inputData1)) << endl;
    exit(1);
  } else {
    mainMap->addObstacle(stod(inputData0), stod(inputData1), o);
  }
}

static void parseData(BallMap* &balls, PlayerMap* &players, Map *&mainMap,
                      int nbPlayer, int nbObstacle, double parsingMargin, int b, 
                      double ballRadius, double ballVelocity, string inputData0, 
                      string inputData1, string inputData2) {
  if ((abs(stoi(inputData0)) > DIM_MAX) || (abs(stoi(inputData1)) > DIM_MAX)) {
    cout << BALL_OUT(b + 1) << endl;
    exit(1);
  } else {
    balls.push_back(new Ball(stod(inputData0), stod(inputData1), stod(inputData2), 
                              ballRadius, ballVelocity));
    collisionCheck(balls, b, b, parsingMargin);
    for (int i = 0; i < nbPlayer; i++) {
      collisionCheck(players, balls, i, b, parsingMargin);
    }
    for (int o = 0; o < nbObstacle; o++) {
      collisionCheck(balls, mainMap, b, o, parsingMargin);
    }
  }
}