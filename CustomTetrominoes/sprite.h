//
//  sprite.h
//  Tetris
//
//  Created by Johan Sunnanväder on 2016-12-30.
//  Copyright (c) 2016 SunandWeather. All rights reserved.
//

#ifndef Tetris_sprite_h
#define Tetris_sprite_h

#include <SDL2/SDL.h>

// Defining the sprite structure
typedef struct {
    SDL_Texture* texture;
    SDL_Rect rect;
} sprite;

// Initialize sprite from texture
sprite *init_sprite(SDL_Texture* texture, int x, int y, int w, int h);

// Free sprite
void free_sprite(sprite* s);

// Render sprite
void render_sprite(sprite * s, int x, int y, SDL_Renderer* r);

#endif
