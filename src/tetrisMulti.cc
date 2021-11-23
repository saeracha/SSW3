#include "tetrisSingle.h"
#include "tetrisMulti.h"
#include <time.h>
#include <iostream>
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
static int pass = 0;
player p;
static pthread_mutex_t mutex;

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

    pthread_mutex_init(&mutex, NULL);
	pthread_create(&send_thread, NULL, Tetris, (void*)multiarg);
	pthread_create(&read_thread, NULL, ReadEnemy, (void*)multiarg);
    
    pthread_join(send_thread, &thread_return);
	pthread_join(read_thread, &thread_return);

    close(sock);
    delete multiarg;
    exit(0);
}

void * Tetris(void * arg){
    MultiArg * multiarg = (MultiArg*)arg;
    int sock = multiarg->sock;
    RenderWindow * scr = multiarg->scr;
    char att = 'a';

    //send and 
    char * temp = new char[BUF_SIZE];
    Convert(temp);
    temp[BUF_SIZE-1] = 'x';
    int len = write(sock, temp, BUF_SIZE);

	while((*scr).isOpen()){
		float time = game_clock.getElapsedTime().asSeconds() ;
		game_clock.restart() ;
		timer += time ;
 
        Event e ;  
        while((*scr).pollEvent(e)){
            if (e.type == Event::Closed) {
                Lose(*scr, sock);
            }
            
            if(e.type == Event::KeyPressed){
                if(e.key.code == Keyboard::Up) p.set_rotate(true) ;
                else if(e.key.code == Keyboard::Left) p.set_move(-1);
                else if(e.key.code == Keyboard::Right) p.set_move(1); 
                else if(e.key.code == Keyboard::Space) p.space_block(delay) ;
                else if(e.key.code == Keyboard::LShift) {
                    if(!p.get_hold_use()) {
                        p.hold_action();
                        p.set_hold_use(true);
                    }
                }
            }
        }
    
        if(pass>0) {
            if(pass == 1) {
                char * temp = new char[BUF_SIZE];
                Convert(temp);
                temp[BUF_SIZE-1] = 'w';
                int len = write(sock, temp, BUF_SIZE);
            }
            pass = -1;
            init(p, *scr);
        }

        if(p.get_win()) {
            Win(*scr, sock);
        }

        if(Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.03 ;

        if(p.get_move()) p.check_move(p.get_move()) ;
        if(p.get_rotate()) p.check_rotate() ;
    
        if(timer>delay){
            if(p.check_move_down()) p.move_down() ;
            else {
                p.fix_cur_block(); 
                if(cnt < 89 ) cnt++ ; 
                p.set_hold_use(false);

                pthread_mutex_lock(&mutex);
                p.generate_new_Block();
                att = p.line_clear();
                pthread_mutex_unlock(&mutex);

                for(int i = 0; i<4; i++) {
                    int y = p.get_Cur_Block().get_Cur_pos(i).y;
                    int x = p.get_Cur_Block().get_Cur_pos(i).x;

                    if(p.get_board(y, x)) {
                        Lose(*scr, sock);
                    }
                }
            }
            timer = 0;
        } 
        p.set_move(0);
        p.set_rotate(false);
        delay = delays[cnt/10];
       
        char * temp = new char[BUF_SIZE];
        Convert(temp);
        temp[BUF_SIZE-1] = att;
        int len = write(sock, temp, BUF_SIZE);
        att = 'a';

        visual_Multi(*scr, p) ;
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

        if(data[BUF_SIZE-1] >= 'b' && data[BUF_SIZE-1] <= 'o') {
            pthread_mutex_lock(&mutex);
            p.attacked(data[BUF_SIZE-1]-'a');
            pthread_mutex_unlock(&mutex);
        }

        if(data[BUF_SIZE-1] == 'z') {
            p.set_win(true);
            pthread_exit(NULL);
        }
        else if(data[BUF_SIZE-1] == 'x') {
            pass = 1;
        }
        else if(data[BUF_SIZE-1] == 'w') {
            pass = 2;
        }
        draw(data, *scr);
    }
    
    return NULL;
}


void draw(char * data, RenderWindow & scr) {
    if(pass >= 0) return;
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
    for(int i = 0; i<6; i++) {
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

void Win(RenderWindow& scr, int sock) {
    std::cout << "VICTORY\n\n";
    p.print_result();
    scr.close();
    char * temp = new char[BUF_SIZE];
    Convert(temp);
    temp[BUF_SIZE-1] = 'z';
    int len = write(sock, temp, BUF_SIZE);
    pthread_exit(NULL);
}

void Lose(RenderWindow& scr, int sock) {
    char * temp = new char[BUF_SIZE];
    Convert(temp);
    temp[BUF_SIZE-1] = 'z';
    int len = write(sock, temp, BUF_SIZE);
    std::cout << "DEFEAT\n\n";     
    p.print_result();
    scr.close();
    pthread_exit(NULL);
}

void visual_Multi(RenderWindow &  scr, player & p){
    Texture t1, t2, t3, t4 ;
    Texture tt0, tt1, tt2, tt3, tt4, tt5, tt6, tt7, tt8, tt9, ttb ;
	t1.loadFromFile("./img/tiles.png") ;
	t2.loadFromFile("./img/background.png") ;
	t3.loadFromFile("./img/frame.png") ;
    t4.loadFromFile("./img/frame_enemy.png") ;
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

	Sprite block(t1), background(t2), frame(t3), frame_enemy(t4) ;
    Sprite one(tt1), two(tt2), three(tt3), four(tt4), five(tt5), six(tt6), seven(tt7), eight(tt8), nine(tt9), zero(tt0), blank(ttb);   
    scr.clear(Color::White) ;
    scr.draw(background) ;
    ColoringBoard_Multi(scr, block, p);
    ColoringBlock_Multi(scr, block, p); 
    ColoringNextBlock(scr, block, p);
    ColoringHoldBlock(scr, block, p); 
    ColoringScoreBoard(scr, blank, zero, one, two, three, four, five, six, seven, eight, nine, p) ;
    ColoringEnemyScoreBoard(scr, zero, one, two, three, four, five, six, seven, eight, nine, p) ;


    scr.draw(frame) ;
    frame_enemy.setTextureRect(IntRect(0, 0, 212, 470)) ;
    frame_enemy.setPosition(0, 0) ;
    frame_enemy.move(334, 0) ;
    scr.draw(frame_enemy) ;
}

void init(player &_p, RenderWindow & scr) {
    Texture s, r; 
    s.loadFromFile("./img/start.png");
    r.loadFromFile("./img/ready.png");
    
    Sprite start(s), ready(r);
    _p = player();
    
    scr.draw(ready);
    scr.display();
    sleep(1);
    scr.draw(start);
    scr.display();
    sleep(1);
}






