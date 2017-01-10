//
//  main.c
//  Custom Tetrominoes
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
#include "SDL2_rotozoom.h"


int HIGHT_IN_BLOCKS;
int WIDTH_IN_BLOCKS;
int BLOCK_WIDTH;
int BLOCK_HEIGHT;
int SCREEN_HEIGHT;
int SCREEN_WIDTH;

//Initialize main game function
int game_loop(gameData* data);

//Initialize file reader and resizer
sprite* create_sprite (int wDesired, int hDesired, SDL_Renderer* renderer, char path[256]);

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
    
    //Initialize paths
    char l_path[256] = "/Users/SunandWeather/Documents/Xcode/CustomTetrominoes/CustomTetrominoes/res/I_block.png";
    char L_path[256] = "/Users/SunandWeather/Documents/Xcode/CustomTetrominoes/CustomTetrominoes/res/L_block.png";
    char O_path[256] = "/Users/SunandWeather/Documents/Xcode/CustomTetrominoes/CustomTetrominoes/res/O_block.png";
    char J_path[256] = "/Users/SunandWeather/Documents/Xcode/CustomTetrominoes/CustomTetrominoes/res/J_block.png";
    char Z_path[256] = "/Users/SunandWeather/Documents/Xcode/CustomTetrominoes/CustomTetrominoes/res/Z_block.png";
    char S_path[256] = "/Users/SunandWeather/Documents/Xcode/CustomTetrominoes/CustomTetrominoes/res/S_block.png";
    char T_path[256] = "/Users/SunandWeather/Documents/Xcode/CustomTetrominoes/CustomTetrominoes/res/T_block.png";
    char wall_path[256] = "/Users/SunandWeather/Documents/Xcode/CustomTetrominoes/CustomTetrominoes/res/wall.png";
    char ground_path[256] = "/Users/SunandWeather/Documents/Xcode/CustomTetrominoes/CustomTetrominoes/res/ground.png";
    char title_path[256] = "/Users/SunandWeather/Documents/Xcode/CustomTetrominoes/CustomTetrominoes/res/title.png";
    char game_over_path[256] = "/Users/SunandWeather/Documents/Xcode/CustomTetrominoes/CustomTetrominoes/res/game_over.png";
    
    //Creating sprites
    sprite* l_sprite = create_sprite (BLOCK_WIDTH, BLOCK_HEIGHT, renderer, l_path);
    sprite* L_sprite = create_sprite (BLOCK_WIDTH, BLOCK_HEIGHT, renderer, L_path);
    sprite* O_sprite = create_sprite (BLOCK_WIDTH, BLOCK_HEIGHT, renderer, O_path);
    sprite* J_sprite = create_sprite (BLOCK_WIDTH, BLOCK_HEIGHT, renderer, J_path);
    sprite* Z_sprite = create_sprite (BLOCK_WIDTH, BLOCK_HEIGHT, renderer, Z_path);
    sprite* S_sprite = create_sprite (BLOCK_WIDTH, BLOCK_HEIGHT, renderer, S_path);
    sprite* T_sprite = create_sprite (BLOCK_WIDTH, BLOCK_HEIGHT, renderer, T_path);
    sprite* wall_sprite = create_sprite (BLOCK_WIDTH, BLOCK_HEIGHT, renderer, wall_path);
    sprite* ground_sprite = create_sprite (BLOCK_WIDTH, BLOCK_HEIGHT, renderer, ground_path);
    sprite* title_sprite = create_sprite (4 * BLOCK_WIDTH, 2 * BLOCK_HEIGHT, renderer, title_path);
    sprite* game_over_sprite = create_sprite (4 * BLOCK_WIDTH, 2 * BLOCK_HEIGHT, renderer, game_over_path);
    
    // Compiling game data
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

sprite* create_sprite (int wDesired, int hDesired, SDL_Renderer* renderer, char path[256]) {
    sprite* sprite;
    int scalingFactorx;
    int scalingFactory;
    int imageWidth;
    int imageHeight;
    
    SDL_Surface* surface = IMG_Load(path);
    imageWidth = surface->w;
    imageHeight = surface->h;
    scalingFactorx = (imageWidth / wDesired) + 1;
    scalingFactory = (imageHeight / hDesired) + 1;
    if ((imageWidth % wDesired) > 0) {
        scalingFactorx++;
    }
    if ((imageHeight % hDesired) > 0) {
        scalingFactory++;
    }
    surface = shrinkSurface(surface, scalingFactorx, scalingFactory);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    
    sprite = init_sprite(texture, 0, 0, wDesired, hDesired);
    
    return sprite;
}
