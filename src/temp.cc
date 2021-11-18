#include "settings.h"
#include <time.h>
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;

Clock game_clock ;
float timer=0 ;
float delay = 0.3 ;
float delays[9] = {1, 0.9, 0.8, 0.7, 0.6, 0.5, 0.4, 0.3, 0.2} ;
int cnt=0 ;

void Tetris(player &p){
    srand(time(0));
    RenderWindow scr(VideoMode(320, 480), "Setup") ;
    Texture t1, t2, t3 ;
	t1.loadFromFile("images/tiles.png") ;
	t2.loadFromFile("images/background.png") ;
	t3.loadFromFile("images/frame.png") ;
	Sprite block(t1), background(t2), frame(t3) ;  
    
    //Block block() ;
	while(scr.isOpen()){
		float time = game_clock.getElapsedTime().asSeconds() ;
		game_clock.restart() ;
		timer += time ;
 
        Event e ;  
        while(scr.pollEvent(e)){
            if (e.type == Event::Closed)
                scr.close() ;
            
            if(e.type == Event::KeyPressed){
                if(e.key.code == Keyboard::Up) p.set_rotate(true) ;
                else if(e.key.code == Keyboard::Left) p.set_move(-1) ;
                else if(e.key.code == Keyboard::Right) p.set_move(1) ;
                else if(e.key.code == Keyboard::Space) 
                    p.space_block(delay) ;            
            }
        }
        
        if(Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.03 ;

        if(p.get_move()) p.check_move(p.get_move()) ;
        if(p.get_rotate()) p.check_rotate() ;
    
        if(timer>delay){
            if(p.check_move_down()) p.move_down() ;
            else {
                p.fix_cur_block();
                p.generate_new_Block() ; 
                if(cnt < 89 ) cnt++ ; 
            }
            timer = 0;
        }
        p.line_clear() ;        
        p.set_move(0);
        p.set_rotate(false);
        delay = delays[cnt/10];
        std::cout << delay << std::endl;
        scr.clear(Color::White) ;
        scr.draw(background) ;

        ColoringBoard(scr, block, p);
        ColoringBlock(scr, block, p);

        scr.draw(frame) ;
        scr.display();
    }
}

void ColoringBoard(RenderWindow & scr, Sprite & block, player & p) { 
    for(size_t i=0 ; i<ROW ; i++){
        for(size_t j=0 ; j<COL ; j++){
            if(p.get_board(i, j)){
                block.setTextureRect(IntRect(p.get_board(i,j)*18, 0, 18, 18)) ;
                block.setPosition(j*18, i*18) ;
                block.move(28, 31) ;
                scr.draw(block) ;
            }
        }
    }
}

void ColoringBlock(RenderWindow & scr, Sprite & block, player & p) {
    for(size_t i=0 ; i<4 ; i++){
        Point Cur_pos = (p.get_Cur_Block()).get_Cur_pos(i);
        block.setTextureRect(IntRect(p.get_Cur_Block().get_TYPE()*18, 0, 18, 18)) ;
        block.setPosition(p.get_Cur_Block().get_Cur_pos(i).x*18, (p.get_Cur_Block().get_Cur_pos(i).y)*18) ;
        block.move(28, 31) ;
        scr.draw(block) ;       
    }
}

