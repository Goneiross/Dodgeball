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

#ifndef OBJECT_HEADER
#define OBJECT_HEADER
#include "player.h"
#include "ball.h"
#include "map.h"
#endif

#include "error.h"
#include "GUI.h"

using namespace std;

static void initialization(string inputFile, int &nbCell, int &nbPlayer,
                    PlayerMap* &players, int &nbObstacle, Map *&mainMap,
                    int &nbBall, BallMap* &balls) ;
static void largeCollisionCheck(PlayerMap* players, Map* map, int p, 
                        vector<int> &toCheck);
static void largeCollisionCheck(BallMap* balls, Map* map, int b, 
                                vector<int> &toCheck);
static void largeCollisionCheck(BallMap* balls, int b, vector<int> &toCheck, int lMax);
static void largeCollisionCheck(PlayerMap* players, int b, 
                                vector<int> &toCheck, int lMax);
static void collisionCheck(PlayerMap* players, int p, int pmax, double delta);
static void collisionCheck(BallMap* balls, int b, int bmax, double delta);
static void collisionCheck(PlayerMap* players, BallMap* balls, int p,
                            int b, double delta);
static void collisionCheck(PlayerMap* players, Map *map, int p, int o, 
                            double delta);
static void collisionCheck(BallMap* balls, Map *map, int b, int o, 
                            double delta);
static void parseData(Map *&mainMap, PlayerMap* &players, BallMap* &balls, int &nbCell,
                      double &ingameMargin, double &parsingMargin, string inputData0);
static void parseData(PlayerMap* &players, int p, int nbCell, double parsingMargin, 
                      double playerRadius, double playerVelocity, string inputData0, 
                      string inputData1, string inputData2, string inputData3);
static void parseData(Map *&mainMap, int nbCell, int o, string inputData0, 
                            string inputData1);
static void parseData(BallMap* &balls, PlayerMap* &players, Map *&mainMap, int nbCell,
               int nbPlayer, int nbObstacle, double parsingMargin, int b,
               double ballRadius, double ballVelocity, string inputData0,
               string inputData1, string inputData2);
void simulation(std::string inputFile, int mode) {
  int nbCell = 0, nbPlayer = 0, nbObstacle = 0, nbBall = 0;
  PlayerMap* players;
  BallMap* balls;
  Map* mainMap;

  if (inputFile != ""){ // TO HANDLE
    initialization(inputFile, nbCell, nbPlayer, players, nbObstacle, mainMap,
                 nbBall, balls);
  }
  
  if (mode == 1) {
    cout << FILE_READING_SUCCESS << endl;
    delete mainMap;
    delete players;
    delete balls;
    return;
  } else {
    draw(players, balls, mainMap);
    delete mainMap;
    delete players;
    delete balls;
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
  double ballRadius;
  double ballVelocity;
  double playerRadius;
  double playerVelocity;
  while (flux >> inputData[0]) {
    if (inputData[0] == "#") {
      do {
        flux.get(charBin);
      } while (charBin != '\n');
    } else if (parseType == 0) {
      parseData(mainMap, players, balls, nbCell, 
                ingameMargin, parsingMargin, inputData[0]);
      ballRadius = COEF_RAYON_BALLE * (SIDE / nbCell);
      ballVelocity = COEF_VITESSE_BALLE * (SIDE / nbCell);
      playerRadius = COEF_RAYON_JOUEUR * (SIDE / nbCell);
      playerVelocity = COEF_VITESSE_JOUEUR * (SIDE / nbCell);
      parseType++;
    } else if (parseType == 1) {
      nbPlayer = stoi(inputData[0]);
      players->reserveSpace(nbPlayer);
      parseType++;
    } else if (parseType == 2) {
      flux >> inputData[1] >> inputData[2] >> inputData[3];
      parseData(players, p, nbCell, parsingMargin, playerRadius, playerVelocity, inputData[0], 
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
      parseData(balls, players, mainMap,nbCell, nbPlayer, nbObstacle, parsingMargin, b, 
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

void save(string filename, int nbCell, PlayerMap* players, Map* mainMap, BallMap* balls){
  ofstream flux(filename, std::ofstream::out);
  int n = 0;
  flux << nbCell << endl << endl;
  n = players->getNb();
  flux << n << endl;
  for (int i = 0; i < n; i ++){
    Player* p = players->getPlayer(i);
    flux << p->getX() << " " << p->getY() << " " 
         << p->getTimeTouched() << " " << p->getCount() << endl;
    p = nullptr;
  }
  n = mainMap->getNb();
  flux << endl << n << endl;
  for (int i = 0; i < n; i ++){
    Obstacle* o = mainMap->getObstacle(i);
    flux << o->getX() << " " << o->getY() << endl;
    o = nullptr;
  }
  n = balls->getNb();
  flux << endl << n << endl;
  for (int i = 0; i < n; i ++){
    Ball* b = balls->getBall(i);
    flux << b->getX() << " " << b->getY() << " " 
         << b->getAngle() << endl;
    b = nullptr; 
  }
  flux.close();
}

static void largeCollisionCheck(PlayerMap* players, Map* map, int p, 
                        vector<int> &toCheck){
  int cPosition = ((players->getPlayer(p)->getX() + DIM_MAX ) /
                    map->getObstacle(0)->getHitbox()->getSide() )
                    - 1 / 2;
  int lPosition = - ((players->getPlayer(p)->getY() - DIM_MAX) /
                    map->getObstacle(0)->getHitbox()->getSide() )
                    - 1 / 2;
  int leftL = -1, leftC = -1, rightL = 1, rightC = 1;
  if (lPosition == 0){leftL = 0;}
  if (cPosition == 0){leftC = 0;}
  if (lPosition == map->getCNb() - 1){rightL = 0;}
  if (cPosition == map->getLNb() - 1){rightC = 0;}
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
                    map->getObstacle(0)->getHitbox()->getSide() )
                    - 1 / 2;
  int lPosition = - ((balls->getBall(b)->getY() - DIM_MAX) /
                    map->getObstacle(0)->getHitbox()->getSide() )
                    - 1 / 2;
  int leftL = -1, leftC = -1, rightL = 1, rightC = 1;
  if (lPosition == 0){leftL = 0;}
  if (cPosition == 0){leftC = 0;}
  if (lPosition == map->getCNb() - 1){rightL = 0;}
  if (cPosition == map->getLNb() - 1){rightC = 0;}
  for (int i = leftL; i <= rightL; i++){
    for (int j = leftC; j <= rightC; j++){
      if(map->isObstacle(lPosition + i, cPosition + j)){
        toCheck.push_back(map->whichObstacle(lPosition + i, cPosition + j));
      }
    }
  }
}

static void largeCollisionCheck(BallMap* balls, int b, vector<int> &toCheck, int lMax){
  int cPosition = balls->getBall(b)->getC();
  int lPosition = balls->getBall(b)->getL();
  int leftL = -1, leftC = -1, rightL = 1, rightC = 1;
  if (lPosition == 0){leftL = 0;}
  if (cPosition == 0){leftC = 0;}
  if (lPosition == lMax - 1){rightL = 0;}
  if (cPosition == lMax - 1){rightC = 0;} //cMax
  for (int i = leftL; i <= rightL; i++){
    for (int j = leftC; j <= rightC; j++){
      if(balls->isBall(lPosition + i, cPosition + j)){
        int n = balls->whichBall(lPosition + i, cPosition + j).size();
        int tmp = -1;
        for (int k = 0; k < n; k++) {
          tmp = balls->whichBall(lPosition + i, cPosition + j)[k];
          if((tmp > -1) && (tmp != b)){
            toCheck.push_back(tmp);
          }
        }
      }
    }
  }
}

static void largeCollisionCheck(PlayerMap* players, int p, 
                                vector<int> &toCheck, int lMax){
  int cPosition = players->getPlayer(p)->getC();
  int lPosition = players->getPlayer(p)->getL();
  int leftL = -1, leftC = -1, rightL = 1, rightC = 1;
  if (lPosition == 0){leftL = 0;}
  if (cPosition == 0){leftC = 0;}
  if (lPosition == lMax - 1){rightL = 0;}
  if (cPosition == lMax - 1){rightC = 0;} //cMax
  for (int i = leftL; i <= rightL; i++){
    for (int j = leftC; j <= rightC; j++){
      if(players->isPlayer(lPosition + i, cPosition + j)){
        int n = players->whichPlayer(lPosition + i, cPosition + j).size();
        int tmp = -1;
        for (int k = 0; k < n; k++) {
          tmp = players->whichPlayer(lPosition + i, cPosition + j)[k];
          if((tmp > -1) && (tmp != p)){
            toCheck.push_back(tmp);
          }
        }
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

static void collisionCheck(BallMap* balls, int b, int c, double delta) {
  double d = distance(balls->getBall(c)->getHitbox(), balls->getBall(b)->getHitbox());
  if (d < (balls->getBall(c)->getRadius() + balls->getBall(b)->getRadius() + delta)) {
    cout << BALL_COLLISION(c + 1, b + 1) << endl;
    exit(1);
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
  double d = distance(map->getObstacle(o)->getHitbox(), players->getPlayer(p)->getHitbox());
  double X = map->getObstacle(o)->getX() - players->getPlayer(p)->getX();
  double Y = map->getObstacle(o)->getY() - players->getPlayer(p)->getY();
  double angle;
  if (X == 0) {
    angle = M_PI_2;
  } else if (Y == 0) {
    angle = 0;
  } else {
    angle = atan(Y / X);
  }
  double rayon = map->getObstacle(o)->getHitbox()->getSide() / 2;
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
  double d = distance(map->getObstacle(o)->getHitbox(), balls->getBall(b)->getHitbox());
  double X = map->getObstacle(o)->getX() - balls->getBall(b)->getX();
  double Y = map->getObstacle(o)->getY() - balls->getBall(b)->getY();
  double angle;
  if (X == 0) {
    angle = M_PI_2;
  } else if (Y == 0) {
    angle = 0;
  } else {
    angle = atan(Y / X);
  }
  double rayon = map->getObstacle(o)->getHitbox()->getSide() / 2;
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

static void parseData(Map* &mainMap, PlayerMap* &players, BallMap* &balls, int &nbCell,
                      double &ingameMargin, double &parsingMargin, string inputData0) {
  nbCell = stoi(inputData0);
  mainMap = new Map(nbCell, nbCell);
  players =new PlayerMap(nbCell, nbCell);
  balls = new BallMap(nbCell, nbCell);
  ingameMargin = COEF_MARGE_JEU * (SIDE / nbCell);
  parsingMargin = ingameMargin / 2;
}

static void parseData(PlayerMap* &players, int p, int nbCell, double parsingMargin, 
                      double playerRadius, double playerVelocity, string inputData0,
                      string inputData1, string inputData2, string inputData3) {
  if (((abs(stod(inputData0)) > DIM_MAX) ||
       (abs(stod(inputData1)  > DIM_MAX)))) {
    cout << PLAYER_OUT(p + 1) << endl;
    exit(1);
  } else {
    players->addPlayer(stod(inputData0), stod(inputData1), stoi(inputData2), 
                    stod(inputData3), playerRadius, playerVelocity, p);

    vector<int> toCheck;
    largeCollisionCheck(players, p, toCheck, nbCell);
    for (auto c : toCheck){
      collisionCheck(players, p, c, parsingMargin);
    }
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

static void parseData(BallMap* &balls, PlayerMap* &players, Map *&mainMap, int nbCell,
                      int nbPlayer, int nbObstacle, double parsingMargin, int b, 
                      double ballRadius, double ballVelocity, string inputData0, 
                      string inputData1, string inputData2) {
  if ((abs(stoi(inputData0)) > DIM_MAX) || (abs(stoi(inputData1)) > DIM_MAX)) {
    cout << BALL_OUT(b + 1) << endl;
    exit(1);
  } else {
    balls->addBall(stod(inputData0), stod(inputData1), stod(inputData2), 
                              ballRadius, ballVelocity, b);
    vector<int> toCheck;
    largeCollisionCheck(balls, b, toCheck, nbCell);
    for (auto c : toCheck){
      collisionCheck(balls, b, c, parsingMargin);
    }
    for (int i = 0; i < nbPlayer; i++) {
      collisionCheck(players, balls, i, b, parsingMargin);
    } 
  }
}