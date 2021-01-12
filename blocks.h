//
//  blocks.h
//  Custom Tetrominoes
//
//  Created by Johan Sunnanväder on 2017-01-04.
//  Copyright (c) 2017 SunandWeather. All rights reserved.
//

#include "stdbool.h"

#ifndef Tetris_blocks_h
#define Tetris_blocks_h

typedef struct {
    
    //Initial oriantation
    int x1;
    int y1;
    int x2;
    int y2;
    int x3;
    int y3;
    int x4;
    int y4;
    
} blocks;

//Returns a block structure with the coordinates in which blocks should be placed.
//x and y: is coordinates for the center block
//orientation: is the orientation of the block
//move indicates if the blockk is moved: 0: do not move
//                                       1: move left
//                                       2: move right

blocks get_block(int x, int y, int orientation, int move, int type);

#endif
