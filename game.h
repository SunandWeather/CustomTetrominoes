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
#include "stdbool.h"
#include <SDL2/SDL.h>

//Define structure for required information to run the game;
typedef struct {
    
    /* Game data */
    
    //SDL
    SDL_Window* window;
    SDL_Renderer* renderer;
    
    //Screen dimentions
    int HEIGHT_IN_BLOCKS;
    int WIDTH_IN_BLOCKS;
    int BLOCK_WIDTH;
    int BLOCK_HEIGHT;
    int SCREEN_HEIGHT;
    int SCREEN_WIDTH;
    int FONT_WIDTH;
    int FONT_HEIGHT;
    int MENU_SCREEN_WIDTH;
    int MENU_SCREEN_HEIGHT;
    
    int FONT_CHARACTERS;
    
    //Sprites
    sprite* I;
    sprite* L;
    sprite* O;
    sprite* J;
    sprite* Z;
    sprite* S;
    sprite* T;
    sprite* title;
    sprite* start;
    sprite* wall;
    sprite* ground;
    sprite* game_over;
    sprite* fader;
    
    //Font
    sprite* font[60];
    
    /* Menu */
    
    //Menu
    SDL_Window* menu_window;
    SDL_Renderer* menu_renderer;
    bool high_score_window_exist;
    
    //Sprites
    sprite* menu_fader;
    
    //Font
    sprite* menu_font[60];
    
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

//Render high score input
int render_highscore_input(int score, int lines, int level, gameData* data);

//Function for entering name for high score
int enter_name(char name[], int pos, int *counter, char characters[]);

//Creates new window for high score input
bool create_high_score_window(gameData *data);

//Displays the high score
int show_high_score(SDL_Renderer *renderer, int mode, gameData data, int menu);


#endif
