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

int player::get_next_block(int cur) { return next_block.at(cur); }

int player::get_hold() {return hold;}
void player::set_hold(int n) {hold = n;}

bool player::get_hold_use() {return hold_use;}
void player::set_hold_use(bool use) {hold_use = use;}

int player::get_enemy_hold() { return enemy_hold;}
void player::set_enemy_hold(int n) { enemy_hold = n; }

int player::get_enemy_score() { return enemy_score; }
void player::set_enemy_score(int n) { enemy_score = n; }

bool player::get_win() { return win; }
void player::set_win(bool r) { win = r; }

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
        if(Cur_Block.Preoccupied_down(board) || Cur_Block.Out_Bottom()) break ;
        Cur_Block.MOVE();
    }
    delay= 0.001 ;
}

void player::down_block(float & delay){
    delay = 0.03 ;
}

void player::check_rotate(){

    Block rotated_Block = Cur_Block ; rotated_Block.ROTATE() ;
    
    if(rotated_Block.Preoccupied(board)) return  ;
    if(rotated_Block.Out_of_Range()) rotated_Block.MOVE(rotated_Block.Out_Left()-rotated_Block.Out_Right()) ;
    if(rotated_Block.Preoccupied(board)) return  ;
    if(rotated_Block.Out_Bottom()) return ;
    Cur_Block = rotated_Block ;
    return ;  
}

void player::fix_cur_block() {
    for(size_t i = 0; i<4; i++) {
        Point cur_point = Cur_Block.get_Cur_pos(i) ;
        board[cur_point.y][cur_point.x] = Cur_Block.get_TYPE() ; 
    }
}

char player::line_clear(){
    int k=ROW-1 ;
    int point=0 ;
    for(int i=ROW-1 ; i>=0 ; i--){
        int cnt = 0 ;
        for(size_t j=0  ;j<COL; j++){
            if(board[i][j]) cnt++ ;
            board[k][j] = board[i][j] ;
        }
    
        if(cnt != COL){
            k = k-1 ;
        }else{
            score += 10;
        }
    }

    //k == -1 means no clear
    int btb = 0;
    if(k == 3) {
        if(tetris) { 
            score += 45;
            btb = 1;
        }
    
        else 
            score += 30;
   
        tetris = true;
    }

    if(k != -1) {
        clear += k+1;
        if(k == 3) {
            tetris = true;
            sum_tetris++;
        }
        else tetris = false;
    }
    if(k == 2) score += 10;

    if(k != -1){
        for(size_t i = 0; i<k+1; i++) {
            for(size_t j=0 ; j<COL ; j++){
                board[i][j] = 0 ;
            }
        }
    }

    if(k==-1) combo = 0;
    else combo++;
    max_com = max_com > combo ? max_com : combo;

    score += ((combo-1)/2)*5;
    
    if(k == 3) return 'd'+(int)(combo/2.5)+btb;
    else if(k == 2) return 'b'+(int)(combo/2.5);
    return 'a'+(int)(combo/2.5);
}

void player::generate_new_Block(){ 
    Cur_Block = Block(next_block.at(0));
    next_block.erase(next_block.begin());
    generate_next_block();   
}

void player::generate_next_block() {
    srand(time(0));
    while(next_block.size() < 11) {
        int n = rand()%7;

        int cnt = 0;
        for(int i = 0; i<next_block.size(); i++) {
            if(next_block[i] == n)
                cnt++;
        }
        if(cnt > 1) continue;

        next_block.push_back(n);
    }
}

void player::hold_action() {
    if(hold == -1) {
        hold = Cur_Block.get_TYPE()-1;
        generate_new_Block();
    }
    else {
        int hold_block = hold;
        hold = Cur_Block.get_TYPE()-1;
        Cur_Block = Block(hold_block);
    }
}

void player::attacked(int num) {  
    int k ;

    for(k=0 ; k<num ; k++){
        if(Cur_Block.Preoccupied_down(board)){
            fix_cur_block() ;
            break ;
        }
        
        for(int i = num-k; i<ROW-k; i++) {
            for(int j = 0; j<COL; j++) {
                board[i-1][j] = board[i][j];
            }
        }
    }

    if(k!=num-1){
        for(int l=k ; l<num ; l++){        
            for(int i = num-l; i<ROW-l; i++) {
                for(int j = 0; j<COL; j++) {
                    if(i<1) break;
                    board[i-1][j] = board[i][j];
                    Cur_Block.move_up();
                }
            }
        }
    }
   
    makeline(num);
}

void player::makeline(int num) {
    int r = rand()%COL;

    for(int t = 0; t <num; t++) {
        for(int i = 0; i<4; i++) {
            if(Cur_Block.get_Cur_pos(i).y >= ROW-num)
                Cur_Block.move_up();
        }
    }

    for(int i = ROW-1; i>= ROW-num; i--) {
        for(int j = 0; j<COL; j++) {
            if(j == r) {
                board[i][j] = 0; 
                continue;
            }
            board[i][j] = 8;
        }
    }
}

void player::print_result() {
    using namespace std;

    std::cout << "Total score : " << score*10 << std::endl;
    std::cout << "The number of line cleared : " << clear << std::endl;
    std::cout << "The number of Tetris : " << sum_tetris << std::endl;
    std::cout << "Max combo : " << max_com << std::endl;
}
