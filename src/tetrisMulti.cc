#include "tetrisSingle.h"
#include "tetrisMulti.h"
#include <time.h>
#include <SFML/Graphics.hpp>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
using namespace sf;

static Clock game_clock ;
static float timer=0 ;
static float delay = 0.3 ;
static float delays[9] = {1, 0.9, 0.8, 0.7, 0.6, 0.5, 0.4, 0.3, 0.2} ;
static int cnt=0 ;
static pthread_mutex_t m;
//static bool getout = false;

player p;

typedef struct MultiArg {
    int sock;
    RenderWindow* scr;
}MultiArg;

void TetrisBattle(int & sock) {
    pthread_t read_thread, send_thread;
    void * thread_return;

    srand(time(0));
    RenderWindow scr(VideoMode(640, 480), "Tetirs");

    MultiArg * multiarg;
    multiarg = new MultiArg;
    multiarg->sock = sock;
    multiarg->scr = &scr;

    pthread_mutex_init(&m, NULL);
	pthread_create(&send_thread, NULL, Tetris, (void*)multiarg);
	pthread_create(&read_thread, NULL, ReadEnemy, (void*)multiarg);
	pthread_join(send_thread, &thread_return);
	pthread_join(read_thread, &thread_return);

    delete multiarg;
}


void * Tetris(void * arg){
    MultiArg * multiarg = (MultiArg*)arg;
    int sock = multiarg->sock;
    RenderWindow * scr = multiarg->scr;


    /*
    Texture wait;
    wait.loadFromFile("./img/frame.png");
    Sprite wait_s(wait);

    (*scr).clear(Color::White) ;
    (*scr).draw(wait_s);
    (*scr).display();
    (*scr).clear(Color::White) ;
*/


    Texture t1, t2, t3 ;
    Texture tt0, tt1, tt2, tt3, tt4, tt5, tt6, tt7, tt8, tt9 ;
	t1.loadFromFile("./img/tiles.png") ;
	t2.loadFromFile("./img/background.png") ;
	t3.loadFromFile("./img/frame.png") ;
	Sprite block(t1), background(t2), frame(t3) ;  
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
    Sprite one(tt1), two(tt2), three(tt3), four(tt4), five(tt5), six(tt6), seven(tt7), eight(tt8), nine(tt9), zero(tt0) ;
	while((*scr).isOpen()){
		float time = game_clock.getElapsedTime().asSeconds() ;
		game_clock.restart() ;
		timer += time ;
 
        Event e ;  
        while((*scr).pollEvent(e)){
            if (e.type == Event::Closed) {
                close(sock);
                (*scr).close() ;
            }
            
            if(e.type == Event::KeyPressed){
                if(e.key.code == Keyboard::Up) p.set_rotate(true) ;
                else if(e.key.code == Keyboard::Left) p.set_move(-1) ;
                else if(e.key.code == Keyboard::Right) p.set_move(1) ;
                else if(e.key.code == Keyboard::Space) p.space_block(delay) ; 
                else if(e.key.code == Keyboard::LShift) {
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

        (*scr).clear(Color::White) ;
        (*scr).draw(background) ;

        ColoringBoardMulti(*scr, block);
        ColoringBlockMulti(*scr, block);
        ColoringNextBlock(*scr, block);
        ColoringHoldBlock(*scr, block);
    //    ColoringScoreBoardMulti(*scr, zero, one, two, three, four, five, six, seven, eight, nine, p) ;

        //write
        char * temp = new char[BUF_SIZE];
        Convert(temp);
        int len = write(sock, temp, BUF_SIZE);

        (*scr).draw(frame) ;
        (*scr).display();
    }
    return NULL;
}

void * ReadEnemy(void * arg) {
     
    MultiArg * multiarg = (MultiArg*)arg;
    int sock = multiarg->sock;
    RenderWindow * scr = multiarg->scr;
    char data[BUF_SIZE];
    int len;

    while(true) {
        len = read(sock, data, BUF_SIZE);
        if(len != BUF_SIZE) continue;
        if(len == -1) return NULL;
        if(data[BUF_SIZE-1] <= '7' ) continue;

        draw(data, *scr);
    }
    
    return NULL;
}

void draw(char * data, RenderWindow & scr) {
    for(size_t i=0 ; i<ROW ; i++)
        for(size_t j=0 ; j<COL ; j++)
                p.set_enemy_board(i, j, data[i*COL+j]-'0');
     
    Block temp_block;
    temp_block.set_TYPE(data[RC]-'0');

    for(size_t i = 0; i<4; i++) 
        temp_block.set_Cur_pos(i, data[RC+i*2+1]-'0', data[RC+i*2+2]-'0');

    p.set_Enemy_Block(temp_block);

    p.set_enemy_hold(data[RC+9]-'0');
    
    int sum = 0;
    for(int i = 0; i<5; i++) {
        sum *= 10;
        sum += data[RC+10+i] -'0';
    }
    p.set_enemy_score(sum);
}

void Convert(char * buf) {
    for(int i = 0; i<ROW; i++) 
        for(int j =0; j<COL; j++) 
            buf[i*COL+j] = p.get_board(i, j) + '0';
   
    buf[RC] = p.get_Cur_Block().get_TYPE() + '0';

    for(int i = 0; i<4; i++) { 
            buf[RC+1+i*2] = p.get_Cur_Block().get_Cur_pos(i).y +'0';
            buf[RC+1+i*2+1] = p.get_Cur_Block().get_Cur_pos(i).x + '0';
    }

    buf[RC+9] = p.get_hold() + '0';
    
    int score = p.get_score();
    for(int i = 5; i>=0; i--) {
        buf[RC+10+i] = (score%10)+'0';
        score /= 10;
    }

    buf[BUF_SIZE-1] = '9'; 
    buf[BUF_SIZE] = 0;
}

void ColoringBoardMulti(RenderWindow & scr, Sprite & block) { 
    for(size_t i=0 ; i<ROW ; i++){
        for(size_t j=0 ; j<COL ; j++){
            if(p.get_board(i, j)){
                block.setTextureRect(IntRect(p.get_board(i,j)*18, 0, 18, 18)) ;
                block.setPosition(j*18, i*18) ;
                block.move(28, 31) ;
                scr.draw(block) ;
            }
 
            if(p.get_enemy_board(i, j)){
                block.setTextureRect(IntRect(p.get_enemy_board(i,j)*18, 0, 18, 18)) ;
                block.setPosition((j+15)*18, i*18) ;
                block.move(28, 31) ;
                scr.draw(block) ;
            }
        }
    }
}

void ColoringBlockMulti(RenderWindow & scr, Sprite & block) {
    for(size_t i=0 ; i<4 ; i++){
        Point Cur_pos = (p.get_Cur_Block()).get_Cur_pos(i);
        block.setTextureRect(IntRect(p.get_Cur_Block().get_TYPE()*18, 0, 18, 18)) ;
        block.setPosition(p.get_Cur_Block().get_Cur_pos(i).x*18, (p.get_Cur_Block().get_Cur_pos(i).y)*18) ;
        block.move(28, 31) ;
        scr.draw(block) ;       
    }

    for(size_t i=0 ; i<4 ; i++){
        Point Cur_pos = (p.get_Cur_Enemy_Block()).get_Cur_pos(i);
        block.setTextureRect(IntRect(p.get_Cur_Enemy_Block().get_TYPE()*18, 0, 18, 18)) ;
        block.setPosition((p.get_Cur_Enemy_Block().get_Cur_pos(i).x+15)*18, (p.get_Cur_Enemy_Block().get_Cur_pos(i).y)*18) ;
        block.move(28, 31) ;
        scr.draw(block) ;          
    }
}

void ColoringNextBlock(RenderWindow & scr, Sprite & block) {
    for(size_t j = 0; j<5; j++) {
        Block b(p.get_next_block(j));

        for(size_t i=0 ; i<4 ; i++){
            Point Cur_pos = b.get_Cur_pos(i);
            block.setTextureRect(IntRect(b.get_TYPE()*18, 0, 15, 15)) ;
            block.setPosition((b.get_Cur_pos(i).x+8)*18, b.get_Cur_pos(i).y*18+18*(j*3)) ;
            block.move(28, 40) ;
            scr.draw(block) ;      
        }    
    }
}

void ColoringHoldBlock(RenderWindow & scr, Sprite & block) {
    if(p.get_hold() != -1) {
        Block b(p.get_hold());

        for(size_t i=0 ; i<4 ; i++){
            Point Cur_pos = b.get_Cur_pos(i);
            block.setTextureRect(IntRect(b.get_TYPE()*18, 0, 15, 15)) ;
            block.setPosition((b.get_Cur_pos(i).x)*18, b.get_Cur_pos(i).y*18-18) ;
            block.move(28, 40) ;
            scr.draw(block) ;      
        }
    }
    if(p.get_enemy_hold() != -1) {
        Block b(p.get_enemy_hold());

        for(size_t i=0 ; i<4 ; i++){
            Point Cur_pos = b.get_Cur_pos(i);
            block.setTextureRect(IntRect(b.get_TYPE()*18, 0, 15, 15)) ;
            block.setPosition((b.get_Cur_pos(i).x+20)*18, b.get_Cur_pos(i).y*18-18) ;
            block.move(28, 40) ;
            scr.draw(block) ;      
        }    
    }
}

void ColoringScoreBoardMulti(RenderWindow & scr, Sprite & zero, Sprite & one, Sprite & two, Sprite & three, Sprite & four, Sprite & five, Sprite & six, Sprite & seven, Sprite & eight, Sprite & nine,player & p){

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
        print_number(scr, zero, one, two, three, four, five, six, seven, eight, nine, score_num.at(i), i, digit, 28, 28) ;   
    }
   
}

/*


void print_number(RenderWindow & scr, Sprite & zero, Sprite & one, Sprite & two, Sprite & three, Sprite & four, Sprite & five, Sprite & six, Sprite & seven, Sprite & eight, Sprite & nine, int k, int i, int digit){
        if(k== 0){
            zero.setTextureRect(IntRect(0,0,24,36)) ; zero.setPosition(24*(digit-i), 0) ; zero.move(28, 28) ; scr.draw(zero) ;
        }else if(k == 1){
            one.setTextureRect(IntRect(0,0,24,36)) ; one.setPosition(24*(digit-i),0) ; one.move(28, 28) ; scr.draw(one) ;
        }else if(k == 2){
            two.setTextureRect(IntRect(0,0,24,36)) ; two.setPosition(24*(digit-i),0) ; two.move(28, 28) ; scr.draw(two) ;
        }else if(k == 3){
            three.setTextureRect(IntRect(0,0,24,36)) ; three.setPosition(24*(digit-i),0) ; three.move(28, 28) ; scr.draw(three) ;
        }else if(k == 4){
            four.setTextureRect(IntRect(0,0,24,36)) ; four.setPosition(24*(digit-i),0) ; four.move(28, 28) ; scr.draw(four) ;
        }else if(k == 5){
            five.setTextureRect(IntRect(0,0,24,36)) ; five.setPosition(24*(digit-i),0) ; five.move(28, 28) ; scr.draw(five) ;
        }else if(k == 6){
            six.setTextureRect(IntRect(0,0,24,36)) ; six.setPosition(24*(digit-i),0) ; six.move(28, 28) ; scr.draw(six) ;
        }else if(k == 7){
            seven.setTextureRect(IntRect(0,0,24,36)) ; seven.setPosition(24*(digit-i),0) ; seven.move(28, 28) ; scr.draw(seven) ;
        }else if(k == 8){
            eight.setTextureRect(IntRect(0,0,24,36)) ; eight.setPosition(24*(digit-i),0) ; eight.move(28, 28) ; scr.draw(eight) ;
        }else if(k == 9){
            nine.setTextureRect(IntRect(0,0,24,36)) ; nine.setPosition(24*(digit-i),0) ; nine.move(28, 28) ; scr.draw(nine) ;     
        }
}

*/

