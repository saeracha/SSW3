#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include <SFML/Graphics.hpp>
#include <time.h>
#include "player.h"
#include "block.h"
using namespace sf;

void Tetris(player &p) ;
void ColoringBlock(RenderWindow& scr, Sprite& img, player &);
void ColoringBoard(RenderWindow& scr, Sprite& img, player &);
void ColoringNextBlock(RenderWindow& scr, Sprite& img, player &);
void ColoringHoldBlock(RenderWindow& scr, Sprite& img, player &);
void ColoringScoreBoard(RenderWindow & scr, Sprite & zero, Sprite & one, Sprite & two, Sprite & three, Sprite & four, Sprite & five, Sprite & six, Sprite & seven, Sprite & eight, Sprite & nine,player & p) ;
#endif
