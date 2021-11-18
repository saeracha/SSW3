#ifndef __PLAYER_H__
#define __PLAYER_H__
#include <algorithm> 
#include "block.h"
#define ROW	20
#define COL	10	

/*class Point{
    public:
        int x, y;
};*/

//class Block ;

class player {
	private:
		int score;
		int combo;
		bool tetris;
        int move;
        bool rotate;
		int board[ROW][COL];
        int enemy_board[ROW][COL];
        Block Cur_Block ;
        Block Next_Block ;
        Block Cur_Enemy_Block;

	public:
        player() : score(0), combo(0), tetris(false), move(0), rotate(false) {
            std::fill(&board[0][0], &board[ROW-1][COL], 0);
            std::fill(&board[0][0], &board[ROW-1][COL], 0);
            generate_new_Block();
            Cur_Enemy_Block = Block();
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
        int get_enemy_board(int y, int x);
        Block get_Cur_Block() ;
        Block get_Cur_Enemy_Block();
        int * get_board_address();

		void set_score(int point);
		void set_combo(bool success);
		void set_tetris(bool success);
		void set_board(int y, int x, int value);
        void set_enemy_board(int y, int x, int value);
        void set_cur_pos(int y, int x, int cur);
        void set_move(int move);
        void set_rotate(bool rotate);
        void set_Enemy_Block(Block& block);

        void check_move(int move);
        bool check_move_down();
        void move_down() ;
        void check_rotate() ;
        void space_block(float & delay) ;
        void fix_cur_block();
        void generate_new_Block();
        void line_clear();
};

 
#endif
