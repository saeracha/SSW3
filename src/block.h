#ifndef __BLOCK_H__
#define __BLOCK_H__
#include <algorithm>

#define ROW	20
#define COL	10	


class Point{
    public:
        int x, y;
};

class Block {
    private:
        int TYPE ;
        Point Cur_pos[4] ; 
    public:

        Block() ;
        void operator=(Block right){
            TYPE = right.get_TYPE() ;
            for(int i=0 ; i<4 ; i++){
                Cur_pos[i] = right.get_Cur_pos(i) ;
            }

        }
        Point get_Cur_pos(int cur);
        int get_TYPE() ;
        void MOVE(int move); // left, right

        void set_TYPE(int type);
        void set_Cur_pos(int cur, int y, int x);

        void MOVE(); // down
        void ROTATE() ;
        bool Out_of_Range() ;
        bool Preoccupied_down(int board[ROW][COL]) ;
        bool Preoccupied(int board[ROW][COL]) ;
        int Out_Right() ;
        int Out_Left() ;
        bool Out_Bottom() ;

        
} ;

#endif
