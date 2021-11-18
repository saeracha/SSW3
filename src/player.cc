#include "player.h"

int player::get_score() {return score;}

int player::get_combo() {return combo;}

bool player::get_tetris() {return tetris;}

int player::get_move() {return move;}

bool player::get_rotate() {return rotate;}

int player::get_board(int y, int x) {return board[y][x];}

void player::set_score(int point) {score += point;}

Block player::get_Cur_Block() {return Cur_Block;}
Block player::get_Cur_Enemy_Block() { return Cur_Enemy_Block; }
int * player::get_board_address() {return &board[0][0];}

int player:: get_enemy_board(int y, int x) { return enemy_board[y][x]; }
void player::set_enemy_board(int y, int x, int value) {
    enemy_board[y][x] = value;
}

void player::set_combo(bool success) {
	if(success) combo++;
	else combo = 0;
}

void player::set_tetris(bool success) {
	if(success) tetris = true;
	else tetris = false;
}
   
void player::set_Enemy_Block(Block& block) {
    Cur_Enemy_Block = block;
}
void player::set_board(int y, int x, int value) {board[y][x] = value;}

void player::set_move(int move) {this->move = move;}

void player::set_rotate(bool rotate) {this->rotate = rotate;}

void player::check_move(int move){
    Block tmp = Cur_Block ; tmp.MOVE(move) ;
    if(!(tmp.Out_of_Range() || tmp.Preoccupied(board))) Cur_Block.MOVE(move) ; 
    return ;
}

void player::move_down(){ Cur_Block.MOVE() ; }

bool player::check_move_down(){
    if(Cur_Block.Out_Bottom() || Cur_Block.Preoccupied_down(board)) return false ;   
    else return true ;
}

void player::space_block(float & delay){
    while(1){
        //Cur_Block.MOVE() ;
        if(Cur_Block.Preoccupied_down(board) || Cur_Block.Out_Bottom()) break ;
        Cur_Block.MOVE();
    }
    delay= 0.001 ;
}

void player::check_rotate(){

    Block rotated_Block = Cur_Block ; rotated_Block.ROTATE() ;

    if(rotated_Block.Preoccupied(board)) return  ;
    if(rotated_Block.Out_of_Range()) rotated_Block.MOVE(rotated_Block.Out_Left()-rotated_Block.Out_Right()) ;
    if(rotated_Block.Preoccupied(board)) return  ;
    Cur_Block = rotated_Block ;
    return ;  
}

void player::fix_cur_block() {
    for(size_t i = 0; i<4; i++) {
        Point cur_point = Cur_Block.get_Cur_pos(i) ;
        board[cur_point.y][cur_point.x] = Cur_Block.get_TYPE() ; 
    }
}

void player::line_clear(){
    int k=ROW-1 ;

    for(int i=ROW-1 ; i>=0 ; i--){
        int cnt = 0 ;
        for(size_t j=0  ;j<COL; j++){
            if(board[i][j]) cnt++ ;
            board[k][j] = board[i][j] ;
        }
    
        if(cnt != COL){
            k = k-1 ;
        }
    }
    if(k != -1){
        for(size_t i=0 ; i<COL ; i++){
            board[0][i] = 0 ;
        }
    } 
}

void player::generate_new_Block(){ 
    Cur_Block = Next_Block ; 
    Next_Block = Block() ;    
}
