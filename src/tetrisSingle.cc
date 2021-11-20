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
    Texture t1, t2, t3 ;
    Texture tt0, tt1, tt2, tt3, tt4, tt5, tt6, tt7, tt8, tt9 ;

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
   // tt0.loadFromFile("./img/zero.png") ;

	Sprite block(t1), background(t2), frame(t3) ;

    Sprite one(tt1), two(tt2), three(tt3), four(tt4), five(tt5), six(tt6), seven(tt7), eight(tt8), nine(tt9), zero(tt0) ;
    
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

        scr.clear(Color::White) ;
        scr.draw(background) ;

        ColoringBoard(scr, block, p);
        ColoringBlock(scr, block, p);
        ColoringNextBlock(scr, block, p);
        ColoringHoldBlock(scr, block, p);
        
        ColoringScoreBoard(scr,zero, one, two, three, four, five, six, seven, eight, nine, p) ;
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

void ColoringNextBlock(RenderWindow & scr, Sprite & block, player & p) {
    for(size_t j = 0; j<5; j++) {
        Block b(p.get_next_block(j));

        for(size_t i=0 ; i<4 ; i++){
            Point Cur_pos = b.get_Cur_pos(i);
            block.setTextureRect(IntRect(b.get_TYPE()*18, 0, 15, 15)) ;
            block.setPosition((b.get_Cur_pos(i).x+8)*18, b.get_Cur_pos(i).y*18+18*(j*3)+90) ;
            block.move(28, 40) ;
            scr.draw(block) ;      
        }    
    }
}

void ColoringHoldBlock(RenderWindow & scr, Sprite & block, player & p) {
    if(p.get_hold() == -1) return;
    Block b(p.get_hold());

    int boff = 7, boff_I = 7;
    if(b.get_TYPE()==1 || b.get_TYPE() == 3) boff += 7;
    if(b.get_TYPE()==1) boff_I += 10;

    for(size_t i=0 ; i<4 ; i++){
        Point Cur_pos = b.get_Cur_pos(i);
        block.setTextureRect(IntRect(b.get_TYPE()*18, 0, 15, 15)) ;
        block.setPosition(((b.get_Cur_pos(i).x)+10)*15+boff, b.get_Cur_pos(i).y*15+boff_I) ;
        block.move(28, 40) ;
        scr.draw(block) ;      
    }
}

void ColoringScoreBoard(RenderWindow & scr, Sprite & zero, Sprite & one, Sprite & two, Sprite & three, Sprite & four, Sprite & five, Sprite & six, Sprite & seven, Sprite & eight, Sprite & nine,player & p){
  /*  for(size_t i=0 ; i<ROW ; i++){
        for(size_t j=0 ; j<COL ; j++){
            if(p.get_board(i, j)){
                block.setTextureRect(IntRect(p.get_board(i,j)*18, 0, 18, 18)) ;
                block.setPosition(j*18, i*18) ;
                block.move(28, 31) ;
                scr.draw(block) ;
            }
        }
    }*/

    int score = p.get_score() ;
    std::vector <int> score_num ;
    int digit = 0 ;
    score_num.push_back(0) ;
    while(score > 0){
        score_num.push_back(score % 10) ;
        score = score / 10 ;
        digit ++ ;
    }

    for(size_t i=0 ; i<=digit ; i++){
        //write_num(score_num.at(i)) ;
        if(score_num.at(i) == 0){
            zero.setTextureRect(IntRect(0,0,24,36)) ; zero.setPosition(24*digit, 0) ; zero.move(28, 28) ; scr.draw(zero) ;
        }else if(score_num.at(i) == 1){
            one.setTextureRect(IntRect(0,0,24,36)) ; one.setPosition(24*digit,0) ; one.move(28, 28) ; scr.draw(one) ;
        }else if(score_num.at(i) == 2){
            two.setTextureRect(IntRect(0,0,24,36)) ; two.setPosition(24*digit,0) ; two.move(28, 28) ; scr.draw(two) ;
        }else if(score_num.at(i) == 3){
            three.setTextureRect(IntRect(0,0,24,36)) ; three.setPosition(24*digit,0) ; three.move(28, 28) ; scr.draw(three) ;
        }else if(score_num.at(i) == 4){
            four.setTextureRect(IntRect(0,0,24,36)) ; four.setPosition(24*digit,0) ; four.move(28, 28) ; scr.draw(four) ;
        }else if(score_num.at(i) == 5){
            five.setTextureRect(IntRect(0,0,24,36)) ; five.setPosition(24*digit,0) ; five.move(28, 28) ; scr.draw(five) ;
        }else if(score_num.at(i) == 6){
            six.setTextureRect(IntRect(0,0,24,36)) ; six.setPosition(24*digit,0) ; six.move(28, 28) ; scr.draw(six) ;
        }else if(score_num.at(i) == 7){
            seven.setTextureRect(IntRect(0,0,24,36)) ; seven.setPosition(24*digit,0) ; seven.move(28, 28) ; scr.draw(seven) ;
        }else if(score_num.at(i) == 8){
            eight.setTextureRect(IntRect(0,0,24,36)) ; eight.setPosition(24*digit,0) ; eight.move(28, 28) ; scr.draw(eight) ;
        }else if(score_num.at(i) == 9){
            nine.setTextureRect(IntRect(0,0,24,36)) ; nine.setPosition(24*digit,0) ; nine.move(28, 28) ; scr.draw(nine) ;     
        }
    }

    //zero.setTextureRect(IntRect(0,0,24,36)) ; zero.setPosition(0, 0) ; zero.move(28, 28) ; scr.draw(zero) ;

   
}
