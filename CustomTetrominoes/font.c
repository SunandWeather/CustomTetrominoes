//
//  font.c
//  CustomTetrominoes
//
//  Created by Johan Sunnanväder on 2017-02-04.
//  Copyright (c) 2017 SunandWeather. All rights reserved.
//

#include <stdio.h>
#include "game.h"
#include "font.h"
#include "sprite.h"

//Render a given string
void render_font(char* string, int x, int y, SDL_Renderer* r, int FONT_WIDTH, int FONT_HEIGHT, sprite* font[]) {
    
    int xStart = x;
    char symbols[60] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789<>\a\v:-,._ !\"#%&/()=?`;'";
    
    for (int i = 0; i < strlen(string); i++) {
        for (int n = 0; n < sizeof(symbols); n++) {
            if (string[i] == symbols[n]) {
                render_sprite(font[n], x, y, r);
                x = x + FONT_WIDTH;
                break;
            }
            if (string[i] == '\\') {
                y = y + FONT_HEIGHT;
                x = xStart;
                break;
            }
        }
    }
    
}