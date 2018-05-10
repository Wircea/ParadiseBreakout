#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <iostream>

using namespace std;

int SCREEN_WIDTH=800;
int SCREEN_HEIGHT=600;

SDL_Window* theWindow;
SDL_Surface* screenSurface;

bool PrepareSDL()
{
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        return false;
    }
    else
    {
        theWindow = SDL_CreateWindow( "Paradise Breakout", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( theWindow == NULL )
        {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            return false;
        }
        else
        {
            screenSurface = SDL_GetWindowSurface( theWindow );
        }
    }

    return true;
}

int main(int argc, char* args[])
{
    PrepareSDL();


    return 0;
}
