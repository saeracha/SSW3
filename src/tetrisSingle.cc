#include "tetrisSingle.h"
#include <time.h>
#include <SFML/Graphics.hpp>
using namespace sf;

Clock game_clock ;
float timer=0 ;
float delay = 0.3 ;
float delays[9] = {1, 0.9, 0.8, 0.7, 0.6, 0.5, 0.4, 0.3, 0.2} ;
int cnt=0 ;

void Tetris(player &p){
    srand(time(0));
    RenderWindow scr(VideoMode(320, 480), "Setup") ;
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
                else if(e.key.code == Keyboard::Space) p.space_block(delay) ;      
                else if(e.key.code == Keyboard::LShift ) {
                    if(!p.get_hold_use()) { 
                        p.hold_action();
                        p.set_hold_use(true);    
                    }
                }
            }
        }
        if(Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.03 ;
        if(Keyboard::isKeyPressed(Keyboard::RShift)) delay = 1000 ;
        if(p.get_move()) p.check_move(p.get_move()) ;
        if(p.get_rotate()) p.check_rotate() ;
    
        if(timer>delay){
            if(p.check_move_down()) p.move_down() ;
            else {
                p.fix_cur_block();
                p.generate_new_Block() ; 
                if(cnt < 89 ) cnt++ ;
                p.set_hold_use(false);
            }
            timer = 0;
        }
        p.line_clear() ;        
        p.set_move(0);
        p.set_rotate(false);
        delay = delays[cnt/10];
        visual(scr, p) ;
        scr.display();
    }
}

void visual(RenderWindow & scr, player & p){
    Texture t1, t2, t3 ;
    Texture tt0, tt1, tt2, tt3, tt4, tt5, tt6, tt7, tt8, tt9, ttb ;
	t1.loadFromFile("./img/tiles.png") ;
	t2.loadFromFile("./img/background.png") ;
	t3.loadFromFile("./img/frame.png") ;
    tt0.loadFromFile("./img/zero.png") ;
    tt1.loadFromFile("./img/one.png") ;
    tt2.loadFromFile("./img/two.png") ;
    tt3.loadFromFile("./img/three.png") ;
    tt4.loadFromFile("./img/four.png") ;
    tt5.loadFromFile("./img/five.png") ;
    tt6.loadFromFile("./img/six.png") ;
    tt7.loadFromFile("./img/seven.png") ;
    tt8.loadFromFile("./img/eight.png") ;
    tt9.loadFromFile("./img/nine.png") ;
    ttb.loadFromFile("./img/blank.png") ;

	Sprite block(t1), background(t2), frame(t3) ;
    Sprite one(tt1), two(tt2), three(tt3), four(tt4), five(tt5), six(tt6), seven(tt7), eight(tt8), nine(tt9), zero(tt0), blank(ttb);   
    scr.clear(Color::White) ;
    scr.draw(background) ;
    ColoringBoard(scr, block, p);
    ColoringBlock(scr, block, p);
    ColoringNextBlock(scr, block, p);
    ColoringHoldBlock(scr, block, p); 
    ColoringScoreBoard(scr, blank, zero, one, two, three, four, five, six, seven, eight, nine, p) ;
    scr.draw(frame) ;
}



