/*!
  \file   floyd.cc
  \author Guillaume Pellerin & Vincent Miche
  \date   May 2019
  \brief  pathfinding, with the Roy-Floyd-Warschall algorithm
*/

/*
 * We use A* algorithmby default, we don't recommand using this one, as we have some
 * angle errors and we don't have time to correct it for now. See github branch
 * floydDebug for more info, such as debug cout, comments everywhere and more recent
 * updates.
 */

#include <cmath>
#include <vector>

#include "define.h"
#include "obstacle.h"
#include "player.h"
#include "tools.h"

using namespace std;

static vector<vector<double>> tabDist;
static vector<vector<double>> angles;
static vector<vector<int>> previousObstacles;

bool isThereObstacleBetween(int l1, int c1, int l2, int c2, ObstacleMap *obstacles);
bool isThereObstacleInColumn(int l1, int c1, int l2, int c2, ObstacleMap *obstacles);
bool isThereObstacleInLine(int l1, int c1, int l2, int c2, ObstacleMap *obstacles);
double simplePath(Player *start, Player *target);
void complexPath(Player *start, Player *target, int infinityInit, int infinityDist,
                 bool &firstInStep);
void floyd(Player *start, Player *target, int infinityInit, int infinityDist);
void diagDist(unsigned int i, unsigned int j, ObstacleMap *obstacles);
void diagDistFromUp(unsigned int i, unsigned int j, int iC, int iL, int jC, int jL,
                    ObstacleMap *obstacles);
void diagDistFromDown(unsigned int i, unsigned int j, int iC, int iL, int jC, int jL,
                      ObstacleMap *obstacles);
bool areThereUninitialisedCases(vector<vector<double>> tab2D, int infinityInit);
void shortestIndirectPath(int startID, int targetID, int infinityDist, int nbCell);
bool sameObstacles(ObstacleMap *obstacles);

bool areThereUninitialisedCases(vector<vector<double>> tab2D, int infinityInit) {
    for (int i(0); i < tab2D.size(); i++) {
        for (int j(0); j < i; j++) {
            if (tab2D[i][j] == infinityInit) { return true; }
        }
    }
    return false;
}

bool isThereObstacleInLine(int l1, int c1, int l2, int c2, ObstacleMap *obstacles) {
    if (c1 < c2) {
        for (int c = c1; c <= c2; c++) {
            if (obstacles->isObstacle(l2, c)) { return true; }
        }
        return false;
    } else {
        for (int c = c2; c <= c1; c++) {
            if (obstacles->isObstacle(l2, c)) { return true; }
        }
        return false;
    }
}

bool isThereObstacleInColumn(int l1, int c1, int l2, int c2, ObstacleMap *obstacles) {
    if (l1 < l2) {
        for (int l = l1; l <= l2; l++) {
            if (obstacles->isObstacle(l, c2)) { return true; }
        }
        return false;
    } else {
        for (int l = l2; l <= l1; l++) {
            if (obstacles->isObstacle(l, c2)) { return true; }
        }
        return false;
    }
}

bool isThereObstacleBetween(int l1, int c1, int l2, int c2, ObstacleMap *obstacles) {
    if (l1 == l2) {
        return isThereObstacleInLine(l1, c1, l2, c2, obstacles);
    } else if (c1 == c2) {
        return isThereObstacleInColumn(l1, c1, l2, c2, obstacles);
    } else {
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
                if (obstacles->isObstacle(l, c)) { return true; }
                direction = column;
            } else {
                if (c < c2) {
                    c++;
                } else if (c > c2) {
                    c--;
                } else {
                    direction = line;
                    continue;
                }
                if (obstacles->isObstacle(l, c)) { return true; }
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
    tabDist.resize(nbCellSquared, vector<double>(nbCellSquared, infinityInit));
    angles.resize(nbCellSquared, vector<double>(nbCellSquared, 0));
    previousObstacles.resize(nbCell, vector<int>(nbCell, 0));
    for (int i(0); i < nbCellSquared; i++) {
        for (int j(0); j < nbCellSquared; j++) { tabDist[i][j] = infinityInit; }
        for (int j(0); j < i; j++) {
            if ((i / nbCell) == (j / nbCell)) {
                if ((i % nbCell) == ((j % nbCell) - 1)) {
                    tabDist[i][j] = 1;
                    tabDist[j][i] = 1;
                    angles[i][j] = 0;
                    angles[j][i] = M_PI;
                } else if ((i % nbCell) == ((j % nbCell) + 1)) {
                    tabDist[i][j] = 1;
                    tabDist[j][i] = 1;
                    angles[i][j] = M_PI;
                    angles[j][i] = 0;
                }
            } else if ((i % nbCell) == (j % nbCell)) {
                if ((i / nbCell) == ((j / nbCell) - 1)) {
                    tabDist[i][j] = 1;
                    tabDist[j][i] = 1;
                    angles[i][j] = 3 * M_PI_2;
                    angles[j][i] = M_PI_2;
                } else if ((i / nbCell) == ((j / nbCell) + 1)) {
                    tabDist[i][j] = 1;
                    tabDist[j][i] = 1;
                    angles[i][j] = M_PI_2;
                    angles[j][i] = 3 * M_PI_2;
                }
            }
        }
    }
    for (int i(0); i < nbCellSquared; i++) {
        if (obstacles->isObstacle(i % nbCell, i / nbCell)) {
            for (int j(0); j < nbCellSquared; j++) {
                tabDist[i][j] = infinityDist;
                tabDist[j][i] = infinityDist;
            }
        }
        tabDist[i][i] = 0;
    }
    for (int i(0); i < nbCellSquared - nbCell; i++) {
        if (i % nbCell != nbCell - 1) { diagDist(i, i + nbCell + 1, obstacles); }
        if (i % nbCell != 0) { diagDist(i, i + nbCell - 1, obstacles); }
    }
    int g = 0;
    while (areThereUninitialisedCases(tabDist, infinityInit) && g < 100) {
        g++;
        for (int i(0); i < nbCellSquared; i++) {
            for (int j(0); j < i; j++) {
                if ((tabDist[i][j] > 2) && (tabDist[i][j] != infinityDist)) {
                    shortestIndirectPath(i, j, infinityDist, nbCell);
                }
            }
        }
    }
    for (int i = 0; i < nbCell; i++) {
        for (int j = 0; j < nbCell; j++) {
            previousObstacles[i][j] = obstacles->whichObstacle(i, j);
        }
    }
}

void shortestIndirectPath(int start, int target, int infinityDist, int nbCell) {
    int nbCellSquared = pow(nbCell, 2);
    for (int k(0); k < nbCellSquared; k++) {
        if ((tabDist[start][k] != infinityDist) &&
            (tabDist[target][k] != infinityDist) && (k != start) && (k != target)) {
            if ((tabDist[start][k] + tabDist[target][k]) < tabDist[start][target]) {
                tabDist[start][target] = tabDist[start][k] + tabDist[target][k];
                tabDist[start][target] = tabDist[start][k] + tabDist[target][k];
                angles[start][target] = angles[start][k];
                angles[target][start] = angles[target][k];
            }
        }
    }
}

void diagDistFromUp(unsigned int i, unsigned int j, int iC, int iL, int jC, int jL,
                    ObstacleMap *obstacles) {
    if (iC == jC - 1) {
        if (obstacles->isObstacle(jL, jC - 1)) {
            tabDist[i][j] = 2;
            tabDist[j][i] = 2;
            angles[i][j] = 0;
            angles[j][i] = 3 * M_PI_2;
        } else if (obstacles->isObstacle(iL, iC + 1)) {
            tabDist[i][j] = 2;
            tabDist[j][i] = 2;
            angles[i][j] = M_PI_2;
            angles[j][i] = M_PI;
        } else {
            tabDist[i][j] = sqrt(2);
            tabDist[j][i] = sqrt(2);
            angles[i][j] = M_PI_4;
            angles[j][i] = 5 * M_PI_4;
        }
    } else {
        if (obstacles->isObstacle(jL, jC + 1)) {
            tabDist[i][j] = 2;
            tabDist[j][i] = 2;
            angles[i][j] = M_PI;
            angles[j][i] = 3 * M_PI_2;
        } else if (obstacles->isObstacle(iL, iC - 1)) {
            tabDist[i][j] = 2;
            tabDist[j][i] = 2;
            angles[i][j] = M_PI_2;
            angles[j][i] = 0;
        } else {
            tabDist[i][j] = sqrt(2);
            tabDist[j][i] = sqrt(2);
            angles[i][j] = 3 * M_PI_4;
            angles[j][i] = 7 * M_PI_4;
        }
    }
}

void diagDistFromDown(unsigned int i, unsigned int j, int iC, int iL, int jC, int jL,
                      ObstacleMap *obstacles) {
    if (iC == jC + 1) {
        if (obstacles->isObstacle(jL, jC + 1)) {
            tabDist[i][j] = 2;
            tabDist[j][i] = 2;
            angles[i][j] = M_PI;
            angles[j][i] = M_PI_2;
        } else if (obstacles->isObstacle(iL, iC - 1)) {
            tabDist[i][j] = 2;
            tabDist[j][i] = 2;
            angles[i][j] = 3 * M_PI_2;
            angles[j][i] = 0;
        } else {
            tabDist[i][j] = sqrt(2);
            tabDist[j][i] = sqrt(2);
            angles[i][j] = 5 * M_PI_4;
            angles[j][i] = M_PI_4;
        }
    } else {
        if (obstacles->isObstacle(jL, jC - 1)) {
            tabDist[i][j] = 2;
            tabDist[j][i] = 2;
            angles[i][j] = 0;
            angles[j][i] = M_PI_2;
        } else if (obstacles->isObstacle(iL, iC + 1)) {
            tabDist[i][j] = 2;
            tabDist[j][i] = 2;
            angles[i][j] = 3 * M_PI_2;
            angles[j][i] = 0;
        } else {
            tabDist[i][j] = sqrt(2);
            tabDist[j][i] = sqrt(2);
            angles[i][j] = 7 * M_PI_4;
            angles[j][i] = 3 * M_PI_4;
        }
    }
}

void diagDist(unsigned int i, unsigned int j, ObstacleMap *obstacles) {
    int nbCell = obstacles->getLNb();
    int iC = i / nbCell;
    int iL = i % nbCell;
    int jC = j / nbCell;
    int jL = j % nbCell;

    if (iL == jL + 1) { // j/i
        diagDistFromUp(i, j, iC, iL, jC, jL, obstacles);
    } else { // i/j
        diagDistFromDown(i, j, iC, iL, jC, jL, obstacles);
    }
}

bool sameObstacles(ObstacleMap *obstacles) {
    if (previousObstacles.size() != obstacles->getCNb()) {
        return false;
    } else {
        for (int i = 0; i < obstacles->getCNb(); i++) {
            for (int j = 0; j < obstacles->getCNb(); j++) {
                if (previousObstacles[i][j] != obstacles->whichObstacle(i, j)) {
                    return false;
                }
            }
        }
    }
    return true;
}

double complexPath(Player *start, Player *target, int infinityInit, int infinityDist,
                   ObstacleMap *obstacles, bool &firstInStep) {
    int nbCell = obstacles->getLNb();
    if (firstInStep) {
        if (not sameObstacles(obstacles)) {
            floyd(start, target, infinityInit, infinityDist, obstacles);
            firstInStep = false;
            return angles[(start->getC() * nbCell) + start->getL()]
                         [(target->getC() * nbCell) + target->getL()];
        }
    }
    return angles[(start->getC() * nbCell) + start->getL()]
                 [(target->getC() * nbCell) + target->getL()];
}

double whichPath(Player *start, Player *target, int infinityInit, int infinityDist,
                 ObstacleMap *obstacles, bool &firstInStep) {
    if (isThereObstacleBetween(start->getL(), start->getC(), target->getL(),
                               target->getC(), obstacles)) {
        return complexPath(start, target, infinityInit, infinityDist, obstacles,
                           firstInStep);
    } else {
        return simplePath(start, target);
    }
}
