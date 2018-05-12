#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <cmath>

#include "textureHandler.cpp"
#include "player.cpp"

using namespace std;

int SCREEN_WIDTH=800;
int SCREEN_HEIGHT=600;

SDL_Window* theWindow;
SDL_Surface* theSurface;
SDL_Renderer* theRenderer;

playerChar thePlayer;

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
textureType walltest;
textureType angletest;

void LoadImages()
{
    string prefixPath="assets/images/";
    missingTex.loadFromFile(prefixPath+"missingtex.png");

    if(!debug_topdownPlayer.loadFromFile(prefixPath+"debug_topdownPlayer.png"))
        debug_topdownPlayer=missingTex;
    if(!walltest.loadFromFile(prefixPath+"walltest.png"))
        walltest=missingTex;
    if(!angletest.loadFromFile(prefixPath+"angletest.png"))
        angletest=missingTex;

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

int walltestposx=300;
int walltestposy=300;
int walltestposz=364;
int walltestposh=364;

int main(int argc, char* args[])
{
    int y=4;
    int *p=&y;
    cout<<*p<<" ";
    testint = p;


    display();

    struct pastMousePos
    {
        int x;int y;
    }pastMouse;


    SDL_GetMouseState( &pastMouse.x, &pastMouse.y );

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
					if( e.type == SDL_MOUSEMOTION)
                    {
                        int mouseX, mouseY;
                        SDL_GetMouseState( &mouseX, &mouseY );

                        thePlayer.rotation+=mouseX-pastMouse.x;
                        if(thePlayer.rotation>359)
                            thePlayer.rotation=thePlayer.rotation-359;
                        if(thePlayer.rotation<0)
                            thePlayer.rotation=thePlayer.rotation+359;

                        pastMouse.x=mouseX;
                        cout<<thePlayer.rotation<<"\n";
                    }
				}
				SDL_SetRenderDrawColor( theRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				SDL_RenderClear( theRenderer );
				SDL_SetRenderDrawColor( theRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				SDL_RenderClear( theRenderer );

				debug_topdownPlayer.render(thePlayer.posx,thePlayer.posy,NULL,thePlayer.rotation);
				//walltest.render(walltestposx,walltestposy);

				SDL_SetRenderDrawColor( theRenderer, 0x00, 0x00, 0xFF, 0xFF );

                SDL_Point a = {1, 2};
                SDL_Point* b=&a;
                //angletest.render(thePlayer.posx+32, thePlayer.posy+32, NULL,thePlayer.rotation+45,b);

                SDL_SetRenderDrawColor( theRenderer, 0x00, 0x00, 0xFF, 0xFF );

                double fovleft=thePlayer.rotation+thePlayer.fov/2;
                double fovright=thePlayer.rotation-thePlayer.fov/2;

                if(fovleft<0)
                    fovleft+=359;
                if(fovright>359)
                    fovright-=359;

                if(walltestposx<thePlayer.posx+32- thePlayer.visionLength* sin(fovleft*0.01745329251))
                if(walltestposy<thePlayer.posy+32- thePlayer.visionLength* cos(fovright*0.01745329251))
                    //walltest.render(walltestposx,walltestposy);}
                    //SDL_RenderDrawLine( theRenderer,walltestposx,walltestposy,walltestposz,walltestposh);}
                     cout<<walltestposx<<" compared to "<<thePlayer.posx+32- thePlayer.visionLength* sin(fovleft*0.01745329251)<<"\n";




                SDL_RenderDrawLine( theRenderer, thePlayer.posx+32, thePlayer.posy+32,thePlayer.posx+32- thePlayer.visionLength* sin(fovleft*0.01745329251) ,thePlayer.posy+32+ thePlayer.visionLength*cos(fovleft*0.0174532925) );
                SDL_RenderDrawLine( theRenderer, thePlayer.posx+32, thePlayer.posy+32,thePlayer.posx+32- thePlayer.visionLength* sin(fovright*0.01745329251) ,thePlayer.posy+32+ thePlayer.visionLength*cos(fovright*0.0174532925) );

                double distanceBetween = sqrt(pow(thePlayer.posx+32-walltestposx,2)+pow(thePlayer.posy+32-walltestposy,2));


                SDL_RenderDrawLine( theRenderer, thePlayer.posx+32, thePlayer.posy+32,thePlayer.posx+32- distanceBetween* sin(thePlayer.rotation* 0.01745329251) ,thePlayer.posy+32+ distanceBetween*cos(thePlayer.rotation* 0.0174532925) );
                SDL_SetRenderDrawColor( theRenderer, 0x00, 0x00, 0x00, 0xFF );
                SDL_RenderDrawLine( theRenderer, thePlayer.posx+32- distanceBetween* sin(thePlayer.rotation* 0.01745329251) ,thePlayer.posy+32+ distanceBetween*cos(thePlayer.rotation* 0.0174532925),thePlayer.posx+32- distanceBetween* sin(fovright*0.01745329251) ,thePlayer.posy+32+ distanceBetween*cos(fovright*0.0174532925) );
                SDL_RenderDrawLine( theRenderer, thePlayer.posx+32- distanceBetween* sin(thePlayer.rotation* 0.01745329251) ,thePlayer.posy+32+ distanceBetween*cos(thePlayer.rotation* 0.0174532925),thePlayer.posx+32- distanceBetween* sin(fovleft*0.01745329251) ,thePlayer.posy+32+ distanceBetween*cos(fovleft*0.0174532925) );
                //SDL_RenderDrawLine( theRenderer, thePlayer.posx+32- distanceBetween* sin(thePlayer.rotation* 0.01745329251) ,thePlayer.posy+32+ distanceBetween*cos(thePlayer.rotation* 0.0174532925),walltestposx,walltestposy );

                double lengthToLeft=sqrt(pow(thePlayer.posx+32- distanceBetween* sin(fovleft*0.01745329251)-(thePlayer.posx+32- distanceBetween* sin(thePlayer.rotation* 0.01745329251)),2)+pow(thePlayer.posy+32+ distanceBetween*cos(fovleft*0.0174532925)-(thePlayer.posy+32+ distanceBetween*cos(thePlayer.rotation* 0.0174532925)),2));

                double distanceToObject=sqrt(pow( thePlayer.posx+32- distanceBetween* sin(thePlayer.rotation* 0.01745329251)-walltestposx,2)+pow(thePlayer.posy+32+ distanceBetween*cos(thePlayer.rotation* 0.0174532925)-walltestposy,2));

                cout<<"distanceLeft: "<<distanceBetween<<"distanceObject:"<<distanceToObject<<"\n";

                if(distanceToObject<lengthToLeft)
                SDL_RenderDrawPoint( theRenderer, walltestposx, walltestposy );

				SDL_RenderPresent( theRenderer );

				thePlayer.posx++;
				thePlayer.posy++;
            }
    }
    CloseAll();
    return 0;
}
