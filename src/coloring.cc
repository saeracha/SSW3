#include "coloring.h"


void ColoringBoard(RenderWindow & scr, Sprite & block, player & p) { 
    for(size_t i=0 ; i<ROW ; i++){
        for(size_t j=0 ; j<COL ; j++){
            if(p.get_board(i, j)){
                print_block(scr, block, p.get_board(i,j), j, i, 28, 31) ;
            }
        }
    }
}

void ColoringBoard_Multi(RenderWindow & scr, Sprite & block, Sprite & block_enemy, player & p){
     for(size_t i=0 ; i<ROW ; i++){
        for(size_t j=0 ; j<COL ; j++){
            if(p.get_board(i, j)){
                print_block(scr, block, p.get_board(i,j), j, i, 28, 31) ;
            }
            if(p.get_enemy_board(i, j)){
                print_block(scr, block_enemy, p.get_enemy_board(i,j), j+18, i, 28, 31) ;
            }
        }
    }  
}

void ColoringBlock(RenderWindow & scr, Sprite & block, player & p) {
    for(size_t i=0 ; i<4 ; i++){
        Point Cur_pos = (p.get_Cur_Block()).get_Cur_pos(i);
        print_block(scr, block, p.get_Cur_Block().get_TYPE(), Cur_pos.x, Cur_pos.y, 28, 31) ;   
    }
}

void ColoringBlock_Multi(RenderWindow & scr, Sprite & block, Sprite & block_enemy, player & p) {
    for(size_t i=0 ; i<4 ; i++){
        Point Cur_pos = (p.get_Cur_Block()).get_Cur_pos(i);
        print_block(scr, block, p.get_Cur_Block().get_TYPE(), Cur_pos.x, Cur_pos.y, 28, 31) ;      
    }

    if(p.get_Cur_Enemy_Block().get_TYPE() == 0) return;
    for(size_t i=0 ; i<4 ; i++){
        Point Cur_pos = (p.get_Cur_Enemy_Block()).get_Cur_pos(i);
        print_block(scr, block_enemy, p.get_Cur_Enemy_Block().get_TYPE(), Cur_pos.x+18, Cur_pos.y, 28, 31) ;          
    }
}

void ColoringNextBlock(RenderWindow & scr, Sprite & block, player & p) {
    for(size_t j = 0; j<5; j++) {
        Block b(p.get_next_block(j));

        for(size_t i=0 ; i<4 ; i++){
            Point Cur_pos = b.get_Cur_pos(i);
             print_block(scr, block, b.get_TYPE(), Cur_pos.x+8, Cur_pos.y + j*3 + 5, 28, 40) ;
         }    
    }
}

void ColoringEnemyScoreBoard(RenderWindow & scr, Sprite & zero, Sprite & one, Sprite & two, Sprite & three, Sprite & four, Sprite & five, Sprite & six, Sprite & seven, Sprite & eight, Sprite & nine, Sprite & margin, player & p){

    int score = p.get_enemy_score() ;
    std::vector <int> score_num ;
    int digit = 0 ;
    int i=0 ;
    score_num.push_back(0) ;

    while(score > 0){
        score_num.push_back(score % 10) ;
        score = score / 10 ;
        digit ++ ;
    }
    for(size_t i=0 ; i<=digit ; i++){
        print_number(scr, zero, one, two, three, four, five, six, seven, eight, nine, margin, score_num.at(i), i, digit, 362, 410) ;   
    }
   
}

void ColoringScoreBoard(RenderWindow & scr, Sprite & blank, Sprite & zero, Sprite & one, Sprite & two, Sprite & three, Sprite & four, Sprite & five, Sprite & six, Sprite & seven, Sprite & eight, Sprite & nine, Sprite & margin, player &p){
    for(int i=0 ; i<10 ; i++){
         blank.setTextureRect(IntRect(0,0,24,36)) ; blank.setPosition(24*i, 0) ; blank.move(28, 410) ; scr.draw(blank) ;
    }

    int score = p.get_score() ;
    std::vector <int> score_num ;
    int score_digit = 0 ;
    score_num.push_back(0) ;
    while(score > 0){
        score_num.push_back(score % 10) ;
        score = score / 10 ;
        score_digit ++ ;
    }
    for(size_t j=0 ; j<=score_digit ; j++){
        print_number(scr, zero, one, two, three, four, five, six, seven, eight, nine, margin, score_num.at(j), j, score_digit, 28, 410) ;   
    }
    int combo = p.get_combo() ;
    std::vector <int> combo_num ; int com_digit=0 ;
    if(combo==0){
        combo_num.push_back(0) ; com_digit ++ ;
    }
    else{
        while(combo>0){
            combo_num.push_back(combo%10) ;
            combo = combo/10 ;
            com_digit ++ ;
        }
    }
    for(size_t k=0 ; k<com_digit ; k++){
        print_number(scr, zero, one, two, three, four, five, six, seven, eight, nine, margin, combo_num.at(k), k, com_digit, 220, 410) ;
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


void print_block(RenderWindow & scr, Sprite & block, int block_type, int pos1, int pos2, int off1, int off2){
    block.setTextureRect(IntRect(block_type*18, 0, 18, 18)) ;
    block.setPosition(pos1*18, pos2*18) ;
    block.move(off1, off2) ;
    scr.draw(block) ;
}

void print_number(RenderWindow & scr, Sprite & zero, Sprite & one, Sprite & two, Sprite & three, Sprite & four, Sprite & five, Sprite & six, Sprite & seven, Sprite & eight, Sprite & nine, Sprite & margin, int k, int i, int digit, int off1, int off2){
        if(k== 0){
            zero.setTextureRect(IntRect(0,0,24,36)) ; zero.setPosition(30*(digit-i), 0) ; zero.move(off1, off2) ; scr.draw(zero) ;
        }else if(k == 1){
            one.setTextureRect(IntRect(0,0,24,36)) ; one.setPosition(30*(digit-i),0) ; one.move(off1, off2) ; scr.draw(one) ;
        }else if(k == 2){
            two.setTextureRect(IntRect(0,0,24,36)) ; two.setPosition(30*(digit-i),0) ; two.move(off1, off2) ; scr.draw(two) ;
        }else if(k == 3){
            three.setTextureRect(IntRect(0,0,24,36)) ; three.setPosition(30*(digit-i),0) ; three.move(off1, off2); scr.draw(three) ;
        }else if(k == 4){
            four.setTextureRect(IntRect(0,0,24,36)) ; four.setPosition(30*(digit-i),0) ; four.move(off1, off2); scr.draw(four) ;
        }else if(k == 5){
            five.setTextureRect(IntRect(0,0,24,36)) ; five.setPosition(30*(digit-i),0) ; five.move(off1, off2) ; scr.draw(five) ;
        }else if(k == 6){
            six.setTextureRect(IntRect(0,0,24,36)) ; six.setPosition(30*(digit-i),0) ; six.move(off1, off2); scr.draw(six) ;
        }else if(k == 7){
            seven.setTextureRect(IntRect(0,0,24,36)) ; seven.setPosition(30*(digit-i),0) ; seven.move(off1, off2); scr.draw(seven) ;
        }else if(k == 8){
            eight.setTextureRect(IntRect(0,0,24,36)) ; eight.setPosition(30*(digit-i),0) ; eight.move(off1, off2) ; scr.draw(eight) ;
        }else if(k == 9){
            nine.setTextureRect(IntRect(0,0,24,36)) ; nine.setPosition(30*(digit-i),0) ; nine.move(off1, off2); scr.draw(nine) ;     
        }
        margin.setTextureRect(IntRect(0,0,6,36)) ; margin.setPosition(30*(digit-i),0) ; margin.move(off1+24, off2) ; scr.draw(margin) ;
}

