/*!
  \file   floyd.cc
  \author Guillaume Pellerin & Vincent Miche
  \date   May 2019
  \brief  pathfinding, with the Roy-Floyd-Warschall algorithm
  to check the size of each line (<=87 character): delete this line:
123456789012345678901234567890123456789012345678901234567890123456789012345678901234567
*/

#include <iostream>
#include <vector>
#include <cmath>
#include <define.h>

using namespace std;

#define INFINITY_INIT(nbCell**2 + 1);
#define INFINITY_DIST(nbCell^2);

vector<size_t> targetting(){
	vector<vector<double> > dBP(nbPlayer); //dBP = distance between players

	for (int i=0; i < nbPlayer; i++) {dBP[i][i] = 0;}
	
	for (int i=0; i < nbPlayer; i++) {
		for (int j=0; j < i; j++) {
			dBP[i][j] = distance(Player start, Player target);
			dBP[j][i] = dBP[i][j];
		}
	}
	
	vector<double> minDistance(nbPlayer);
	vector<size_t> target(nbPlayer);
	
	for (int i=0; i < nbPlayer; i++) {
		if (i == (nbPlayer - 1)) {
			minDistance[i] = dBP[i][i-1];
			target[i] = i - 1;
		} else {
			minDistance[i] = dBP[i][i+1];
			target[i] = i + 1;
		}
		for (int j=0; j < nbPlayer; j++) {
			if (i != j) {
				if (dBP[i][j] < minDistance[i]) {
					minDistance[i] = dBP[i][j];
					target[i] = j;
				}
			}
		}
	}
	return target;
}

void whichPath() {
    /*ajouter les arguments: 2 Players et les tableaux nécessaires
    à isThereObstacle (obstacle et joueurs), simplePath (idem),
    complexPath, floyd, diagonalDist et shortestindirectPath*/
	if (isThereObstacle(Player start, Player target)) {
		complexPath(start, target);
	} else {simplePath(start, target);}
}


