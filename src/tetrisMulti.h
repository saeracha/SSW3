#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <SFML/Graphics.hpp>
#include <time.h>
#include "player.h"
#include "block.h"
#include "coloring.h"

using namespace sf;

#define WELL_KNOWN_IP   "127.0.0.1"
#define WELL_KNOWN_PORT 8000
#define RC          ROW*COL
#define BUF_SIZE    ROW*COL+17

void TetrisBattle(int & sock);
void draw(char * data, RenderWindow& scr);
void * Tetris(void * arg);
void * ReadEnemy(void * arg);
void visual_Multi(RenderWindow & scr, player & p) ;
void Convert(char * buf);
void Win(RenderWindow & scr, int sock);
void Lose(RenderWindow & scr, int sock);
void init(player & p, RenderWindow & scr);

#endif
