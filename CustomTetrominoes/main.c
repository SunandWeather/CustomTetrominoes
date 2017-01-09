//
//  main.c
//  Tetris
//
//  Created by Johan Sunnanväder on 2016-12-29.
//  Copyright (c) 2016 SunandWeather. All rights reserved.
//

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

#include "time.h"


int HIGHT_IN_BLOCKS;
int WIDTH_IN_BLOCKS;
int BLOCK_WIDTH;
int BLOCK_HEIGHT;
int SCREEN_HEIGHT;
int SCREEN_WIDTH;

//Initialize main game function
int game_loop(gameData* data);

//Loads media
SDL_Texture* loadMedia(char path, SDL_Rect rect, SDL_Renderer *renderer);

//The window we'll be rendering to
SDL_Window* window = NULL;

//The window renderer
SDL_Renderer* renderer = NULL;

//Current displayed texture
SDL_Texture* gTexture = NULL;

/* Main function */
int main(int argc, const char * argv[])
{
    
    //Letting the user choose dimensions
    printf("Number of rows (height): ");
    scanf("%i", &HIGHT_IN_BLOCKS);
    printf("Number of collums (width): ");
    scanf("%i", &WIDTH_IN_BLOCKS);
    printf("Block width (and height) in pixels: ");
    scanf("%i", &BLOCK_WIDTH);
    
    BLOCK_HEIGHT = BLOCK_WIDTH;
    SCREEN_HEIGHT = HIGHT_IN_BLOCKS * BLOCK_WIDTH;
    SCREEN_WIDTH = WIDTH_IN_BLOCKS * BLOCK_WIDTH + 2 * BLOCK_WIDTH;
    
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
    window = SDL_CreateWindow( "", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
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
    
    /* Set up the sprites */
	SDL_Texture* l_block_texture = IMG_LoadTexture(renderer, "res/I_block.png");
    SDL_Texture* L_block_texture = IMG_LoadTexture(renderer, "res/L_block.png");
    SDL_Texture* O_block_texture = IMG_LoadTexture(renderer, "res/O_block.png");
    SDL_Texture* J_block_texture = IMG_LoadTexture(renderer, "res/J_block.png");
    SDL_Texture* Z_block_texture = IMG_LoadTexture(renderer, "res/Z_block.png");
    SDL_Texture* S_block_texture = IMG_LoadTexture(renderer, "res/S_block.png");
    SDL_Texture* T_block_texture = IMG_LoadTexture(renderer, "res/T_block.png");
    SDL_Texture* wall_texture = IMG_LoadTexture(renderer, "res/wall.png");
    SDL_Texture* ground_texture = IMG_LoadTexture(renderer, "res/ground.png");
    SDL_Texture* title_texture = IMG_LoadTexture(renderer, "res/title.png");
    SDL_Texture* game_over_texture = IMG_LoadTexture(renderer, "res/game_over.png");
    
    sprite* l_sprite = init_sprite(l_block_texture, 0, 0, BLOCK_HEIGHT, BLOCK_WIDTH);
    sprite* L_sprite = init_sprite(L_block_texture, 0, 0, BLOCK_HEIGHT, BLOCK_WIDTH);
    sprite* O_sprite = init_sprite(O_block_texture, 0, 0, BLOCK_HEIGHT, BLOCK_WIDTH);
    sprite* J_sprite = init_sprite(J_block_texture, 0, 0, BLOCK_HEIGHT, BLOCK_WIDTH);
    sprite* Z_sprite = init_sprite(Z_block_texture, 0, 0, BLOCK_HEIGHT, BLOCK_WIDTH);
    sprite* S_sprite = init_sprite(S_block_texture, 0, 0, BLOCK_HEIGHT, BLOCK_WIDTH);
    sprite* T_sprite = init_sprite(T_block_texture, 0, 0, BLOCK_HEIGHT, BLOCK_WIDTH);
    sprite* wall_sprite = init_sprite(wall_texture, 0, 0, BLOCK_HEIGHT, BLOCK_WIDTH);
    sprite* ground_sprite = init_sprite(ground_texture, 0, 0, BLOCK_HEIGHT, BLOCK_WIDTH);
    sprite* title_sprite = init_sprite(title_texture, 0, 0, BLOCK_WIDTH * 4, BLOCK_HEIGHT * 2);
    sprite* game_over_sprite = init_sprite(game_over_texture, 0, 0, BLOCK_WIDTH * 4, BLOCK_HEIGHT * 2);
    
    
    /* Compiling game data */
    gameData data;
    
    //SDL
    data.window = window;
    data.renderer = renderer;
    
    //Screen dimensions
    data.HIGHT_IN_BLOCKS = HIGHT_IN_BLOCKS;
    data.WIDTH_IN_BLOCKS = WIDTH_IN_BLOCKS;
    data.BLOCK_WIDTH = BLOCK_WIDTH;
    data.BLOCK_HEIGHT = BLOCK_HEIGHT;
    data.SCREEN_HEIGHT = SCREEN_HEIGHT;
    data.SCREEN_WIDTH = SCREEN_WIDTH;
    
    //Sprites
    data.l = l_sprite;
    data.L = L_sprite;
    data.O = O_sprite;
    data.J = J_sprite;
    data.Z = Z_sprite;
    data.S = S_sprite;
    data.T = T_sprite;
    data.wall = wall_sprite;
    data.ground = ground_sprite;
    data.game_over = game_over_sprite;
    
    //Event handler
    SDL_Event e;
    
    //Main loop flag
    bool quit = false;
    
    //While application is running
    while( !quit )
    {
        //Handle events on queue
        while( SDL_PollEvent( &e ) != 0 )
        {
            //User requests quit
            if( e.type == SDL_QUIT)
            {
                quit = true;
                break;
            } if (e.key.keysym.sym == SDLK_SPACE) {
                while (game_loop(&data) == 2) {
                }
                
                quit = true;
            }
        }
        
        
        /* Render Title Screen */
        SDL_RenderClear(renderer);
        
        render_sprite(title_sprite, (WIDTH_IN_BLOCKS / 2 - 1) * BLOCK_HEIGHT,
                      (HIGHT_IN_BLOCKS / 2 - 1) * BLOCK_HEIGHT, renderer);
        
		SDL_RenderPresent(renderer);

        //Update the surface
        SDL_UpdateWindowSurface( window );
        
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
    free_sprite(l_sprite);
    free_sprite(L_sprite);
    free_sprite(O_sprite);
    free_sprite(J_sprite);
    free_sprite(Z_sprite);
    free_sprite(S_sprite);
    free_sprite(T_sprite);
    free_sprite(wall_sprite);
    free_sprite(ground_sprite);
    free_sprite(title_sprite);
    free_sprite(game_over_sprite);
    
}
