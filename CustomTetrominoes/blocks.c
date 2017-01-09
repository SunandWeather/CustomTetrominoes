//
//  blocks.c
//  Tetris
//
//  Created by Johan Sunnanväder on 2017-01-04.
//  Copyright (c) 2017 SunandWeather. All rights reserved.
//

#include <stdio.h>
#include "stdbool.h"

#include "blocks.h"

blocks get_block(int x, int y, int orientation, int move, int type)
{
    blocks block;
    
    if (type == 1) {            //I-block
    
        if (orientation == 1 || orientation == 3) {
            
            block.x1 = x - 1;
            block.y1 = y;
            block.x2 = x;
            block.y2 = y;
            block.x3 = x + 1;
            block.y3 = y;
            block.x4 = x + 2;
            block.y4 = y;
            
        } else if (orientation == 2 || orientation == 4) {
            
            block.x1 = x;
            block.y1 = y + 1;
            block.x2 = x;
            block.y2 = y;
            block.x3 = x;
            block.y3 = y - 1;
            block.x4 = x;
            block.y4 = y - 2;
            
        }
        
    } else if (type == 2) {     //L-block
        
        if (orientation == 1) {
            
            block.x1 = x - 1;
            block.y1 = y + 1;
            block.x2 = x - 1;
            block.y2 = y;
            block.x3 = x;
            block.y3 = y;
            block.x4 = x + 1;
            block.y4 = y;
            
        } else if (orientation == 2) {
            
            block.x1 = x - 1;
            block.y1 = y - 1;
            block.x2 = x;
            block.y2 = y - 1;
            block.x3 = x;
            block.y3 = y;
            block.x4 = x;
            block.y4 = y + 1;
            
        } else if (orientation == 3) {
            
            block.x1 = x + 1;
            block.y1 = y - 1;
            block.x2 = x + 1;
            block.y2 = y;
            block.x3 = x;
            block.y3 = y;
            block.x4 = x - 1;
            block.y4 = y;
            
        } else if (orientation == 4) {
            
            block.x1 = x + 1;
            block.y1 = y + 1;
            block.x2 = x;
            block.y2 = y + 1;
            block.x3 = x;
            block.y3 = y;
            block.x4 = x;
            block.y4 = y - 1;
            
        }
        
    } else if (type == 3) {     //O-block
        
        if (orientation == 1 || orientation == 2 || orientation == 3 || orientation == 4) {
            
            block.x1 = x;
            block.y1 = y;
            block.x2 = x + 1;
            block.y2 = y;
            block.x3 = x + 1;
            block.y3 = y + 1;
            block.x4 = x;
            block.y4 = y + 1;
            
        }
        
    } else if (type == 4) {     //J-block
        
        if (orientation == 1) {
            
            block.x1 = x - 1;
            block.y1 = y;
            block.x2 = x;
            block.y2 = y;
            block.x3 = x + 1;
            block.y3 = y;
            block.x4 = x + 1;
            block.y4 = y + 1;
            
        } else if (orientation == 2) {
            
            block.x1 = x;
            block.y1 = y - 1;
            block.x2 = x;
            block.y2 = y;
            block.x3 = x;
            block.y3 = y + 1;
            block.x4 = x - 1;
            block.y4 = y + 1;
            
        } else if (orientation == 3) {
            
            block.x1 = x + 1;
            block.y1 = y;
            block.x2 = x;
            block.y2 = y;
            block.x3 = x - 1;
            block.y3 = y;
            block.x4 = x - 1;
            block.y4 = y - 1;
            
        } else if (orientation == 4) {
            
            block.x1 = x;
            block.y1 = y + 1;
            block.x2 = x;
            block.y2 = y;
            block.x3 = x;
            block.y3 = y - 1;
            block.x4 = x + 1;
            block.y4 = y - 1;
            
        }
        
    } else if (type == 5) {     //Z-block
        
        if (orientation == 1 || orientation == 3) {
            
            block.x1 = x - 1;
            block.y1 = y;
            block.x2 = x;
            block.y2 = y;
            block.x3 = x;
            block.y3 = y + 1;
            block.x4 = x + 1;
            block.y4 = y + 1;
            
        } else if (orientation == 2 || orientation == 4) {
            
            block.x1 = x;
            block.y1 = y - 1;
            block.x2 = x;
            block.y2 = y;
            block.x3 = x - 1;
            block.y3 = y;
            block.x4 = x - 1;
            block.y4 = y + 1;
            
        }
        
    } else if (type == 6) {     //S-block
        
        if (orientation == 1 || orientation == 3) {
            
            block.x1 = x - 1;
            block.y1 = y + 1;
            block.x2 = x;
            block.y2 = y + 1;
            block.x3 = x;
            block.y3 = y;
            block.x4 = x + 1;
            block.y4 = y;
            
        } else if (orientation == 2 || orientation == 4) {
            
            block.x1 = x;
            block.y1 = y + 1;
            block.x2 = x;
            block.y2 = y;
            block.x3 = x - 1;
            block.y3 = y;
            block.x4 = x - 1;
            block.y4 = y - 1;
            
        }
        
    } else if (type == 7) {     //T-block
        
        if (orientation == 1) {
            
            block.x1 = x - 1;
            block.y1 = y;
            block.x2 = x;
            block.y2 = y;
            block.x3 = x + 1;
            block.y3 = y;
            block.x4 = x;
            block.y4 = y + 1;
            
        } else if (orientation == 2) {
            
            block.x1 = x;
            block.y1 = y - 1;
            block.x2 = x;
            block.y2 = y;
            block.x3 = x;
            block.y3 = y + 1;
            block.x4 = x - 1;
            block.y4 = y;
            
        } else if (orientation == 3) {
            
            block.x1 = x + 1;
            block.y1 = y;
            block.x2 = x;
            block.y2 = y;
            block.x3 = x - 1;
            block.y3 = y;
            block.x4 = x;
            block.y4 = y - 1;
            
        } else if (orientation == 4) {
            
            block.x1 = x;
            block.y1 = y + 1;
            block.x2 = x;
            block.y2 = y;
            block.x3 = x;
            block.y3 = y - 1;
            block.x4 = x + 1;
            block.y4 = y;
            
        }
        
    }
    
    if (move == 1) {
        
        block.x1 = block.x1 - 1;
        block.x2 = block.x2 - 1;
        block.x3 = block.x3 - 1;
        block.x4 = block.x4 - 1;
        
    } else if (move == 2) {
        
        block.x1 = block.x1 + 1;
        block.x2 = block.x2 + 1;
        block.x3 = block.x3 + 1;
        block.x4 = block.x4 + 1;
        
    }
    
    return block;
}