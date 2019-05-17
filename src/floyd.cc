/*!
  \file   floyd.cc
  \author Guillaume Pellerin & Vincent Miche
  \date   May 2019
  \brief  pathfinding, with the Roy-Floyd-Warschall algorithm
*/

#include "define.h"
#include "obstacle.h"
#include "player.h"
#include "tools.h"
#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

static vector<vector<double>> tabCellDist;
static vector<vector<double>> pathAngles;

bool isThereObstacleBetween(int l1, int c1, int l2, int c2, ObstacleMap *obstacles);
double simplePath(Player *start, Player *target);
void complexPath(Player *start, Player *target, int infinityInit, int infinityDist,
                 bool &firstInStep);
void floyd(Player *start, Player *target, int infinityInit, int infinityDist);
void diagonalDistance(unsigned int i, unsigned int j,
                      vector<vector<double>> &tabCellDist,
                      vector<vector<double>> &pathAngles, ObstacleMap *obstacles);
bool areThereUninitialisedCases(vector<vector<double>> tab2D, int infinityInit);
void shortestIndirectPath(int startID, int targetID,
                          vector<vector<double>> &tabCellDist,
                          vector<vector<double>> &pathAngles, int infinityDist,
                          int nbCell);

vector<int> targetting(PlayerMap *players, int infinityInit, int infinityDist) {
  // cout << "infinityInit: " << infinityInit << endl;
  // dBP = distance between players
  vector<vector<double>> dBP(players->getNb(),
                             vector<double>(players->getNb(), infinityInit));
  for (int i = 0; i < players->getNb(); i++) {
    for (int j = 0; j < i; j++) {
      dBP[i][j] =
          distance(players->getPlayer(i)->getL(), players->getPlayer(i)->getC(),
                   players->getPlayer(j)->getL(), players->getPlayer(j)->getC());
      dBP[j][i] = dBP[i][j];
      cout << "start: " << i << "(" << players->getPlayer(i)->getL() << ","
           << players->getPlayer(i)->getC() << ") target: " << j << "("
           << players->getPlayer(j)->getL() << "," << players->getPlayer(j)->getC()
           << ") distance:"
           << distance(players->getPlayer(i)->getL(), players->getPlayer(i)->getC(),
                       players->getPlayer(j)->getL(), players->getPlayer(j)->getC())
           << endl;
    }
  }
  vector<double> minDistance(players->getNb());
  vector<int> target(players->getNb());
  for (int i = 0; i < players->getNb(); i++) {
    minDistance[i] = infinityDist;

    for (int j = 0; j < players->getNb(); j++) {
      if (i != j) {
        if (dBP[i][j] < minDistance[i]) {
          minDistance[i] = dBP[i][j];
          target[i] = j;
        }
      }
    }
  }
  return target; // rend donc l'indice correspondant à players
}

bool areThereUninitialisedCases(vector<vector<double>> tab2D, int infinityInit) {
  // considers an inferior triangular matrix, with a null-diagonal
  for (int i(0); i < tab2D.size(); i++) {
    for (int j(0); j < i; j++) {
      if (tab2D[i][j] == infinityInit) {
        return true;
      }
    }
  }
  return false;
}

bool isThereObstacleBetween(int l1, int c1, int l2, int c2, ObstacleMap *obstacles) {
  if (l1 == l2) { // Case 1 : same line
    if (c1 < c2) {
      for (int c = c1; c <= c2; c++) {
        if (obstacles->isObstacle(l2, c)) {
          return true;
        }
      }
      return false;
    } else {
      for (int c = c2; c <= c1; c++) {
        if (obstacles->isObstacle(l2, c)) {
          return true;
        }
      }
      return false;
    }
  } else if (c1 == c2) { // Case 2 : same column
    if (l1 < l2) {
      for (int l = l1; l <= l2; l++) {
        if (obstacles->isObstacle(l, c2)) {
          return true;
        }
      }
      return false;
    } else {
      for (int l = l2; l <= l1; l++) {
        if (obstacles->isObstacle(l, c2)) {
          return true;
        }
      }
      return false;
    }
  } else { // Case 3 : different line and column
    enum direction { line = 0, column = 1 };
    bool direction = line;
    int l = l1, c = c1;
    while ((l != l2) && (c != c2)) {
      if (direction == line) {
        if (l < l2) {
          l++;
        } else if (l > l2) {
          l--;
        } else {
          direction = column;
          continue;
        }
        if (obstacles->isObstacle(l, c)) {
          return true;
        }
        direction = column;
      } else {
        if (c < c2) {
          l++;
        } else if (c > c2) {
          l--;
        } else {
          direction = line;
          continue;
        }
        if (obstacles->isObstacle(l, c)) {
          return true;
        }
        direction = line;
      }
    }
  }
}

double simplePath(Player *start, Player *target) {
  return angle(start->getX(), start->getY(), target->getX(), target->getY());
}

void floyd(Player *start, Player *target, int infinityInit, int infinityDist,
           ObstacleMap *obstacles) {
  int nbCell = obstacles->getLNb();
  int nbCellSquared = pow(nbCell, 2);
  // initialisation d'un tableau de distances
  tabCellDist.resize(nbCellSquared, vector<double>(nbCellSquared, infinityInit));
  pathAngles.resize(nbCellSquared, vector<double>(nbCellSquared, 0));
  for (int i(0); i < nbCellSquared; i++) {
    for (int j(0); j < nbCellSquared; j++) {
      tabCellDist[i][j] = infinityInit;
    }
    for (int j(0); j < i; j++) {
      if ((i / nbCell) == (j / nbCell)) {
        if ((i % nbCell) == ((j % nbCell) - 1)) {
          tabCellDist[i][j] = 1;
          tabCellDist[j][i] = 1;
          pathAngles[i][j] = 0;
          pathAngles[j][i] = M_PI;
        } else if ((i % nbCell) == ((j % nbCell) + 1)) {
          tabCellDist[i][j] = 1;
          tabCellDist[j][i] = 1;
          pathAngles[i][j] = M_PI;
          pathAngles[j][i] = 0;
        }
      } else if ((i % nbCell) == (j % nbCell)) {
        if ((i / nbCell) == ((j / nbCell) - 1)) {
          tabCellDist[i][j] = 1;
          tabCellDist[j][i] = 1;
          pathAngles[i][j] = 3 * M_PI_2;
          pathAngles[j][i] = M_PI_2;
        } else if ((i / nbCell) == ((j / nbCell) + 1)) {
          tabCellDist[i][j] = 1;
          tabCellDist[j][i] = 1;
          pathAngles[i][j] = M_PI_2;
          pathAngles[j][i] = 3 * M_PI_2;
        }
      }
    }
  }
  for (int i(0); i < nbCellSquared; i++) {
    if (obstacles->isObstacle(i % nbCell, i / nbCell)) {
      // cout << i%nbCell << i/nbCell << endl;
      for (int j(0); j < nbCellSquared; j++) {
        // cout << i << j << " OBST" << endl;
        tabCellDist[i][j] = infinityDist;
        tabCellDist[j][i] = infinityDist;
      }
    }
    tabCellDist[i][i] = 0;
  }
  // Diag not working
  for (int i(0); i < nbCellSquared - nbCell; i++) {
    if (i % nbCell != nbCell - 1) {
      diagonalDistance(i, i + nbCell + 1, tabCellDist, pathAngles, obstacles);
    }
    if (i % nbCell != 0) {
      diagonalDistance(i, i + nbCell - 1, tabCellDist, pathAngles, obstacles);
    }
  }

  // cout << "done" << endl;
  int g = 0;
  while (areThereUninitialisedCases(tabCellDist, infinityInit) && g < 20) {
    g++;
    /*
    cout << "infinityInit: " << infinityInit << endl;
    for (int i = 0; i < tabCellDist.size(); i++) {
      for (int j = 0; j < tabCellDist[0].size(); j++) {
        cout << "(" << i << "," << j << "," << tabCellDist[i][j] << ") ";
      }
    }
    cout << endl << endl;
    */
    for (int i(0); i < nbCellSquared; i++) {
      for (int j(0); j < i; j++) {
        if ((tabCellDist[i][j] > 2) && (tabCellDist[i][j] != infinityDist)) {
          shortestIndirectPath(i, j, tabCellDist, pathAngles, infinityDist, nbCell);
        }
      }
    }
  }
  // cout << "gggg" << endl;
  /* !! Si start et target sont adjacents ou diagonales, on connait leur angle
  avant  la boucle _while_, ce qui rend inutile le fait de compléter les
  vector<vector<double> > ! Il faudrait que, dans ces cas, floyd ne complète pas
  sa tâche, ce qui serait très économique. De même, si une portion de la map est
  entièrement séparée du reste, comment arrêter l'algorithme et définir que les
  Players des deux côté de la frontière ne pourront pas se rejoindre à moins de
  provoquer la chute du mur ?
  De même, faudra penser à aller acheter ces 10kg de riz à Aligro :) */
}

void shortestIndirectPath(int startID, int targetID,
                          vector<vector<double>> &tabCellDist,
                          vector<vector<double>> &pathAngles, int infinityDist,
                          int nbCell) {
  int nbCellSquared = pow(nbCell, 2);
  for (int k(0); k < nbCellSquared; k++) {
    if ((tabCellDist[startID][k] != infinityDist) &&
        (tabCellDist[targetID][k] != infinityDist) && (k != startID) &&
        (k != targetID)) {
      if ((tabCellDist[startID][k] + tabCellDist[targetID][k]) <
          tabCellDist[startID][targetID]) {
        tabCellDist[startID][targetID] =
            tabCellDist[startID][k] + tabCellDist[targetID][k];
        tabCellDist[startID][targetID] =
            tabCellDist[startID][k] + tabCellDist[targetID][k];
        pathAngles[startID][targetID] = pathAngles[startID][k];
        pathAngles[targetID][startID] = pathAngles[targetID][k];
      }
    }
  }
}

void diagonalDistance(unsigned int i, unsigned int j,
                      vector<vector<double>> &tabCellDist,
                      vector<vector<double>> &pathAngles, ObstacleMap *obstacles) {
  int nbCell = obstacles->getLNb();
  // cout << "nbCell: " << nbCell << endl;
  int iC = i / nbCell;
  int iL = i % nbCell;
  int jC = j / nbCell;
  int jL = j % nbCell;

  // cout << "i: " << i << " j: " << j << " iC: " << iC << " iL: " << iL << " jC: " << jC
  //     << " jL: " << jL << endl;
  if (iL == jL + 1) { // j/i
    // cout << "J au dessus de I" << endl;
    if (iC == jC - 1) { // i-j
      // cout << "J a droite de I" << endl;
      if (obstacles->isObstacle(jL, jC - 1)) { // oj
        // cout << "a" << endl;
        tabCellDist[i][j] = 2;
        tabCellDist[j][i] = 2;
        pathAngles[i][j] = 0;
        pathAngles[j][i] = 3 * M_PI_2;
      } else if (obstacles->isObstacle(iL, iC + 1)) { // io
        // cout << "b" << endl;
        tabCellDist[i][j] = 2;
        tabCellDist[j][i] = 2;
        pathAngles[i][j] = M_PI_2;
        pathAngles[j][i] = M_PI;
      } else { //õ
        // cout << "c" << endl;
        tabCellDist[i][j] = sqrt(2);
        tabCellDist[j][i] = sqrt(2);
        pathAngles[i][j] = M_PI_4;
        pathAngles[j][i] = 5 * M_PI_4;
      }
    } else { // j-i
      // cout << "J a gauche de I" << endl;
      if (obstacles->isObstacle(jL, jC + 1)) { // jo
        // cout << "d" << endl;
        tabCellDist[i][j] = 2;
        tabCellDist[j][i] = 2;
        pathAngles[i][j] = M_PI;
        pathAngles[j][i] = 3 * M_PI_2;
      } else if (obstacles->isObstacle(iL, iC - 1)) { // oi
        // cout << "e" << endl;
        tabCellDist[i][j] = 2;
        tabCellDist[j][i] = 2;
        pathAngles[i][j] = M_PI_2;
        pathAngles[j][i] = 0;
      } else { //õ
        // cout << "f" << endl;
        tabCellDist[i][j] = sqrt(2);
        tabCellDist[j][i] = sqrt(2);
        pathAngles[i][j] = 3 * M_PI_4;
        pathAngles[j][i] = 7 * M_PI_4;
      }
    }
  } else { // i/j
    // cout << "J en dessous de I" << endl;
    if (iC == jC + 1) { // j-i
      // cout << "J a gauche de I" << endl;
      if (obstacles->isObstacle(jL, jC + 1)) { // jo
        // cout << "g" << endl;
        tabCellDist[i][j] = 2;
        tabCellDist[j][i] = 2;
        pathAngles[i][j] = M_PI;
        pathAngles[j][i] = M_PI_2;
      } else if (obstacles->isObstacle(iL, iC - 1)) { // oi
        // cout << "h" << endl;
        tabCellDist[i][j] = 2;
        tabCellDist[j][i] = 2;
        pathAngles[i][j] = 3 * M_PI_2;
        pathAngles[j][i] = 0;
      } else { //õ
        // cout << "i" << endl;
        tabCellDist[i][j] = sqrt(2);
        tabCellDist[j][i] = sqrt(2);
        pathAngles[i][j] = 5 * M_PI_4;
        pathAngles[j][i] = M_PI_4;
      }
    } else { // i-j
      // cout << "J a droite de I" << endl;
      // cout << "j" << endl;
      if (obstacles->isObstacle(jL, jC - 1)) { // oj
        tabCellDist[i][j] = 2;
        tabCellDist[j][i] = 2;
        pathAngles[i][j] = 0;
        pathAngles[j][i] = M_PI_2;
      } else if (obstacles->isObstacle(iL, iC + 1)) { // io
        // cout << "k" << endl;
        tabCellDist[i][j] = 2;
        tabCellDist[j][i] = 2;
        pathAngles[i][j] = 3 * M_PI_2;
        pathAngles[j][i] = 0;
      } else { //õ
        // cout << "l" << endl;
        tabCellDist[i][j] = sqrt(2);
        tabCellDist[j][i] = sqrt(2);
        pathAngles[i][j] = 7 * M_PI_4;
        pathAngles[j][i] = 3 * M_PI_4;
      }
    }
  }
}

double complexPath(Player *start, Player *target, int infinityInit, int infinityDist,
                   ObstacleMap *obstacles, bool &firstInStep) {
  /* Description of Floyd's algorithm's implementation */
  int nbCell = obstacles->getLNb();
  if (firstInStep) {
    cout << "-----Floyd------" << endl;
    floyd(start, target, infinityInit, infinityDist, obstacles);
    firstInStep = false;
    // cout << "Distance tab" << endl;
    for (int i = 0; i < tabCellDist.size(); i++) {
      for (int j = 0; j < tabCellDist[0].size(); j++) {
        // cout << tabCellDist[i][j] << " ";
      }
      // cout << endl;
    }
    // cout << "Angle tab (degrés)" << endl;
    for (int i = 0; i < pathAngles.size(); i++) {
      for (int j = 0; j < pathAngles[0].size(); j++) {
        // cout << (int)(pathAngles[i][j] * 180 / M_PI) << "     ";
      }
      // cout << endl;
    }
    cout << "--------------" << endl;
    return pathAngles[(start->getC() * nbCell) + start->getL()]
                     [(target->getC() * nbCell) + target->getL()];
  }
  return pathAngles[(start->getC() * nbCell) + start->getL()]
                   [(target->getC() * nbCell) + target->getL()];
}

double whichPath(Player *start, Player *target, int infinityInit, int infinityDist,
                 ObstacleMap *obstacles, bool &firstInStep) {
  if (isThereObstacleBetween(start->getL(), start->getC(), target->getL(),
                             target->getC(), obstacles)) {
    cout << "Complex path" << endl;
    return complexPath(start, target, infinityInit, infinityDist, obstacles,
                       firstInStep);
  } else {
    cout << "Simple path" << endl;
    return simplePath(start, target);
  }
}
