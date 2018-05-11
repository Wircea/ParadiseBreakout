#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <iostream>

#include "textureHandler.cpp"

using namespace std;

int SCREEN_WIDTH=800;
int SCREEN_HEIGHT=600;

SDL_Window* theWindow;
SDL_Surface* theSurface;
SDL_Renderer* theRenderer;



void display();

bool PrepareSDL()
{

	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		return false;
	}
	else
	{
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}
		theWindow = SDL_CreateWindow( "Paradise Breakout", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( theWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			return false;
		}
		else
		{
			theRenderer = SDL_CreateRenderer( theWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( theRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				return false;
			}
			else
			{
				SDL_SetRenderDrawColor( theRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					return false;
				}
			}
		}
	}
return true;
}

textureType missingTex;
textureType debug_topdownPlayer;

void LoadImages()
{
    string prefixPath="assets/images/";
    missingTex.loadFromFile(prefixPath+"missingtex.png");

    if(!debug_topdownPlayer.loadFromFile(prefixPath+"debug_topdownPlayer.png"))
        debug_topdownPlayer=missingTex;


    //if( !patrat_simplu.loadFromFile( "imagini/patrat_simplu.png"  ))

}

void CloseAll()
{
    //SDL_FreeSurface( surface );
    //surface = NULL;

    SDL_DestroyWindow( theWindow );
    theWindow = NULL;

    SDL_Quit();
}


int main(int argc, char* args[])
{
    int y=4;
    int *p=&y;
    cout<<*p<<" ";
    testint = p;


    display();


    if(PrepareSDL())
    {
            renderRef = theRenderer;
            LoadImages();
            bool quit = false;
			SDL_Event e;
			while( !quit )
			{
				while( SDL_PollEvent( &e ) != 0 )
				{
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
				}
				SDL_SetRenderDrawColor( theRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				SDL_RenderClear( theRenderer );
				SDL_SetRenderDrawColor( theRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				SDL_RenderClear( theRenderer );

				debug_topdownPlayer.render(2,2);

				SDL_RenderPresent( theRenderer );
            }
    }
    CloseAll();
    return 0;
}
