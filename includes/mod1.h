//
// Created by Robert JONES on 2016/10/25.
//

#ifndef MOD1_MOD1_H_H
#define MOD1_MOD1_H_H

#include "../includes/map.h"
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <regex>
#include <math.h>
#include "SDL.h"

typedef struct  s_sdl{
    SDL_Window *window;
    SDL_Renderer *render;
    SDL_Texture *texture;
}               t_sdl;

using namespace std;

#endif //MOD1_MOD1_H_H
