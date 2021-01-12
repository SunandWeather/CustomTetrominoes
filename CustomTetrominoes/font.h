//
//  font.h
//  CustomTetrominoes
//
//  Created by Johan Sunnanväder on 2017-02-04.
//  Copyright (c) 2017 SunandWeather. All rights reserved.
//

#include <SDL2/SDL.h>
#include "sprite.h"

#ifndef CustomTetrominoes_font_h
#define CustomTetrominoes_font_h

//Render a given string
void render_font(char* string, int x, int y, SDL_Renderer* r, int FONT_WIDTH, int FONT_HEIGHT, sprite* font[]);


#endif
