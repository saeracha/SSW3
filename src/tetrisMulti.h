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
/*void ColoringBoardMulti(RenderWindow& scr, Sprite & block);
void ColoringBlockMulti(RenderWindow& scr, Sprite & block);
void ColoringHoldBlock(RenderWindow& scr, Sprite & block);
void ColoringNextBlock(RenderWindow& scr, Sprite & block);*/
void Convert(char * buf);
/*void ColoringScoreBoardMulti(RenderWindow & scr, Sprite & zero, Sprite & one, Sprite & two, Sprite & three, Sprite & four, Sprite & five, Sprite & six, Sprite & seven, Sprite & eight, Sprite & nine,player & p) ;
void ColoringEnemyScoreBoardMulti(RenderWindow & scr, Sprite & zero, Sprite & one, Sprite & two, Sprite & three, Sprite & four, Sprite & five, Sprite & six, Sprite & seven, Sprite & eight, Sprite & nine,player & p) ;
void print_number_Multi(RenderWindow & scr, Sprite & zero, Sprite & one, Sprite & two, Sprite & three, Sprite & four, Sprite & five, Sprite & six, Sprite & seven, Sprite & eight, Sprite & nine, int k, int i, int digit, int off1, int off2) ;*/ 


#endif
