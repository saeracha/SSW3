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
#endif
