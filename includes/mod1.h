#ifndef MOD1_MOD1_H
#define MOD1_MOD1_H

#include <iostream>
#include <regex>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "SDL.h"
#include <stdio.h>
#include "map.h"

typedef struct  s_sdl
{
    SDL_Window      *window;
    SDL_Surface     *screenSurface;
    SDL_Renderer    *render;
    SDL_Texture     *texture;
}               t_sdl;

#endif //MOD1_MOD1_H
