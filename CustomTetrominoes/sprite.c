//
//  sprite.c
//  Custom Tetrominoes
//
//  Created by Johan Sunnanväder on 2016-12-31.
//  Copyright (c) 2016 SunandWeather. All rights reserved.
//
#include "sprite.h"

#include <stdio.h>
#include <stdlib.h>

// Initialize sprite from texture
sprite *init_sprite(SDL_Texture* texture, int x, int y, int w, int h)
{
    sprite* s = malloc(sizeof(sprite));
    
    // sprite texture
    s->texture = texture;
    
    // Coordinates of sprite in texture
    s->rect.x = x;
    s->rect.y = y;
    
    // Dimensions of sprite
    s->rect.w = w;
    s->rect.h = h;
    
    return s;
}

// Free sprite
void free_sprite(sprite* s)
{
    SDL_DestroyTexture(s->texture);
    free(s);
}

// Render sprite
void render_sprite(sprite * s, int x, int y, SDL_Renderer* r)
{
    SDL_Rect rendRect;
    
    // Coordinates to render to
    rendRect.x = x;
    rendRect.y = y;
    
    // Render dimensions
    rendRect.w = s->rect.w;
    rendRect.h = s->rect.h;
    
    SDL_RenderCopy(r, s->texture, &s->rect, &rendRect);
}

#include <stdio.h>


