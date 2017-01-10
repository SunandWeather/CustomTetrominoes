//
//  game.h
//  Custom Tetrominoes
//
//  Created by Johan Sunnanväder on 2017-01-03.
//  Copyright (c) 2017 SunandWeather. All rights reserved.
//

#ifndef Tetris_game_h
#define Tetris_game_h

#include "sprite.h"
#include "blocks.h"
#include <SDL2/SDL.h>

//Define structure for required information to run the game;
typedef struct {
    
    //SDL
    SDL_Window* window;
    SDL_Renderer* renderer;
    
    //Screen dimentions
    int HIGHT_IN_BLOCKS;
    int WIDTH_IN_BLOCKS;
    int BLOCK_WIDTH;
    int BLOCK_HEIGHT;
    int SCREEN_HEIGHT;
    int SCREEN_WIDTH;
    
    //Sprites
    sprite* l;
    sprite* L;
    sprite* O;
    sprite* J;
    sprite* Z;
    sprite* S;
    sprite* T;
    sprite* wall;
    sprite* ground;
    sprite* game_over;
    
} gameData;

//The main loop of the game
int game_Loop(gameData* data);

//Increases the value of the orientation tracker
int increase_orientation(int orientation);

//Increases the value of the orientation tracker
int decrease_orientation(int orientation);

//Render block tiles
void render_blocks (int x, int y, gameData data, int type);

//Update the score
int get_score(int score, int drop, int level, int rowCounter);

//Check if block includes x and y
bool block_include(blocks block, int x, int y);

#endif
