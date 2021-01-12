//  main.c
//  Custom Tetrominoes
//
//  Created by Johan Sunnanväder on 2016-12-29.
//  Copyright (c) 2016 SunandWeather. All rights reserved.


#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

//Including SDL framework
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

//Include header files
#include "sprite.h"
#include "game.h"
#include "font.h"

#include "time.h"
#include "SDL2_rotozoom.h"


int HEIGHT_IN_BLOCKS;
int WIDTH_IN_BLOCKS;
int BLOCK_WIDTH;
int BLOCK_HEIGHT;
int SCREEN_HEIGHT;
int SCREEN_WIDTH;
int FONT_WIDTH;
int FONT_HEIGHT;
int FONT_CHARACTERS;
int TITLE_WIDTH;
int TITLE_HEIGHT;
int MENU_SCREEN_WIDTH;
int MENU_SCREEN_HEIGHT;

//int restart;

//Initialize main game function
int game_loop(gameData* data);

//Initialize file reader and resizer
sprite* create_sprite (int wDesired, int hDesired, SDL_Renderer* renderer, char path[256], int xpos, int ypos);
sprite* create_sprite_no_resize (int wDesired, int hDesired, SDL_Renderer* renderer, char path[256], int xpos, int ypos);

//Initialize function for tracking menu selection
int increase_selection(int selection);
int decrease_selection(int selection);

//Function for changing game dimensions
int change_dimensions(int* dimensions, int pos);
int change_block_width(int* blockWidth, int pos);

//Render title screen
void render_title(gameData data, int selection, int pos, bool active, int TITLE_WIDTH, int SCREEN_WIDTH, int dimensions[8], int blockWidth[4]);

//Loads media
SDL_Texture* loadMedia(char path, SDL_Rect rect, SDL_Renderer *renderer);

//The window we'll be rendering to
SDL_Window* window = NULL;

//The window renderer
SDL_Renderer* renderer = NULL;

//Current displayed texture
SDL_Texture* gTexture = NULL;

//Main function
int main(int argc, const char * argv[])
{
    HEIGHT_IN_BLOCKS = 18;
    WIDTH_IN_BLOCKS = 10;
    BLOCK_WIDTH = 1;

    BLOCK_HEIGHT = BLOCK_WIDTH;
    SCREEN_HEIGHT = HEIGHT_IN_BLOCKS * BLOCK_WIDTH;
    SCREEN_WIDTH = WIDTH_IN_BLOCKS * BLOCK_WIDTH + 2 * BLOCK_WIDTH;

    MENU_SCREEN_WIDTH = 640;
    MENU_SCREEN_HEIGHT = 480;

    FONT_WIDTH = 11;
    FONT_HEIGHT = 25;
    FONT_CHARACTERS = 60;

    TITLE_WIDTH = 560;
    TITLE_HEIGHT = 200;


    // Define size of a block for the image reader
    SDL_Rect blockRect;
    blockRect.x = 0;
    blockRect.y = 0;
    blockRect.w = BLOCK_WIDTH;
    blockRect.h = SCREEN_HEIGHT;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        return 1;
    }

    //Initialize PNG loading
    int imgFlags = IMG_INIT_PNG;
    if( !( IMG_Init( imgFlags ) & imgFlags ) )
    {
        printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
        SDL_Quit();
        return 1;
    }

    //Set texture filtering to linear
    if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
    {
        printf( "Warning: Linear texture filtering not enabled!" );
        SDL_Quit();
        return 1;
    }

    //Create window
    window = SDL_CreateWindow( "", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, MENU_SCREEN_WIDTH, MENU_SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if( window == NULL )
    {
        printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    //Create renderer for window
    renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
    if( renderer == NULL )
    {
        printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    //Initialize paths
    char l_path[256] = "res/I_block.png";
    char L_path[256] = "res/L_block.png";
    char O_path[256] = "res/O_block.png";
    char J_path[256] = "res/J_block.png";
    char Z_path[256] = "res/Z_block.png";
    char S_path[256] = "res/S_block.png";
    char T_path[256] = "res/T_block.png";
    char wall_path[256] = "res/wall.png";
    char ground_path[256] = "res/ground.png";
    char title_path[256] = "res/title.png";
    char game_over_path[256] = "res/game_over.png";
    char start_path[256] = "res/start.png";
    char fader_path[256] = "res/fader.png";
    char font_path[256] = "res/font.png";
    

    //Creating sprites
    sprite* wall_sprite = create_sprite (BLOCK_WIDTH, BLOCK_HEIGHT, renderer, wall_path, 0, 0);
    sprite* title_sprite = create_sprite_no_resize(TITLE_WIDTH, TITLE_HEIGHT, renderer, title_path, 0, 0);
    sprite* fader_sprite = create_sprite_no_resize(MENU_SCREEN_WIDTH, MENU_SCREEN_WIDTH, renderer, fader_path, 0, 0);
    SDL_SetTextureAlphaMod(fader_sprite->texture, 150 );


    //Font sprites
    sprite* font[FONT_CHARACTERS];
    for (int i = 0; i < FONT_CHARACTERS; i++) {
        font[i] = create_sprite_no_resize (FONT_WIDTH, FONT_HEIGHT, renderer, font_path, (i * FONT_WIDTH), 0);
    }

    // Compiling game data
    gameData data;

    //SDL
    data.window = window;
    data.renderer = renderer;

    //Screen dimensions
    data.HEIGHT_IN_BLOCKS = HEIGHT_IN_BLOCKS;
    data.WIDTH_IN_BLOCKS = WIDTH_IN_BLOCKS;
    data.BLOCK_WIDTH = BLOCK_WIDTH;
    data.BLOCK_HEIGHT = BLOCK_HEIGHT;
    data.SCREEN_HEIGHT = SCREEN_HEIGHT;
    data.SCREEN_WIDTH = SCREEN_WIDTH;
    data.FONT_WIDTH = FONT_WIDTH;
    data.FONT_HEIGHT = FONT_HEIGHT;
    data.MENU_SCREEN_WIDTH = MENU_SCREEN_WIDTH;
    data.MENU_SCREEN_HEIGHT = MENU_SCREEN_HEIGHT;

    //Sprites
//    data.I = l_sprite;
//    data.L = L_sprite;
//    data.O = O_sprite;
//    data.J = J_sprite;
//    data.Z = Z_sprite;
//    data.S = S_sprite;
//    data.T = T_sprite;
//    data.start = start_sprite;
//    data.wall = wall_sprite;
//    data.ground = ground_sprite;
//    data.game_over = game_over_sprite;
    data.title = title_sprite;
    data.fader = fader_sprite;
    data.FONT_CHARACTERS = FONT_CHARACTERS;

    //Font
    for (int i = 0; i < FONT_CHARACTERS; i++) {
        data.font[i] = font[i];
    }

    //Setting up icon
    SDL_Surface* icon = IMG_Load(wall_path);
    SDL_SetWindowIcon(window, icon);

    //////////////////////////
    /* SETTING UP MAIN MENU */
    //////////////////////////

    int selection = 1;
    int dimensions[8] = {0,0,1,0,0,0,1,8};
    int blockWidth[4] = {0,0,0,8};
    bool active = false;

    //Event handler
    SDL_Event event;

    //Main loop flag
    bool quit = false;

    //While application is running
    while( !quit )
    {
        //Handle events on queue
        while( SDL_PollEvent( &event ) != 0 )
        {
            //User requests quit
            if( event.type == SDL_QUIT)
            {
                quit = true;
                break;
            } else if (event.key.keysym.sym == SDLK_ESCAPE && event.type == SDL_KEYDOWN) {
                quit = true;
                break;
            }


            if ((event.key.keysym.sym == SDLK_SPACE && event.type == SDL_KEYDOWN)
                  || ((event.key.keysym.sym == SDLK_RETURN) && event.type == SDL_KEYDOWN)
                  || ((event.key.keysym.sym == SDLK_RIGHT) && event.type == SDL_KEYDOWN)) {
                if (selection == 1) {

                    //Add block width gameData
                    int widthTemp = 0;
                    for (int i = 0; i < 4; i++) {
                        widthTemp = 10 * widthTemp + blockWidth[i];
                    }

                    data.BLOCK_WIDTH = widthTemp;
                    data.BLOCK_HEIGHT = widthTemp;

                    //Add screen dimensions gameData
                    data.SCREEN_WIDTH = data.WIDTH_IN_BLOCKS * data.BLOCK_WIDTH;
                    data.SCREEN_HEIGHT = data.HEIGHT_IN_BLOCKS * data. BLOCK_HEIGHT;

                    //Destroy renderer
                    SDL_DestroyRenderer(renderer);
                    renderer = NULL;

                    //Destroy window
                    SDL_DestroyWindow(window);
                    window = NULL;

                    //Create window
                    window = SDL_CreateWindow( "", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, data.SCREEN_WIDTH + 2 * data.BLOCK_WIDTH, data.SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
                    if( window == NULL )
                    {
                        printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
                        SDL_DestroyWindow(window);
                        SDL_Quit();
                        return 1;
                    }

                    //Create renderer for window
                    renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
                    if( renderer == NULL )
                    {
                        printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
                        SDL_DestroyWindow(window);
                        SDL_Quit();
                        return 1;
                    }

                    data.window = window;
                    data.renderer = renderer;

                    //Reinitializing sprites to fit specifications
                    data.I = create_sprite (data.BLOCK_WIDTH, data.BLOCK_HEIGHT, renderer, l_path, 0, 0);
                    data.L = create_sprite (data.BLOCK_WIDTH, data.BLOCK_HEIGHT, renderer, L_path, 0, 0);
                    data.O = create_sprite (data.BLOCK_WIDTH, data.BLOCK_HEIGHT, renderer, O_path, 0, 0);
                    data.J = create_sprite (data.BLOCK_WIDTH, data.BLOCK_HEIGHT, renderer, J_path, 0, 0);
                    data.Z = create_sprite (data.BLOCK_WIDTH, data.BLOCK_HEIGHT, renderer, Z_path, 0, 0);
                    data.S = create_sprite (data.BLOCK_WIDTH, data.BLOCK_HEIGHT, renderer, S_path, 0, 0);
                    data.T = create_sprite (data.BLOCK_WIDTH, data.BLOCK_HEIGHT, renderer, T_path, 0, 0);
                    data.wall = create_sprite (data.BLOCK_WIDTH, data.BLOCK_HEIGHT, renderer, wall_path, 0, 0);
                    data.ground = create_sprite (data.BLOCK_WIDTH, data.BLOCK_HEIGHT, renderer, ground_path, 0, 0);
                    data.start = create_sprite (4 * data.BLOCK_WIDTH, 2 * data.BLOCK_HEIGHT, renderer, start_path, 0, 0);
                    data.game_over = create_sprite (4 * data.BLOCK_WIDTH, 2 * data.BLOCK_HEIGHT, renderer, game_over_path, 0, 0);
                    data.title = create_sprite_no_resize(TITLE_WIDTH, TITLE_HEIGHT, renderer, title_path, 0, 0);
                    data.fader = create_sprite_no_resize(data.SCREEN_WIDTH, data.SCREEN_WIDTH, renderer, fader_path, 0, 0);
                    SDL_SetTextureAlphaMod(data.fader->texture, 150 );

                    //Font
                    for (int i = 0; i < FONT_CHARACTERS; i++) {
                        data.font[i] = font[i];
                    }

                    int ret = 2;
                    while (ret == 2) {
                        ret = game_loop(&data);
                    }

                    if (ret == 1) {
                        quit = true;
                    } else if (ret == 3) {
                        quit = true;
                    }

                } else if (selection == 2) {
                    //Change dimensions
                    int pos = 0;
                    active = true;
                    //Render
                    SDL_RenderClear(renderer);
                    render_title(data, selection, pos, active, TITLE_WIDTH, MENU_SCREEN_WIDTH, dimensions, blockWidth);
                    SDL_RenderPresent(renderer);
                    //Update the surface
                    SDL_UpdateWindowSurface( window );
                    while(pos != 9) {
                        if (pos == 111) {
                            return 0;
                        }
                        pos = change_dimensions(dimensions, pos);
                        //Render
                        SDL_RenderClear(renderer);
                        render_title(data, selection, pos, active, TITLE_WIDTH, MENU_SCREEN_WIDTH, dimensions, blockWidth);
                        SDL_RenderPresent(renderer);
                        //Update the surface
                        SDL_UpdateWindowSurface( window );
                    }

                    //Add game dimensions to gameData
                    int tempWidth[4];
                    int tempHeight[4];
                    for (int i = 0; i < 4; i++) {
                        tempWidth[i] = dimensions[i];
                        tempHeight[i] = dimensions[i + 4];
                    }

                    int width = 0;
                    int height = 0;
                    for (int i = 0; i < 4; i++) {
                        width = 10 * width + tempWidth[i];
                        height = 10 * height + tempHeight[i];
                    }

                    HEIGHT_IN_BLOCKS = height;
                    WIDTH_IN_BLOCKS = width;
                    data.HEIGHT_IN_BLOCKS = HEIGHT_IN_BLOCKS;
                    data.WIDTH_IN_BLOCKS = WIDTH_IN_BLOCKS;

                    active = false;

                } else if (selection == 3) {
                    //Change block width
                    int pos = 0;
                    active = true;
                    //Render
                    SDL_RenderClear(renderer);
                    render_title(data, selection, pos, active, TITLE_WIDTH, MENU_SCREEN_WIDTH, dimensions, blockWidth);
                    SDL_RenderPresent(renderer);
                    //Update the surface
                    SDL_UpdateWindowSurface( window );
                    while(pos != 9) {
                        if (pos == 111) {
                            return 0;
                        }
                        pos = change_block_width(blockWidth, pos);
                        //Render
                        SDL_RenderClear(renderer);
                        render_title(data, selection, pos, active, TITLE_WIDTH, MENU_SCREEN_WIDTH, dimensions, blockWidth);
                        SDL_RenderPresent(renderer);
                        //Update the surface
                        SDL_UpdateWindowSurface( window );
                    }
                    active = false;

                } else if (selection == 4) {

                    //View High Score
                    int mode;
                    mode = 10000 * WIDTH_IN_BLOCKS + HEIGHT_IN_BLOCKS;
                    if(show_high_score(renderer, mode, data, 1) == 0) {
                        return 0;
                    }

                    //Empty key queue
                    while (SDL_PollEvent(&event) != 0) {}

                    //Render
                    SDL_RenderClear(renderer);
                    render_title(data, selection, 3, active, TITLE_WIDTH, MENU_SCREEN_WIDTH, dimensions, blockWidth);
                    SDL_RenderPresent(renderer);
                    //Update the surface
                    SDL_UpdateWindowSurface(window);

                } else if (selection == 5) {
                    //Quit game
                    return 0;
                }
            } else if ((event.key.keysym.sym == SDLK_UP) && event.type == SDL_KEYDOWN) {
                selection = decrease_selection(selection);
            } else if ((event.key.keysym.sym == SDLK_DOWN) && event.type == SDL_KEYDOWN) {
                selection = increase_selection(selection);
            }
        }

        /* Render Title Screen */

        //Render
        SDL_RenderClear(renderer);

        render_title(data, selection, 0, active, TITLE_WIDTH, MENU_SCREEN_WIDTH, dimensions, blockWidth);

        SDL_RenderPresent(renderer);

        //Update the surface
        SDL_UpdateWindowSurface(window);

        //Delay
        SDL_Delay(20);
    }

    //Destroy renderer
    SDL_DestroyRenderer(renderer);
    renderer = NULL;

    //Destroy window
    SDL_DestroyWindow(window);
    window = NULL;

    //Quit SDL
    SDL_Quit();

    //Quit IMG
    IMG_Quit();

    //Free sprites
    free_sprite(wall_sprite);
    free_sprite(title_sprite);
    free_sprite(fader_sprite);

    //Free font sprites
    for (int i = 0; i < FONT_CHARACTERS; i++) {
        free_sprite(font[i]);
    }

}


sprite* create_sprite (int wDesired, int hDesired, SDL_Renderer* renderer, char path[256], int xpos,int ypos) {
    sprite* sprite;

    int scalingFactorx;
    int scalingFactory;
    int imageWidth;
    int imageHeight;

    SDL_Surface* surface = malloc(sizeof(surface));
    surface = IMG_Load(path);
    imageWidth = surface->w;
    imageHeight = surface->h;
    scalingFactorx = (imageWidth / wDesired);
    scalingFactory = (imageHeight / hDesired);

    if ((imageWidth % wDesired) > 0) {
        scalingFactorx++;
    }
    if ((imageHeight % hDesired) > 0) {
        scalingFactory++;
    }
    surface = shrinkSurface(surface, scalingFactorx, scalingFactory);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    sprite = init_sprite(texture, xpos, ypos, wDesired, hDesired);

    return sprite;
}

sprite* create_sprite_no_resize (int wDesired, int hDesired, SDL_Renderer* renderer, char path[256], int xpos, int ypos) {
    sprite* sprite;

    SDL_Surface* surface = malloc(sizeof(surface));
    surface = IMG_Load(path);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    sprite = init_sprite(texture, xpos, ypos, wDesired, hDesired);

    return sprite;
}

int increase_selection(int selection) {
    int newSelection;

    if (selection == 5) {
        newSelection = 1;
    } else {
        newSelection = selection + 1;
    }

    return newSelection;
}

int decrease_selection(int selection) {
    int newSelection;

    if (selection == 1) {
        newSelection = 5;
    } else {
        newSelection = selection - 1;
    }

    return newSelection;
}

int change_dimensions(int* dimensions, int pos) {
    int temp;

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
                return 111;
            }

            if (event.key.keysym.sym == SDLK_ESCAPE) {
                exit = true;
            } else if ((event.key.keysym.sym == SDLK_UP) && event.type == SDL_KEYDOWN) {
                temp = dimensions[pos];
                temp++;
                temp = temp % 10;
                dimensions[pos] = temp;
                return pos;
            } else if ((event.key.keysym.sym == SDLK_DOWN) && event.type == SDL_KEYDOWN) {
                temp = dimensions[pos];
                temp = temp - 1;
                if (temp < 0) {
                    temp = 9;
                }
                dimensions[pos] = temp;
                return pos;
            } else if ((event.key.keysym.sym == SDLK_LEFT) && event.type == SDL_KEYDOWN) {
                if (pos > 0)
                    pos = pos - 1;
                return pos;
            } else if ((event.key.keysym.sym == SDLK_RIGHT) && event.type == SDL_KEYDOWN) {
                if (pos < 7)
                    pos++;
                return pos;
            } else if (((event.key.keysym.sym == SDLK_RETURN) && event.type == SDL_KEYDOWN)
                       || (event.key.keysym.sym == SDLK_SPACE && event.type == SDL_KEYDOWN)) {
                exit = true;
                break;
            }
        }

        //Delay
        SDL_Delay(20);
    }
    return 9;
}

int change_block_width(int* blockWidth, int pos) {
    int temp;

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
                return 111;
            }

            if (event.key.keysym.sym == SDLK_ESCAPE) {
                exit = true;
            } else if ((event.key.keysym.sym == SDLK_UP) && event.type == SDL_KEYDOWN) {
                temp = blockWidth[pos];
                temp++;
                temp = temp % 10;
                blockWidth[pos] = temp;
                return pos;
            } else if ((event.key.keysym.sym == SDLK_DOWN) && event.type == SDL_KEYDOWN) {
                temp = blockWidth[pos];
                temp = temp - 1;
                if (temp < 0) {
                    temp = 9;
                }
                blockWidth[pos] = temp;
                return pos;
            } else if ((event.key.keysym.sym == SDLK_LEFT) && event.type == SDL_KEYDOWN) {
                if (pos > 0)
                    pos = pos - 1;
                return pos;
            } else if ((event.key.keysym.sym == SDLK_RIGHT) && event.type == SDL_KEYDOWN) {
                if (pos < 3)
                    pos++;
                return pos;
            } else if (((event.key.keysym.sym == SDLK_RETURN) && event.type == SDL_KEYDOWN)
                       || (event.key.keysym.sym == SDLK_SPACE && event.type == SDL_KEYDOWN)) {
            exit = true;
            break;
            }

        }

        //Delay
        SDL_Delay(20);
    }
    return 9;
}

void render_title(gameData data, int selection, int pos, bool active, int TITLE_WIDTH, int SCREEN_WIDTH, int dimensions[8], int blockWidth[4]) {

    int xTitle = (SCREEN_WIDTH - TITLE_WIDTH) / 2;
    int yTitle = 40;
    render_sprite(data.title, xTitle, yTitle, data.renderer);

    //Rendering dimensions
    char charDimensions[256];
    for (int i = 0; i < 8; i++) {
        if (i < 4) {
            sprintf(&charDimensions[i], "%d", dimensions[i]);
        } else {
            if (i == 4) {
                sprintf(&charDimensions[i], " ");
                sprintf(&charDimensions[i + 1], "X");
                sprintf(&charDimensions[i + 2], " ");
            }
            sprintf(&charDimensions[i + 3], "%d", dimensions[i]);
        }
    }
    char dimensionString[256];
    sprintf(dimensionString, "GAME DIMENSIONS: %s (WIDTH X HEIGHT)", charDimensions);

    //Rendering block size
    char charBlockWidth[256];
    for (int i = 0; i < 4; i++) {
        sprintf(&charBlockWidth[i], "%d", blockWidth[i]);
    }
    char blockWidthString[256];
    sprintf(blockWidthString, "BLOCK WIDTH: %s PIXELS", charBlockWidth);

    render_font("PLAY", 50, 260, data.renderer, data.FONT_WIDTH, data.FONT_HEIGHT, data.font);
    render_font(dimensionString, 50, 300, data.renderer, data.FONT_WIDTH, data.FONT_HEIGHT, data.font);
    render_font(blockWidthString, 50, 340, data.renderer, data.FONT_WIDTH, data.FONT_HEIGHT, data.font);
    render_font("HIGH SCORE", 50, 380, data.renderer, data.FONT_WIDTH, data.FONT_HEIGHT, data.font);
    render_font("EXIT", 50, 420, data.renderer, data.FONT_WIDTH, data.FONT_HEIGHT, data.font);
    render_sprite(data.fader, 0, 0, data.renderer);

    if (!active) {
        if (selection == 1) {
            render_font("PLAY", 50, 260, data.renderer, data.FONT_WIDTH, data.FONT_HEIGHT, data.font);
        } else if (selection == 2){
            render_font(dimensionString, 50, 300, data.renderer, data.FONT_WIDTH, data.FONT_HEIGHT, data.font);
        } else if (selection == 3){
            render_font(blockWidthString, 50, 340, data.renderer, data.FONT_WIDTH, data.FONT_HEIGHT, data.font);
        } else if (selection == 4){
            render_font("HIGH SCORE", 50, 380, data.renderer, data.FONT_WIDTH, data.FONT_HEIGHT, data.font);
        } else if (selection == 5){
            render_font("EXIT", 50, 420, data.renderer, data.FONT_WIDTH, data.FONT_HEIGHT, data.font);
        }
    } else {
        if (selection == 1) {
            render_font("PLAY", 50, 260, data.renderer, data.FONT_WIDTH, data.FONT_HEIGHT, data.font);
        } else if (selection == 2){
            char tempChar[16];
            int add = 0;
            if (pos > 3) {
                add = 3;
            }
            sprintf(tempChar, "%d", dimensions[pos]);
            render_font(tempChar, 50 + (data.FONT_WIDTH * (17 + pos + add)), 300, data.renderer, data.FONT_WIDTH, data.FONT_HEIGHT, data.font);
        } else if (selection == 3){
            char tempChar[8];
            sprintf(tempChar, "%d", blockWidth[pos]);
            render_font(tempChar, 50 + (data.FONT_WIDTH * (13 + pos)), 340, data.renderer, data.FONT_WIDTH, data.FONT_HEIGHT, data.font);
        } else if (selection == 4){
            render_font("HIGH SCORE", 50, 380, data.renderer, data.FONT_WIDTH, data.FONT_HEIGHT, data.font);
        } else if (selection == 5){
            render_font("EXIT", 50, 420, data.renderer, data.FONT_WIDTH, data.FONT_HEIGHT, data.font);
        }
    }

    render_sprite(data.title, xTitle, yTitle, data.renderer);
    render_font("WWW.SUNANDWEATHER.COM", 409, 455, data.renderer, data.FONT_WIDTH, data.FONT_HEIGHT, data.font);
}





