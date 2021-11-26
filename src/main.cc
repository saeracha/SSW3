#include <SFML/Graphics.hpp>
#include "player.h"
#include "block.h"
#include "tetrisSingle.h"
#include "tetrisMulti.h"
//
#include <iostream>
//
#include <cstring>
#include <cstdlib>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

using namespace sf;

#define SINGLE  0
#define MULTI   1
int main(){
    
    int sock;
    struct sockaddr_in serv_addr;
    bool out = false;
    int cur = SINGLE;

    Texture select_single, select_multi;
    select_single.loadFromFile("./img/choosesingle.png");
    select_multi.loadFromFile("./img/choosemulti.png");
    Sprite choose_single(select_single);
    Sprite choose_multi(select_multi);

    RenderWindow scr(VideoMode(480, 600), "choose");
    scr.draw(choose_single);
    scr.display();

    while(scr.isOpen()) {
        Event e;
        while(scr.pollEvent(e)) {
            //click close
            if(e.type == Event::Closed) {
                scr.close();
                return 0;
            }

            if(e.type == Event::KeyPressed) {
                if(e.key.code == Keyboard::Down) cur++;
                else if(e.key.code == Keyboard::Up) cur--;
                else if(e.key.code == Keyboard::RShift) {
                    out = true;
                    scr.close();
                }       
            }
        }
        if(out)
            break;

        if(cur>1) cur-=2;
        else if(cur<0) cur+=2;

        scr.clear(Color::White);
        if(cur == SINGLE) scr.draw(choose_single);
        else if(cur == MULTI) scr.draw(choose_multi);
        scr.display();
    }
    
    if(cur == MULTI) {
	    sock=socket(PF_INET, SOCK_STREAM, 0);
	    memset(&serv_addr, 0, sizeof(serv_addr));
	    serv_addr.sin_family=AF_INET;
	    serv_addr.sin_addr.s_addr=inet_addr(WELL_KNOWN_IP);
	    serv_addr.sin_port=htons(WELL_KNOWN_PORT);

	    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1) {
            std::cout << "CONNECT FAILED!!\n You must open server to play multimode!\n";
	    	exit(1);//server is not open;
        }
        TetrisBattle(sock);    
    }

    //single mode
    else if (cur == SINGLE) {
        player p;
        Tetris(p) ;
        std::cout << "GAME OVER\n\n";
        p.print_result();
    }
    return 0 ;
}
