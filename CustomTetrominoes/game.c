//
//  game.c
//  Custom Tetrominoes
//
//  Created by Johan Sunnanväder on 2017-01-04.
//  Copyright (c) 2017 SunandWeather. All rights reserved.
//

#include <stdio.h>

#include "stdbool.h"
#include "time.h"

#include "Game.h"
#include "sprite.h"
#include "font.h"
#include "Highscore.h"
#include <SDL2_image/SDL_image.h>

#define N 1000000000
int grid[N];
int tempGrid[N];
int temptempGrid[N];

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

    
    /* Wait for start command */
    
    //Event handler
    SDL_Event event;
    
    //Main loop flag
    bool start = false;
    
    while (!start) {
        
        //Handle events on queue
        while( SDL_PollEvent( &event ) != 0 )
        {
            //User requests quit
            if( event.type == SDL_QUIT) {
                return 1;
            }
            
            if(((event.key.keysym.sym == SDLK_RETURN) && event.type == SDL_KEYDOWN)
               || (event.key.keysym.sym == SDLK_SPACE && event.type == SDL_KEYUP))
            {
                start = true;
            } else if (event.key.keysym.sym == SDLK_ESCAPE && event.type == SDL_KEYDOWN) {
                return 1;
            }
        }
        
        int w;
        int h;
        SDL_GetWindowSize(data->window, &w, &h);
        int xOffset = (w - data->WIDTH_IN_BLOCKS * data->BLOCK_WIDTH) / 2 - 1 * data->BLOCK_WIDTH;
        int yOffset = (h - data->HEIGHT_IN_BLOCKS * data->BLOCK_HEIGHT) / 2;
        SDL_RenderClear(data->renderer);
        render_sprite(data->start, (data->WIDTH_IN_BLOCKS / 2 - 1) * data->BLOCK_WIDTH + xOffset,
                      (data->HEIGHT_IN_BLOCKS / 2 - 1) * data->BLOCK_HEIGHT + yOffset, data->renderer);
		SDL_RenderPresent(data->renderer);
        
        //Update the surface
        SDL_UpdateWindowSurface(data->window);
        
        SDL_Delay(20);
        
    }
    
    //Initialize starting point of the blocks
    int STARTING_POINT_X = (data->WIDTH_IN_BLOCKS + 1) / 2 + 1;
    int STARTING_POINT_Y = 2;
    
    //Initialize matrix dimensions
    int wMatrix = (data->WIDTH_IN_BLOCKS + 4);
    int hMatrix = (data->HEIGHT_IN_BLOCKS + 2);
    
    //Setting grid values to 0 (free space), walls to 8 and ground to 9
    int i, j, k;
    for (j = 0; j < data->HEIGHT_IN_BLOCKS + 2; j++)
    {
        for (i = 0; i < data->WIDTH_IN_BLOCKS + 4; i++)
        {
            if(i < 2 || i > (data->WIDTH_IN_BLOCKS + 1)){
               grid[i * hMatrix + j] = 8;
                //printf("%d\t",grid[i * hMatrix + j]);
            } else {
               grid[i * hMatrix + j] = 0;
                //printf("%d\t",grid[i * hMatrix + j]);
            }
            
            if (j == data->HEIGHT_IN_BLOCKS + 1){
               grid[i * hMatrix + j] = 9;
                //printf("%d\t",grid[i * hMatrix + j]);
            }
            
        }
    }
    
    for (j = 0; j < data->HEIGHT_IN_BLOCKS + 2; j++)
    {

        for (i = 0; i < data->WIDTH_IN_BLOCKS + 4; i++)
        {
            tempGrid[i * hMatrix + j] = 0;
            temptempGrid[i * hMatrix + j] = 0;
        }
    }
    
    //Hiding mouse pointer
    SDL_ShowCursor(SDL_DISABLE);
    
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
    
    int action;                             //Store the actions given at the high score input
    
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
        
        if (score > 999999) {
            score = 999999;
        }
        
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
        tempGrid[block.x1 * hMatrix + block.y1] = type;
        tempGrid[block.x2 * hMatrix + block.y2] = type;
        tempGrid[block.x3 * hMatrix + block.y3] = type;
        tempGrid[block.x4 * hMatrix + block.y4] = type;
        
        //////////////////////////////////////////////////////
        /* RENDERING BLOCKS AND COMPARING grid AND tempGrid */
        //////////////////////////////////////////////////////
        
        SDL_RenderClear(data->renderer);
        
        for (j = 1; j < data->HEIGHT_IN_BLOCKS + 2; j++)
        {
            for (i = 1; i < data->WIDTH_IN_BLOCKS + 3; i++)
            {
                if(grid[i * hMatrix + j] > 0)
                    render_blocks (i, j, *data,grid[i * hMatrix + j]);
                if (tempGrid[i * hMatrix + j] > 0)
                    render_blocks (i, j, *data, tempGrid[i * hMatrix + j]);
                if(grid[i * hMatrix + j] > 0 && tempGrid[i * hMatrix + j] > 0) {
                    
                    ///////////////
                    /* GAME OVER */
                    ///////////////
                    
                    //Show mouse pointer
                    SDL_ShowCursor(SDL_ENABLE);
                    
                    //Render tempGrid and grid
                    SDL_RenderClear(data->renderer);
                    
                    for (j = 1; j < data->HEIGHT_IN_BLOCKS + 2; j++)
                    {
                        for (i = 1; i < data->WIDTH_IN_BLOCKS + 3; i++)
                        {
                            if(grid[i * hMatrix + j] > 0)
                                render_blocks (i, j, *data,grid[i * hMatrix + j]);
                            if (tempGrid[i * hMatrix + j] > 0)
                                render_blocks (i, j, *data, tempGrid[i * hMatrix + j]);
                        }
                    }
                     
                    //Update the surface
                    SDL_RenderPresent(data->renderer);
                    SDL_UpdateWindowSurface(data->window);
                    
                    SDL_Delay(speed);
                    
                    //Empty key queue
                    while (SDL_PollEvent(&event)) { }
                    
                    //////////////////////
                    /* GAME OVER SCREEN */
                    //////////////////////
                    
                    //Hide game window
                    SDL_HideWindow(data->window);
                    
                    if (!data->high_score_window_exist) {
                        data->high_score_window_exist = create_high_score_window(data);
                    } else {
                        SDL_ShowWindow(data->menu_window);
                    }
                    
                    //Assigning title
                    char title[256];
                    sprintf(title, "Score: %d Level: %d Lines: %d", score, level, lines);
                    SDL_SetWindowTitle(data->menu_window, title);
                    
                    action = render_highscore_input(score, lines, level, data);
                    
                    if (action == 10) {
                        //Quit
                        return 1;
                    } else if (action == 18) {
                        //Restart
                        SDL_HideWindow(data->menu_window);
                        SDL_ShowWindow(data->window);
                        return 2;
                    }
                    
                    //Show high score
                    int mode;
                    mode = 10000 * data->WIDTH_IN_BLOCKS + data->HEIGHT_IN_BLOCKS;
                    
                    action = show_high_score(data->menu_renderer, mode, *data, 2);
                    
                    if (action == 0 || action == 1) {
                        //Quit
                        return 1;
                    } else if (action == 2) {
                        //Restart
                        SDL_HideWindow(data->menu_window);
                        SDL_ShowWindow(data->window);
                        return 2;
                    }
                    
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
                        SDL_Delay(30);
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
                        
                    } else if (e.key.keysym.sym == SDLK_ESCAPE && e.type == SDL_KEYDOWN) {
                        return 3;
                    } else if (e.key.keysym.sym == SDLK_p && e.type == SDL_KEYDOWN) {
                        bool paused = true;
                        while (paused) {
                            while( SDL_PollEvent( &e ) != 0) {
                                if (e.type == SDL_QUIT) {
                                    return 1;
                                }
                                if (e.key.keysym.sym == SDLK_p && e.type == SDL_KEYDOWN) {
                                    paused = false;
                                } else if (e.key.keysym.sym == SDLK_r && e.type == SDL_KEYDOWN) {
                                    return 2;
                                } else if (e.key.keysym.sym == SDLK_ESCAPE && e.type == SDL_KEYDOWN) {
                                    return 1;
                                }
                            }
                            //Render empty game
                            SDL_RenderClear(data->renderer);
                            
                            for (j = 1; j < data->HEIGHT_IN_BLOCKS + 2; j++)
                            {
                                for (i = 1; i < data->WIDTH_IN_BLOCKS + 3; i++)
                                {
                                    if(grid[i * hMatrix + j] == 8 || grid[i * hMatrix + j] == 9)
                                        render_blocks (i, j, *data,grid[i * hMatrix + j]);
                                }
                            }
                            
                            //Update the surface
                            SDL_RenderPresent(data->renderer);
                            SDL_UpdateWindowSurface(data->window);
                            SDL_Delay(30);
                        }
                        
                        //Render occupied grid
                        SDL_RenderClear(data->renderer);
                        
                        for (j = 1; j < data->HEIGHT_IN_BLOCKS + 2; j++)
                        {
                            for (i = 1; i < data->WIDTH_IN_BLOCKS + 3; i++)
                            {
                                if(grid[i * hMatrix + j] > 0)
                                    render_blocks (i, j, *data,grid[i * hMatrix + j]);
                                if (tempGrid[i * hMatrix + j] > 0)
                                    render_blocks (i, j, *data, tempGrid[i * hMatrix + j]);
                            }
                        }
                        
                        //Update the surface
                        SDL_RenderPresent(data->renderer);
                        SDL_UpdateWindowSurface(data->window);
                        
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
                        temptempGrid[block.x1 * hMatrix + block.y1] = type;
                        temptempGrid[block.x2 * hMatrix + block.y2] = type;
                        temptempGrid[block.x3 * hMatrix + block.y3] = type;
                        temptempGrid[block.x4 * hMatrix + block.y4] = type;
                        
                        //Antifreeze
                        printf("");
                        
                        //Checking if the move is valid
                        for (j = current_y - 4; j < current_y + 4; j++)
                        {
                            for (i = current_x - 4; i < current_x + 4; i++)
                            {
                                if(grid[i * hMatrix + j] > 0 && temptempGrid[i * hMatrix + j] > 0) {
                                    
                                    //Invalid move
                                    valid = false;
                                    
                                    move = 0;
                                    
                                    //Antifreeze
                                    printf("");
                                    
                                    temptempGrid[block.x1 * hMatrix + block.y1] = 0;
                                    temptempGrid[block.x2 * hMatrix + block.y2] = 0;
                                    temptempGrid[block.x3 * hMatrix + block.y3] = 0;
                                    temptempGrid[block.x4 * hMatrix + block.y4] = 0;
                                    
                                    //exiting loops
                                    i = data->HEIGHT_IN_BLOCKS + 1;
                                    j = data->WIDTH_IN_BLOCKS + 2;
                                    
                                    //Exiting mooving block loop and add a new block if block
                                    //lands on stationary object or ground
                                    if (down) {
                                        
                                        //Double check to avoid glitches
                                        block = get_block(current_x, current_y, orientation, move, type);
                                        down = false;
                                        
                                        if (grid[block.x1 * hMatrix + block.y1] > 0)
                                            if (!block_include(block, block.x1, block.y1 + 1))
                                                down = true;
                                        if (grid[block.x2 * hMatrix + block.y2] > 0)
                                            if (!block_include(block, block.x2, block.y2) + 1)
                                                down = true;
                                        if (grid[block.x3 * hMatrix + block.y3] > 0)
                                            if (!block_include(block, block.x3, block.y3) + 1)
                                                down = true;
                                        if (grid[block.x4 * hMatrix + block.y4] > 0)
                                            if (!block_include(block, block.x3, block.y4) + 1)
                                                down = true;
                                        
                                        if (down) {
                                            moving = false;
                                            
                                            //Merging tempGrid with grid
                                            for (j = current_y - 4; j < current_y + 4; j++)
                                            {
                                                for (i = current_x - 4; i < current_x + 4; i++)
                                                {
                                                    if (tempGrid[i * hMatrix + j] > 0) {
                                                       grid[i * hMatrix + j] = tempGrid[i * hMatrix + j];
                                                        tempGrid[i * hMatrix + j] = 0;
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
                                    i = data->HEIGHT_IN_BLOCKS + 1;
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
                            for (j = current_y - 4; j < current_y + 4; j++)
                            {
                                for (i = current_x - 4; i < current_x + 4; i++)
                                {
                                    tempGrid[i * hMatrix + j] = 0;                  
                                }
                            }
                            
                            //Adding moved block to tempGrid
                            tempGrid[block.x1 * hMatrix + block.y1] = type;
                            tempGrid[block.x2 * hMatrix + block.y2] = type;
                            tempGrid[block.x3 * hMatrix + block.y3] = type;
                            tempGrid[block.x4 * hMatrix + block.y4] = type;
                            
                            //Resetting temptempGrid
                            temptempGrid[block.x1 * hMatrix + block.y1] = 0;
                            temptempGrid[block.x2 * hMatrix + block.y2] = 0;
                            temptempGrid[block.x3 * hMatrix + block.y3] = 0;
                            temptempGrid[block.x4 * hMatrix + block.y4] = 0;
                            
                            //Render tempGrid and grid
                            SDL_RenderClear(data->renderer);
                            
                            for (j = 1; j < data->HEIGHT_IN_BLOCKS + 2; j++)
                            {
                                for (i = 1; i < data->WIDTH_IN_BLOCKS + 3; i++)
                                {
                                    if(grid[i * hMatrix + j] > 0)
                                        render_blocks (i, j, *data,grid[i * hMatrix + j]);
                                    if (tempGrid[i * hMatrix + j] > 0)
                                        render_blocks (i, j, *data, tempGrid[i * hMatrix + j]);
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
        
        
        for (j = 0; j < data->HEIGHT_IN_BLOCKS; j++) {
            i = 0;
            
            while (rowOccupied) {
                if (grid[(i + 2) * hMatrix + (j + 1)] == 0) {
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
                    grid[i * hMatrix + (rows[k] - j)] = grid[i * hMatrix + ((rows[k] - j) - 1)];
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
    
    //Center the game
    int xOffset = 0;
    int yOffset = 0;
    int w;
    int h;
    
    SDL_GetWindowSize(data.window, &w, &h);
    
    xOffset = (w - data.WIDTH_IN_BLOCKS * data.BLOCK_WIDTH) / 2 - 1 * data.BLOCK_WIDTH;
    yOffset = (h - data.HEIGHT_IN_BLOCKS * data.BLOCK_HEIGHT) / 2;
    
    //Render
    if (type == 1)
        render_sprite(data.I, (x - 1) * data.BLOCK_WIDTH + xOffset, (y - 1) * data.BLOCK_HEIGHT + yOffset, data.renderer);
    else if (type == 2)
        render_sprite(data.L, (x - 1) * data.BLOCK_WIDTH + xOffset, (y - 1) * data.BLOCK_HEIGHT + yOffset, data.renderer);
    else if (type == 3)
        render_sprite(data.O, (x - 1) * data.BLOCK_WIDTH + xOffset, (y - 1) * data.BLOCK_HEIGHT + yOffset, data.renderer);
    else if (type == 4)
        render_sprite(data.J, (x - 1) * data.BLOCK_WIDTH + xOffset, (y - 1) * data.BLOCK_HEIGHT + yOffset, data.renderer);
    else if (type == 5)
        render_sprite(data.Z, (x - 1) * data.BLOCK_WIDTH + xOffset, (y - 1) * data.BLOCK_HEIGHT + yOffset, data.renderer);
    else if (type == 6)
        render_sprite(data.S, (x - 1) * data.BLOCK_WIDTH + xOffset, (y - 1) * data.BLOCK_HEIGHT + yOffset, data.renderer);
    else if (type == 7)
        render_sprite(data.T, (x - 1) * data.BLOCK_WIDTH + xOffset, (y - 1) * data.BLOCK_HEIGHT + yOffset, data.renderer);
    else if (type == 8)
        render_sprite(data.wall, (x - 1) * data.BLOCK_WIDTH + xOffset, (y - 1) * data.BLOCK_HEIGHT + yOffset, data.renderer);
    else if (type == 9)
        render_sprite(data.ground, (x - 1) * data.BLOCK_WIDTH + xOffset, (y - 1) * data.BLOCK_HEIGHT + yOffset, data.renderer);
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

bool create_high_score_window(gameData *data) {
    //Create window
    data->menu_window = SDL_CreateWindow( "", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, data->MENU_SCREEN_WIDTH, data->MENU_SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if(data->menu_window == NULL)
    {
        printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
        SDL_DestroyWindow(data->menu_window);
        SDL_Quit();
        return false;
    }
    
    //Create renderer for window
    data->menu_renderer = SDL_CreateRenderer(data->menu_window, -1, SDL_RENDERER_ACCELERATED );
    if(data->menu_renderer == NULL)
    {
        printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
        SDL_DestroyWindow(data->menu_window);
        SDL_Quit();
        return false;
    }
    
    //Reinitializing sprites
//    char font_path[256] = "/Users/SunandWeather/Documents/Xcode/CustomTetrominoes/CustomTetrominoes/res/font.png";
//    char fader_path[256] = "/Users/SunandWeather/Documents/Xcode/CustomTetrominoes/CustomTetrominoes/res/fader.png";
    
    char font_path[256] = "res/font.png";
    char fader_path[256] = "res/fader.png";
    
    //Initializing font for
    for (int i = 0; i < data->FONT_CHARACTERS; i++) {
        SDL_Surface* surface = malloc(sizeof(surface));
        surface = IMG_Load(font_path);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(data->menu_renderer, surface);
        SDL_FreeSurface(surface);
        data->menu_font[i] = init_sprite(texture, i * data->FONT_WIDTH, 0, data->FONT_WIDTH, data->FONT_HEIGHT);
    }
    
    //Reinitializing fader
    SDL_Surface* surface = malloc(sizeof(surface));
    surface = IMG_Load(fader_path);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(data->menu_renderer, surface);
    SDL_FreeSurface(surface);
    data->menu_fader = init_sprite(texture, 0, 0, data->MENU_SCREEN_WIDTH, data->MENU_SCREEN_HEIGHT);
    SDL_SetTextureAlphaMod(data->menu_fader->texture, 150);
    
    
    return true;
}



int render_highscore_input(int score, int lines, int level, gameData* data) {
    
    char scoreString[256];
    char levelString[256];
    char linesString[256];
    
    char characters[40] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 .-";
    
    char name[4] = {'A'};
    int pos = 0;
    int counter = 0;
    
    sprintf(scoreString, "SCORE: %d", score);
    sprintf(levelString, "LEVEL: %d", level);
    sprintf(linesString, "LINES: %d", lines);
    
    while (pos != 9) {
        
        SDL_RenderClear(data->menu_renderer);
    
        render_font(&name[0], 303, 300, data->menu_renderer, data->FONT_WIDTH, data->FONT_HEIGHT, data->menu_font);
        render_font(&name[1], 314, 300, data->menu_renderer, data->FONT_WIDTH, data->FONT_HEIGHT, data->menu_font);
        render_font(&name[2], 325, 300, data->menu_renderer, data->FONT_WIDTH, data->FONT_HEIGHT, data->menu_font);
        
        render_sprite(data->menu_fader, 0, 0, data->menu_renderer);
        
        render_font("--GAME OVER--", 248, 50, data->menu_renderer, data->FONT_WIDTH, data->FONT_HEIGHT, data->menu_font);
        render_font(scoreString, 75, 100, data->menu_renderer, data->FONT_WIDTH, data->FONT_HEIGHT, data->menu_font);
        render_font(levelString, 75, 140, data->menu_renderer, data->FONT_WIDTH, data->FONT_HEIGHT, data->menu_font);
        render_font(linesString, 75, 180, data->menu_renderer, data->FONT_WIDTH, data->FONT_HEIGHT, data->menu_font);
        
        render_font("--SUBMIT SCORE--", 234, 230, data->menu_renderer, data->FONT_WIDTH, data->FONT_HEIGHT, data->menu_font);
        render_font("PRESS 'R' TO RETRY, OR 'ESCAPE' TO QUIT", 105, 380, data->menu_renderer, data->FONT_WIDTH, data->FONT_HEIGHT, data->menu_font);
        
        //Render active character
        render_font("\a", 303 + pos * data->FONT_WIDTH, 300 - 25, data->menu_renderer, data->FONT_WIDTH, data->FONT_HEIGHT, data->menu_font);
        render_font(&name[pos], 303 + pos * data->FONT_WIDTH, 300, data->menu_renderer, data->FONT_WIDTH, data->FONT_HEIGHT, data->menu_font);
        render_font("\v", 303 + pos * data->FONT_WIDTH, 300 + 25, data->menu_renderer, data->FONT_WIDTH, data->FONT_HEIGHT, data->menu_font);
        
        //Update the surface
        SDL_RenderPresent(data->menu_renderer);
        SDL_UpdateWindowSurface(data->menu_window);
    
        pos = enter_name(name, pos, &counter, characters);
        
        if (pos == 18) {
            return 18;
        } else if (pos == 10) {
            return 10;
        }
        
    }
    
    //Submitting high score
    int game_mode = data->WIDTH_IN_BLOCKS * 10000 + data->HEIGHT_IN_BLOCKS;
    enter_highscore(score, name, game_mode);
    
    return 1;
    
}

int enter_name(char name[], int pos, int *counter, char characters[]) {
    
    int count = 0;
    
    //Event handler
    SDL_Event event;
    
    //Main loop flag
    bool exit = false;
    
    while (!exit) {
        
        //Handle events on queue
        while( SDL_PollEvent( &event ) != 0 )
        {
            //User requests quit
            if( event.type == SDL_QUIT)
            {
                return 10;
            }
            
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                exit = true;
                return 10;
            } else if ((event.key.keysym.sym == SDLK_r) && event.type == SDL_KEYDOWN) {
                return 18;
            } else if ((event.key.keysym.sym == SDLK_DOWN) && event.type == SDL_KEYDOWN) {
                *counter = *counter + 1;
                if (*counter == (int) strlen(characters)) {
                    *counter = 0;
                }
                name[pos] = characters[*counter];
                return pos;
                
            } else if ((event.key.keysym.sym == SDLK_UP) && event.type == SDL_KEYDOWN) {
                if (*counter == 0) {
                    *counter = (int) strlen(characters) - 1;
                } else {
                   *counter = *counter - 1;
                }
                name[pos] = characters[*counter];
                return pos;
            } else if ((event.key.keysym.sym == SDLK_LEFT) && event.type == SDL_KEYDOWN) {
                name[pos] = '\0';
                if (pos > 0)
                    pos = pos - 1;
                return pos;
            } else if ((event.key.keysym.sym == SDLK_RIGHT) && event.type == SDL_KEYDOWN) {
                if (pos < 2)
                    pos++;
                name[pos] = characters[*counter];
                return pos;
            } else if (((event.key.keysym.sym == SDLK_RETURN) && event.type == SDL_KEYDOWN)
                       || (event.key.keysym.sym == SDLK_SPACE && event.type == SDL_KEYDOWN)) {
                if (pos == 2) {
                    exit = true;
                    break;
                } else {
                    pos++;
                    name[pos] = characters[*counter];
                    return pos;
                }
            }
            
        }
        
        //Delay
        SDL_Delay(50);
        if (count == 10) {
            return pos;
        } else {
            count++;
        }
        
    }
    return 9;
}

int show_high_score(SDL_Renderer* r, int mode, gameData data, int menu) {
    
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {}
    
	bool running = true;
    
	char list[256];
    for (int i = 0; i < 256; i++) {
		list[i] = 0;
	}
    
	get_highscore(list, mode);
	int entries = 0;
    
	// Count the number of entries in the list
	for (int i = 0; i < 256; i++) {
		if (list[i] == ';')
			entries++;
	}
    
	while (running) {
        
		while (SDL_PollEvent(&e) != 0) {
            //User requests quit
			if (e.type == SDL_QUIT) {
				return 0;
			} else if (e.key.keysym.sym == SDLK_ESCAPE && e.type == SDL_KEYDOWN) {
                return 1;
            } else if ((e.key.keysym.sym == SDLK_RETURN && e.type == SDL_KEYDOWN)
                        || (e.key.keysym.sym == SDLK_r && e.type == SDL_KEYDOWN)) {
                return 2;
            }
        }
        
		SDL_RenderClear(r);
        
        char high_score_title[256];
        
        
        sprintf(high_score_title, "HIGH SCORE - %04d X %04d", data.WIDTH_IN_BLOCKS, data.HEIGHT_IN_BLOCKS);
        
        if (menu == 1) {
            render_font(high_score_title, 193, 50, r, data.FONT_WIDTH, data.FONT_HEIGHT, data.font);
        } else {
            render_font(high_score_title, 193, 50, r, data.FONT_WIDTH, data.FONT_HEIGHT, data.menu_font);
        }
        
		char temp[16] = { 0 };
        char position[12];
        int xadd = 0;
		for (int i = 0; i < 10; i++) {
			if (i < entries) {
				memcpy(temp, &(list[i * 12 + i]), 12);
                sprintf(position, "%d.", i + 1);
                if (i == 9)
                    xadd = 1;
                if (menu == 1) {
                    render_font(position, 200 - (11 * xadd), 115 + i * 30, r, data.FONT_WIDTH, data.FONT_HEIGHT, data.font);
                    render_font(temp, 237, 115 + i * 30, r, data.FONT_WIDTH, data.FONT_HEIGHT, data.font);
                } else {
                    render_font(position, 200 - (11 * xadd), 115 + i * 30, r, data.FONT_WIDTH, data.FONT_HEIGHT, data.menu_font);
                    render_font(temp, 237, 115 + i * 30, r, data.FONT_WIDTH, data.FONT_HEIGHT, data.menu_font);
                }
                xadd = 0;
			} else {
                if (menu == 1) {
                    render_font("-", 314, 115 + i * 30, r, data.FONT_WIDTH, data.FONT_HEIGHT, data.font);
                } else {
                    render_font("-", 314, 115 + i * 30, r, data.FONT_WIDTH, data.FONT_HEIGHT, data.menu_font);
                }
			}
		}
        
		SDL_RenderPresent(r);
        
        //Delay
        SDL_Delay(20);
        
	}
    
    return 1;
}










