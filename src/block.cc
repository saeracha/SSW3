#include "block.h"

static int block_shape[7][4]={
    0, 1, 2, 3,
    2, 5, 6, 7,
    1, 2, 5, 6,
    1, 5, 6, 7,
    3, 5, 6, 7,
    1, 2, 6, 7,
    2, 3, 5, 6
};

Block:: Block(){
    int n = rand()%7 ;
    TYPE = n + 1;
    for(size_t i=0 ; i<4;  i++){
        Cur_pos[i].x = block_shape[n][i] %4 + 3 ;
        Cur_pos[i].y = block_shape[n][i] /4 ;
    }
}

Block::Block(int n) {
    TYPE = n + 1;
    for(size_t i=0 ; i<4;  i++){
        Cur_pos[i].x = block_shape[n][i] %4 + 3 ;
        Cur_pos[i].y = block_shape[n][i] /4 ;
    }
}

Point Block::get_Cur_pos(int cur) {return Cur_pos[cur];}

int Block::get_TYPE(){ return TYPE ; }
void Block::MOVE(int move){
    for(size_t i=0 ; i<4 ; i++){
        Cur_pos[i].x = Cur_pos[i].x + move ;
    }
}

void Block::move_up() {
    for(size_t i = 0; i<4; i++) {
            Cur_pos[i].y--;    
    }
}

void Block::MOVE(){
    for(size_t i=0 ; i<4 ; i++){
        Cur_pos[i].y++ ;
    }
}

void Block::set_TYPE(int type) {
    TYPE = type;
}

void Block::set_Cur_pos(int cur, int y, int x) {
    Cur_pos[cur].y = y;
    Cur_pos[cur].x = x;
}

void Block::ROTATE(){
    
    Point center ; 
    if(TYPE==1 || TYPE==6) center = Cur_pos[1] ;
    else if(TYPE==2 || TYPE==4 || TYPE==5) center = Cur_pos[2] ;
    else if(TYPE==7) center = Cur_pos[0] ;
    else if(TYPE==3) return ;

    for(size_t i=0 ; i<4 ; i++){
        int tmp = Cur_pos[i].x ;
        Cur_pos[i].x = center.x-(Cur_pos[i].y - center.y) ;
        Cur_pos[i].y = center.y+(tmp - center.x) ;
    }
    return ;
}

bool Block::Preoccupied_down(int board[ROW][COL]){
    for(size_t i=0 ; i<4 ; i++){
        if(board[Cur_pos[i].y+1][Cur_pos[i].x] > 0) return true ;
    }
    return false ;
}

bool Block::Preoccupied(int board[ROW][COL]){
    for(size_t i=0 ; i<4 ; i++){
        if(board[Cur_pos[i].y][Cur_pos[i].x] > 0) return true ;
    }
    return false ;
}


bool Block::Out_of_Range(){
    if(this->Out_Left()>0 || this->Out_Right()>0) return true ;
    else return false ;
}

int Block::Out_Left(){
    int ans=0 ;
    for(size_t i=0 ; i<4 ; i++){
        if(Cur_pos[i].x<0){
            if(-Cur_pos[i].x > ans) ans = -Cur_pos[i].x ;
        } 
    }
    return ans ;
}

int Block::Out_Right(){
    int ans=0 ;
    for(size_t i=0 ; i<4 ; i++){
        if(Cur_pos[i].x>COL-1){
            if(Cur_pos[i].x-COL+1 > ans) ans = Cur_pos[i].x-COL+1 ;
        } 
    }
    return ans ;
}

bool Block::Out_Bottom(){
    for(size_t i=0  ; i<4 ; i++){
        if(Cur_pos[i].y+1 >= ROW)
            return true ;
    }
    return false ;
}

