//
//  game.c
//  Tetris
//
//  Created by Johan Sunnanväder on 2017-01-04.
//  Copyright (c) 2017 SunandWeather. All rights reserved.
//

#include <stdio.h>

#include "stdbool.h"
#include "time.h"

#include "Game.h"
#include "sprite.h"


int game_loop(gameData* data)
{
    /* Game grid value chart
     0 = Empty space
     1 = I-block
     2 = L-block
     3 = O-block
     4 = J-block
     5 = Z-block
     6 = S-block
     7 = T-block
     8 = Walls
     9 = ground
     */
    
    //Initialize starting point of the blocks
    int STARTING_POINT_X = (data->WIDTH_IN_BLOCKS + 1) / 2 + 1;
    int STARTING_POINT_Y = 2;
    
    //Setup main grid (2 blocks higher, 4 blocks wider)
    int grid[data->WIDTH_IN_BLOCKS + 4][data->HIGHT_IN_BLOCKS + 2];
    
    //Setting grid values to 0 (free space), walls to 8 and ground to 9
    int i, j, k;
    for (j = 0; j < data->HIGHT_IN_BLOCKS + 2; j++)
    {
        for (i = 0; i < data->WIDTH_IN_BLOCKS + 4; i++)
        {
            if(i < 2 || i > (data->WIDTH_IN_BLOCKS + 1)){
                grid[i][j] = 8;
                //printf("%d\t", grid[i][j]);
            } else if (j == data->HIGHT_IN_BLOCKS + 1){
                grid[i][j] = 9;
                //printf("%d\t", grid[i][j]);
            } else {
                grid[i][j] = 0;
                //printf("%d\t", grid[i][j]);
            }
        }
    }
    
    //Setup 2 temporary grids
    int tempGrid[data->WIDTH_IN_BLOCKS + 4][data->HIGHT_IN_BLOCKS + 2];
    int temptempGrid[data->WIDTH_IN_BLOCKS + 4][data->HIGHT_IN_BLOCKS + 2];
    for (j = 0; j < data->HIGHT_IN_BLOCKS + 2; j++)
    {

        for (i = 0; i < data->WIDTH_IN_BLOCKS + 4; i++)
        {
            tempGrid[i][j] = 0;
            temptempGrid[i][j] = 0;
        }
    }
    
    
    /* INITIALIZING BLOCK INFORMATION */
    
    srand(time(NULL));                      //Used to randomize blocks
    
    char title[64];                         //Title array
    
    bool reset;                             //Reset flag
    
    bool game = true;                       //Flag for wether the game is on
    
    int current_x = STARTING_POINT_X;       //Tracker for the center tile of the blocks
    int current_y = STARTING_POINT_Y;
    
    int orientation = 1;                    //Tracker of orientation of the center block
    
    int type;                           //Tracker of block type for current block
    
    int types = 7;                          //Number of different blocks
    
    int move = 0;                           //Move LEFT (1) or RIGHT (2)
    
    blocks block;                           //Block data
    
    bool down;                              //Flag for when the block should be moved down
    
    bool moving = true;                     //Flag for moving block
    
    int speed = 887;                       //Speed of loop (in ms)
    
    int n = speed;                            //Loop n times before exiting loop
                                            //(n times to check input before block decends)
    
    SDL_Event e;                            //Event handler
    
    bool valid;                             //Valid move flag
    
    bool rowOccupied;                       //Tracks if a row is occupied when checking if a row
                                            //should be removed
    
    int tileCounter;                        //Counting occupied tiles when rows may be removed
    
    int rows[4];                            //Tracking rows to be removed
    
    int rowListTracker = 0;                 //Tracking position in rows
    
    int score = 1;                          //Keeps track of the score
    
    int level = 0;                          //Keeps Track of level
    
    int lines = 0;                          //Keeps track of lines
    
    int drop = 0;                           //Tracks the number of steps a block is dropped
                                            //Hitting something
    
    int speeds[21] = {887, 870, 753, 686,   //Speed per level
        619, 552, 469, 368, 285, 184, 167,
        151, 134, 117, 100, 100, 84, 84,
        67, 67, 50};
    
    double speedCorrection = 0.87;          //Compensating for processing time
    
    //Main game loop
    while (game)
    {
        
        /* UPDATING SCORE */
        
        score = get_score(score, drop, level, rowListTracker);
        
        if (level < 20) {
            level = lines / 10;
        }
        
        speed = speeds[level] * speedCorrection;
        n = speed;
        
        sprintf(title, "Score: %d Level: %d Lines: %d", score, level, lines);
        SDL_SetWindowTitle(data->window, title);
        
        
        /* RESETTING DATA */
        
        reset = false;
        moving = true;
        int loopCounter = 0;
        orientation = 1;
        current_x = STARTING_POINT_X;
        current_y = STARTING_POINT_Y;
        loopCounter = 0;
        moving = true;
        down = false;
        rowOccupied = true;
        tileCounter = 0;
        rowListTracker = 0;
        drop = 0;
        
        ////////////////////////////////
        /* ADDING BLOCKS TO TEMP GRID */
        ////////////////////////////////
        
        //Adding random delay to avoid same block every game start
        SDL_Delay(rand() % 7);
        
        //Randomizing block
        type = (rand() % types) + 1;
        
        //Getting block information for specific block
        block = get_block(current_x, current_y, orientation, move, type);
        
        //Adding blocks to tempGrid
        tempGrid[block.x1][block.y1] = type;
        tempGrid[block.x2][block.y2] = type;
        tempGrid[block.x3][block.y3] = type;
        tempGrid[block.x4][block.y4] = type;
        
        //////////////////////////////////////////////////////
        /* RENDERING BLOCKS AND COMPARING grid AND tempGrid */
        //////////////////////////////////////////////////////
        
        SDL_RenderClear(data->renderer);
        
        for (j = 1; j < data->HIGHT_IN_BLOCKS + 1; j++)
        {
            for (i = 1; i < data->WIDTH_IN_BLOCKS + 3; i++)
            {
                if (grid[i][j] > 0)
                    render_blocks (i, j, *data, grid[i][j]);
                if (tempGrid[i][j] > 0)
                    render_blocks (i, j, *data, tempGrid[i][j]);
                if (grid[i][j] > 0 && tempGrid[i][j] > 0) {
                    
                    ///////////////
                    /* GAME OVER */
                    ///////////////
                    
                    //Render tempGrid and grid
                    SDL_RenderClear(data->renderer);
                    
                    for (j = 1; j < data->HIGHT_IN_BLOCKS + 1; j++)
                    {
                        for (i = 1; i < data->WIDTH_IN_BLOCKS + 3; i++)
                        {
                            if (grid[i][j] > 0)
                                render_blocks (i, j, *data, grid[i][j]);
                            if (tempGrid[i][j] > 0)
                                render_blocks (i, j, *data, tempGrid[i][j]);
                        }
                    }
                     
                    //Update the surface
                    SDL_RenderPresent(data->renderer);
                    SDL_UpdateWindowSurface(data->window);
                    
                    SDL_Delay(speed);
                    
                    //////////////////////
                    /* GAME OVER SCREEN */
                    //////////////////////
                    
                    SDL_RenderClear(data->renderer);
                    
                    render_sprite(data->game_over, (data->WIDTH_IN_BLOCKS / 2 - 1) * data->BLOCK_HEIGHT,
                                  (data->HIGHT_IN_BLOCKS / 2 - 1) * data->BLOCK_HEIGHT, data->renderer);
                    
                    SDL_RenderPresent(data->renderer);
                    SDL_UpdateWindowSurface(data->window);
                    
                    //Waiting for restart or quit
                    while(true)
                    {
                        //Handle events on queue
                        while( SDL_PollEvent( &e ) != 0 )
                        {
                            //User requests quit
                            if( e.type == SDL_QUIT)
                            {
                                return 1;
                            } if (e.key.keysym.sym == SDLK_r && e.type == SDL_KEYUP) {
                                return 2;
                            }
                        }
                    }
                }
            }
        }
        
        //Update the surface
        SDL_RenderPresent(data->renderer);
        SDL_UpdateWindowSurface(data->window);
        
        /////////////////////////////////////////////////
        /* REGISTER ROTATION, MOVEMENT OR ACCELERATION */
        /////////////////////////////////////////////////
        
            while(moving) {
                    
                //Delay the loop by one game tick while checking for key action n times per tick
                SDL_Delay(speed / n);
                
                //Reset move flag
                valid = true;
                
                //Resting down flag
                down = false;
                
                //Handle events on queue
                while( SDL_PollEvent( &e ) != 0)
                {
                    if( e.type == SDL_QUIT) {
                        //Quit Game
                        return 1;
                    } else if (e.key.keysym.sym == SDLK_r && e.type == SDL_KEYUP) {
                        return 2;
                    } else if ((e.key.keysym.sym == SDLK_LEFT && e.type == SDL_KEYDOWN)
                             || ((e.key.keysym.sym == SDLK_RIGHT) && e.type == SDL_KEYDOWN)
                             || ((e.key.keysym.sym == SDLK_z) && e.type == SDL_KEYDOWN)
                             || ((e.key.keysym.sym == SDLK_x) && e.type == SDL_KEYDOWN)
                             || ((e.key.keysym.sym == SDLK_SPACE) && e.type == SDL_KEYDOWN)
                             || ((e.key.keysym.sym == SDLK_UP) && e.type == SDL_KEYDOWN)
                             || ((e.key.keysym.sym == SDLK_DOWN) && e.type == SDL_KEYDOWN)
                             || ((e.key.keysym.sym == SDLK_DOWN) && e.type == SDL_KEYUP)){
                        
                        if (e.key.keysym.sym == SDLK_LEFT) {
                            move = 1;
                        } else if (e.key.keysym.sym == SDLK_RIGHT){
                            move = 2;
                        } else if (e.key.keysym.sym == SDLK_z){
                            orientation = decrease_orientation(orientation);
                        } else if (e.key.keysym.sym == SDLK_x || e.key.keysym.sym == SDLK_SPACE
                                   || e.key.keysym.sym == SDLK_UP){
                            orientation = increase_orientation(orientation);
                        } else if (e.key.keysym.sym == SDLK_DOWN && e.type == SDL_KEYDOWN) {
                            loopCounter = 0;
                            down = true;
                            current_y = current_y + 1;
                            drop++;
                        } else if (e.key.keysym.sym == SDLK_DOWN && e.type == SDL_KEYDOWN) {
                            drop = 0;
                        }
                        
                        //Get coordinates for moved block
                        blocks block;
                        block = get_block(current_x, current_y, orientation, move, type);
                        
                        //Resetting move flag
                        move = 0;
                        
                        //Add moved block to temptempGrid
                        temptempGrid[block.x1][block.y1] = type;
                        temptempGrid[block.x2][block.y2] = type;
                        temptempGrid[block.x3][block.y3] = type;
                        temptempGrid[block.x4][block.y4] = type;
                        
                        //Antifreeze
                        printf("");
                        
                        //Checking if the move is valid
                        for (j = 0; j < data->HIGHT_IN_BLOCKS + 2; j++)
                        {
                            for (i = 0; i < data->WIDTH_IN_BLOCKS + 4; i++)
                            {
                                if (grid[i][j] > 0 && temptempGrid[i][j] > 0) {
                                    
                                    
                                    //Invalid move
                                    valid = false;
                                    
                                    move = 0;
                                    
                                    //Antifreeze
                                    printf("");
                                    
                                    temptempGrid[block.x1][block.y1] = 0;
                                    temptempGrid[block.x2][block.y2] = 0;
                                    temptempGrid[block.x3][block.y3] = 0;
                                    temptempGrid[block.x4][block.y4] = 0;
                                    
                                    //exiting loops
                                    i = data->HIGHT_IN_BLOCKS + 1;
                                    j = data->WIDTH_IN_BLOCKS + 2;
                                    
                                    //Exiting mooving block loop and add a new block if block
                                    //lands on stationary object or ground
                                    if (down) {
                                        
                                        //Double check to avoid glitches
                                        block = get_block(current_x, current_y, orientation, move, type);
                                        down = false;
                                        
                                        if (grid[block.x1][block.y1] > 0)
                                            if (!block_include(block, block.x1, block.y1 + 1))
                                                down = true;
                                        if (grid[block.x2][block.y2] > 0)
                                            if (!block_include(block, block.x2, block.y2) + 1)
                                                down = true;
                                        if (grid[block.x3][block.y3] > 0)
                                            if (!block_include(block, block.x3, block.y3) + 1)
                                                down = true;
                                        if (grid[block.x4][block.y4] > 0)
                                            if (!block_include(block, block.x3, block.y4) + 1)
                                                down = true;
                                        
                                        if (down) {
                                            moving = false;
                                            
                                            //Merging tempGrid with grid
                                            for (j = 0; j < data->HIGHT_IN_BLOCKS + 2; j++)
                                            {
                                                for (i = 0; i < data->WIDTH_IN_BLOCKS + 4; i++)
                                                {
                                                    if (tempGrid[i][j] > 0) {
                                                        grid[i][j] = tempGrid[i][j];
                                                        tempGrid[i][j] = 0;
                                                    }
                                                }
                                            }
                                        } 
                                    }
                                    
                                    //Resetting orientation
                                    if (e.key.keysym.sym == SDLK_z){
                                        orientation = increase_orientation(orientation);
                                    } else if (e.key.keysym.sym == SDLK_x || e.key.keysym.sym == SDLK_SPACE || e.key.keysym.sym == SDLK_UP){
                                        orientation = decrease_orientation(orientation);
                                    }
                                    
                                    //exiting loops
                                    i = data->HIGHT_IN_BLOCKS + 1;
                                    j = data->WIDTH_IN_BLOCKS + 2;
                                    
                                }
                            }
                        }
                        
                        if (valid) {
                            //Updating current x position
                            if (e.key.keysym.sym == SDLK_LEFT) {
                                current_x = current_x - 1;
                            } else if (e.key.keysym.sym == SDLK_RIGHT) {
                                current_x = current_x + 1;
                            }
                            
                            
                            /* Copy temptempGrid to tempGrid and reset temptempGrid */
                            
                            //Resetting tempGrid to add moved block
                            for (i = 0; i < data->HIGHT_IN_BLOCKS + 2; i++)
                            {
                                for (j = 0; j < data->WIDTH_IN_BLOCKS + 4; j++)
                                {
                                    tempGrid[j][i] = 0;
                                }
                            }
                            
                            //Adding moved block to tempGrid
                            tempGrid[block.x1][block.y1] = type;
                            tempGrid[block.x2][block.y2] = type;
                            tempGrid[block.x3][block.y3] = type;
                            tempGrid[block.x4][block.y4] = type;
                            
                            //Resetting temptempGrid
                            temptempGrid[block.x1][block.y1] = 0;
                            temptempGrid[block.x2][block.y2] = 0;
                            temptempGrid[block.x3][block.y3] = 0;
                            temptempGrid[block.x4][block.y4] = 0;
                            
                            //Render tempGrid and grid
                            SDL_RenderClear(data->renderer);
                            
                            for (j = 1; j < data->HIGHT_IN_BLOCKS + 1; j++)
                            {
                                for (i = 1; i < data->WIDTH_IN_BLOCKS + 3; i++)
                                {
                                    if (grid[i][j] > 0)
                                        render_blocks (i, j, *data, grid[i][j]);
                                    if (tempGrid[i][j] > 0)
                                        render_blocks (i, j, *data, tempGrid[i][j]);
                                }
                            }
                            
                            //Update the surface
                            SDL_RenderPresent(data->renderer);
                            SDL_UpdateWindowSurface(data->window);
                        }
                    }
                }
                
                loopCounter = loopCounter + 1;
                
                //Check if it is time for the block to go down one step
                if (loopCounter >= n) {
                    
                    //Simulating a DOWN push
                    SDL_Event sdlevent;
                    sdlevent.type = SDL_KEYDOWN;
                    sdlevent.key.keysym.sym = SDLK_DOWN;
                    SDL_PushEvent(&sdlevent);
                    drop = 0;
                    
                }
            }
        
        //////////////////////////////////
        /* CHECK FOR ROWS TO BE REMOVED */
        //////////////////////////////////
        
        
        for (j = 0; j < data->HIGHT_IN_BLOCKS; j++) {
            i = 0;
            
            while (rowOccupied) {
                if (grid[i + 2][j + 1] == 0) {
                    rowOccupied = false;
                break;
                }
                
                if (i == data->WIDTH_IN_BLOCKS - 1) {
                    
                    //Row should be removed
                    rows[rowListTracker] = j + 1;
                    rowListTracker++;
                    lines++;
                    
                }
                i++;
            }
            rowOccupied = true;
        }
        
        ///////////////////
        /* REMOVING ROWS */
        ///////////////////
        
        for (k = 0; k < rowListTracker; k++) {
            for (j = 0; j < rows[k]; j++)
            {
                for (i = 2; i < data->WIDTH_IN_BLOCKS + 2; i++)
                {
                    grid[i][(rows[k] - j)] = grid[i][(rows[k] - j) - 1];
                }
            }
        }
    }
    return 1;
}

int increase_orientation(int orientation) {
    int newOrientation;
    
    if (orientation == 4) {
        newOrientation = 1;
    } else {
        newOrientation = orientation + 1;
    }
    
    return newOrientation;
}

int decrease_orientation(int orientation) {
    int newOrientation;
    
    if (orientation == 1) {
        newOrientation = 4;
    } else {
        newOrientation = orientation - 1;
    }
    
    return newOrientation;
}

void render_blocks (int x, int y, gameData data, int type) {
    if (type == 1)
        render_sprite(data.l, (x - 1) * data.BLOCK_WIDTH, (y - 1) * data.BLOCK_HEIGHT, data.renderer);
    if (type == 2)
        render_sprite(data.L, (x - 1) * data.BLOCK_WIDTH, (y - 1) * data.BLOCK_HEIGHT, data.renderer);
    if (type == 3)
        render_sprite(data.O, (x - 1) * data.BLOCK_WIDTH, (y - 1) * data.BLOCK_HEIGHT, data.renderer);
    if (type == 4)
        render_sprite(data.J, (x - 1) * data.BLOCK_WIDTH, (y - 1) * data.BLOCK_HEIGHT, data.renderer);
    if (type == 5)
        render_sprite(data.Z, (x - 1) * data.BLOCK_WIDTH, (y - 1) * data.BLOCK_HEIGHT, data.renderer);
    if (type == 6)
        render_sprite(data.S, (x - 1) * data.BLOCK_WIDTH, (y - 1) * data.BLOCK_HEIGHT, data.renderer);
    if (type == 7)
        render_sprite(data.T, (x - 1) * data.BLOCK_WIDTH, (y - 1) * data.BLOCK_HEIGHT, data.renderer);
    if (type == 8)
        render_sprite(data.wall, (x - 1) * data.BLOCK_WIDTH, (y - 1) * data.BLOCK_HEIGHT, data.renderer);
}

int get_score(int score, int drop, int level, int rowListTracker) {
    int newScore;
    newScore = score + drop - 1;
    
    if (rowListTracker == 1) {
        newScore = newScore + 40 * (level + 1);
    } else if (rowListTracker == 2) {
        newScore = newScore + 100 * (level + 1);
    } else if (rowListTracker == 3) {
        newScore = newScore + 300 * (level + 1);
    } else if (rowListTracker == 4) {
        newScore = newScore + 1200 * (level + 1);
    }
    
    return newScore;
}

bool block_include(blocks block, int x, int y) {
    bool include = false;
    
    if (block.x1 == x && block.y1 == y) {
        include = true;
    } else if (block.x2 == x && block.y2 == y) {
        include = true;
    } else if (block.x3 == x && block.y3 == y) {
        include = true;
    } else if (block.x4 == x && block.y4 == y) {
        include = true;
    }
    
    return include;
}
