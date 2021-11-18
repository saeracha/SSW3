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

    Texture t1, t2, t3 ;
	t1.loadFromFile("./img/tiles.png") ;
	t2.loadFromFile("./img/background.png") ;
	t3.loadFromFile("./img/frame.png") ;
	Sprite block(t1), background(t2), frame(t3) ;  
    
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

        (*scr).clear(Color::White) ;
        (*scr).draw(background) ;

        ColoringBoardMulti(*scr, block);
        ColoringBlockMulti(*scr, block);

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