#ifndef __PLAYER_H__
#define __PLAYER_H__
#include <algorithm> 
#include <vector>
#include <iostream>
#include "block.h"
#define ROW	20
#define COL	10	

class player {
	private:
		int score;
		int combo;
        int max_com;
        int sum_tetris;
        int clear;
		bool tetris;
        bool win;
        int move;
        bool rotate;
        int hold;
        bool hold_use;
		int board[ROW][COL];
        int enemy_score;
        int enemy_hold;
        int enemy_board[ROW][COL];
        std::vector <int> next_block;
        Block Cur_Block ;
        Block Cur_Enemy_Block;

	public:
        player() : score(0), combo(0), max_com(0), sum_tetris(0), clear(0), tetris(false), win(false), move(0), rotate(false), hold(-1), hold_use(false) {
            std::fill(&board[0][0], &board[ROW-1][COL], 0);
            std::fill(&board[0][0], &board[ROW-1][COL], 0);
            generate_next_block();
            generate_new_Block();
            Cur_Enemy_Block = Block(-1);
        }

        void operator=(player rhs) {
            score = rhs.score; combo = rhs.combo; tetris = rhs.tetris;
            move = rhs.move; rotate = rhs.rotate;
            Cur_Block = rhs.Cur_Block;        
            Cur_Enemy_Block = rhs.Cur_Enemy_Block;
            std::fill(&board[0][0], &board[ROW-1][COL], 0);
            std::fill(&board[0][0], &board[ROW-1][COL], 0);
        }
		
	public:
		int get_score();
		int get_combo();
		bool get_tetris();
        int get_move();
        bool get_rotate();
		int get_board(int y, int x);
        int get_enemy_hold();
        int get_enemy_score();
        int get_enemy_board(int y, int x);
        Block get_Cur_Block() ;
        Block get_Cur_Enemy_Block();
        int * get_board_address();
        int get_next_block(int n);
        int get_hold();
        bool get_hold_use();
        bool get_win();

		void set_score(int point);
		void set_combo(bool success);
		void set_tetris(bool success);
		void set_board(int y, int x, int value);
        void set_enemy_hold(int n);
        void set_enemy_score(int n);
        void set_enemy_board(int y, int x, int value);
        void set_cur_pos(int y, int x, int cur);
        void set_move(int move);
        void set_rotate(bool rotate);
        void set_Enemy_Block(Block& block);
        void set_hold(int n);
        void set_hold_use(bool use);
        void set_win(bool win);

        void check_move(int move);
        bool check_move_down();
        void move_down() ;
        void check_rotate() ;
        void space_block(float & delay) ;
        void down_block(float & delay) ;
        void fix_cur_block();
        void generate_new_Block();
        char line_clear();
        void generate_next_block();
        void hold_action();
        void attacked(int n);
        void makeline(int n);
        void print_result();
};

 
#endif
