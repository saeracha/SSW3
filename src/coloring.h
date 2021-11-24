#ifndef __COLORING_H__
#define __COLORING_H__
#include <SFML/Graphics.hpp>
#include <time.h>
#include "block.h"
#include "player.h"

using namespace sf ;
void ColoringBoard(RenderWindow & scr, Sprite & block, player & p) ;
void ColoringBoard_Multi(RenderWindow & scr, Sprite & block, Sprite & block_enemy, player & p) ;
void ColoringBlock(RenderWindow & scr, Sprite & block, player & p) ;
void ColoringBlock_Multi(RenderWindow & scr, Sprite & block, Sprite & block_enemy, player & p) ; 
void ColoringNextBlock(RenderWindow & scr, Sprite & block, player & p) ;
void ColoringHoldBlock(RenderWindow & scr, Sprite & block, player & p) ; 
void ColoringScoreBoard(RenderWindow & scr, Sprite & blank, Sprite & zero, Sprite & one, Sprite & two, Sprite & three, Sprite & four, Sprite & five, Sprite & six, Sprite & seven, Sprite & eight, Sprite & nine, Sprite & margin, player &p) ;
void print_block(RenderWindow & scr, Sprite & block, int block_type, int pos1, int pos2, int off1, int off2) ;
void ColoringEnemyScoreBoard(RenderWindow & scr, Sprite & zero, Sprite & one, Sprite & two, Sprite & three, Sprite & four, Sprite & five, Sprite & six, Sprite & seven, Sprite & eight, Sprite & nine, Sprite & margin, player & p) ;
void print_number(RenderWindow & scr, Sprite & zero, Sprite & one, Sprite & two, Sprite & three, Sprite & four, Sprite & five, Sprite & six, Sprite & seven, Sprite & eight, Sprite & nine, Sprite & margin, int k, int i, int digit, int off1, int off2) ;

#endif
