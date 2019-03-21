#ifndef HEADER_ERROR_H
#define HEADER_ERROR_H

// ce fichier fourni des "macros" paramétrées destinées à être appelées pour les
// erreurs à détecter. Chaque symbole s'utilise dans une commande d'affichage
// et il est suivi par un passage à la ligne (cf exemples ci-dessous).
// 
// en général les paramètres sont les indices des entités concernées 
// compris entre 1 et nb_entité, SAUF pour OBSTACLE_VALUE_INCORRECT et MULTI_OBSTACLE
// pour lesquelles il on indique des indices de ligne/colonne d'obstacle.
// 
// ATTENTION: 
// ->  bien vérifier la nature du paramètre demandé
// ->  Pour un indice entre 1 et nb_entité, il s'agit de l'ordre dans le fichier
//
//     Exemple1: si la seconde balle indiquée dans le fichier
//              n'est pas dans le domaine du terrain de jeu :
//
//              cout << BALL_OUT(2) << endl;
//
//     Exemple2: si l'indice de ligne d'un obstacle quelconque vaut -1 :
//
//              cout << OBSTACLE_VALUE_INCORRECT(-1) << endl;
//
// version 28/02: version initiale
// version 08/03: avec message en cas de lecture sans erreur
// version 09/02: avec les deux dernier messages balle-balle et joueur-balle


#include <sstream>

#include "define.h"

// un seul message est destiné à indiquer l'absence d'erreur après la lecture complète
#define FILE_READING_SUCCESS				("File reading successful")

// le centre d'une balle est en dehors du domaine du terrain de jeu
#define BALL_OUT(PARAM)						("Ball out: " + std::to_string(PARAM))

// le centre d'un joueur est en dehors du terrain de jeu
#define PLAYER_OUT(PARAM)					("Player out: " + std::to_string(PARAM))

// les deux joueurs d'indices PARAM1 et PARAM2 sont en collision
#define PLAYER_COLLISION(PARAM1, PARAM2)	\
		("Player collision: " + std::to_string(PARAM1)	\
							  + " with " + std::to_string(PARAM2))

// le joueur d'indice PARAM1 est en collision avec la balle d'indice PARAM2
#define PLAYER_BALL_COLLISION(PARAM1, PARAM2)	\
		("Player  " + std::to_string(PARAM1)	\
				    + " colliding with ball " + std::to_string(PARAM2))


// la valeur PARAM indiquant l'indice incorrect de ligne ou de colonne 						  
#define OBSTACLE_VALUE_INCORRECT(PARAM)		\
		("Impossible value " + std::to_string(PARAM) + " for obstacle")
		
// les valeurs PARAM1 et PARAM2 sont respectivement les indices de ligne et colonne
// d'un obstacle qui est redéfini une seconde fois
#define MULTI_OBSTACLE(PARAM1, PARAM2)		\
		("Multiple definition of obstacle (" + std::to_string(PARAM1)	\
											 + ";" + std::to_string(PARAM2) + ")")

// collision obstacle-joueur
// le premier paramètre est l'indice de l'obstace, le second l'indice du joueur
#define COLL_OBST_PLAYER(PARAM1, PARAM2)	\
		("Obstacle " + std::to_string(PARAM1) + " in collision with player "	\
											  + std::to_string(PARAM2))

// les deux balles d'indices PARAM1 et PARAM2 sont en collision
#define BALL_COLLISION(PARAM1, PARAM2)	\
		("Ball collision: " + std::to_string(PARAM1)	\
							+ " with " + std::to_string(PARAM2))

// collision balle-obstacle
// l'unique paramètre est l'indice de la balle
#define COLL_BALL_OBSTACLE(PARAM)			\
		("Ball " + std::to_string(PARAM) + " in collision with an obstacle")

#endif
