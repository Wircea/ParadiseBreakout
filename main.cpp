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
#include "wall.cpp"

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
textureType dottest;

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
    if(!dottest.loadFromFile(prefixPath+"dottest.png"))
        dottest=missingTex;

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

wallType wallTest;

int AVAILABLE_WALLS=3;

wallType worldWalls[50];

int main(int argc, char* args[])
{
    thePlayer.posx=SCREEN_WIDTH/2;
    thePlayer.posy=SCREEN_HEIGHT/2;

    worldWalls[0].posx1=214;
    worldWalls[0].posy1=50;
    worldWalls[0].posx2=582;
    worldWalls[0].posy2=53;

    worldWalls[1].posx1=582;
    worldWalls[1].posy1=50;
    worldWalls[1].posx2=680;
    worldWalls[1].posy2=230;

    worldWalls[2].posx1=680;
    worldWalls[2].posy1=230;
    worldWalls[2].posx2=670;
    worldWalls[2].posy2=520;

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
                        //cout<<thePlayer.rotation<<"\n";
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

                //if(walltestposx<thePlayer.posx+32- thePlayer.visionLength* sin(fovleft*0.01745329251))
                //if(walltestposy<thePlayer.posy+32- thePlayer.visionLength* cos(fovright*0.01745329251))
                    //walltest.render(walltestposx,walltestposy);}
                    //SDL_RenderDrawLine( theRenderer,walltestposx,walltestposy,walltestposz,walltestposh);}
                     //cout<<walltestposx<<" compared to "<<thePlayer.posx+32- thePlayer.visionLength* sin(fovleft*0.01745329251)<<"\n";


                for(int i=0;i<AVAILABLE_WALLS;i++)
                {


                SDL_RenderDrawLine( theRenderer, thePlayer.posx+32, thePlayer.posy+32,thePlayer.posx+32- thePlayer.visionLength* sin(fovleft*0.01745329251) ,thePlayer.posy+32+ thePlayer.visionLength*cos(fovleft*0.0174532925) );
                SDL_RenderDrawLine( theRenderer, thePlayer.posx+32, thePlayer.posy+32,thePlayer.posx+32- thePlayer.visionLength* sin(fovright*0.01745329251) ,thePlayer.posy+32+ thePlayer.visionLength*cos(fovright*0.0174532925) );

                //double distanceBetween = sqrt(pow(thePlayer.posx+32-worldWalls[i]posx,2)+pow(thePlayer.posy+32-worldWalls[i]posy,2));

                worldWalls[i].dist1=sqrt(pow(thePlayer.posx+32-worldWalls[i].posx1,2)+pow(thePlayer.posy+32-worldWalls[i].posy1,2));
                worldWalls[i].dist2=sqrt(pow(thePlayer.posx+32-worldWalls[i].posx2,2)+pow(thePlayer.posy+32-worldWalls[i].posy2,2));

                //SDL_RenderDrawLine( theRenderer, thePlayer.posx+32, thePlayer.posy+32,thePlayer.posx+32- distanceBetween* sin(thePlayer.rotation* 0.01745329251) ,thePlayer.posy+32+ distanceBetween*cos(thePlayer.rotation* 0.0174532925) );
                SDL_SetRenderDrawColor( theRenderer, 0x00, 0x00, 0x00, 0xFF );
                //SDL_RenderDrawLine( theRenderer, thePlayer.posx+32- distanceBetween* sin(thePlayer.rotation* 0.01745329251) ,thePlayer.posy+32+ distanceBetween*cos(thePlayer.rotation* 0.0174532925),thePlayer.posx+32- distanceBetween* sin(fovright*0.01745329251) ,thePlayer.posy+32+ distanceBetween*cos(fovright*0.0174532925) );
                //SDL_RenderDrawLine( theRenderer, thePlayer.posx+32- distanceBetween* sin(thePlayer.rotation* 0.01745329251) ,thePlayer.posy+32+ distanceBetween*cos(thePlayer.rotation* 0.0174532925),thePlayer.posx+32- distanceBetween* sin(fovleft*0.01745329251) ,thePlayer.posy+32+ distanceBetween*cos(fovleft*0.0174532925) );
                //SDL_RenderDrawLine( theRenderer, thePlayer.posx+32- distanceBetween* sin(thePlayer.rotation* 0.01745329251) ,thePlayer.posy+32+ distanceBetween*cos(thePlayer.rotation* 0.0174532925),worldWalls[i]posx,worldWalls[i]posy );

                double lengthToLeft1=sqrt(pow(thePlayer.posx+32- worldWalls[i].dist1* sin(fovleft*0.01745329251)-(thePlayer.posx+32- worldWalls[i].dist1* sin(thePlayer.rotation* 0.01745329251)),2)+pow(thePlayer.posy+32+ worldWalls[i].dist1*cos(fovleft*0.0174532925)-(thePlayer.posy+32+ worldWalls[i].dist1*cos(thePlayer.rotation* 0.0174532925)),2));
                double lengthToLeft2=sqrt(pow(thePlayer.posx+32- worldWalls[i].dist2* sin(fovleft*0.01745329251)-(thePlayer.posx+32- worldWalls[i].dist2* sin(thePlayer.rotation* 0.01745329251)),2)+pow(thePlayer.posy+32+ worldWalls[i].dist2*cos(fovleft*0.0174532925)-(thePlayer.posy+32+ worldWalls[i].dist2*cos(thePlayer.rotation* 0.0174532925)),2));

                double distanceToObject1=sqrt(pow( thePlayer.posx+32- worldWalls[i].dist1* sin(thePlayer.rotation* 0.01745329251)-worldWalls[i].posx1,2)+pow(thePlayer.posy+32+ worldWalls[i].dist1*cos(thePlayer.rotation* 0.0174532925)-worldWalls[i].posy1,2));
                double distanceToObject2=sqrt(pow( thePlayer.posx+32- worldWalls[i].dist2* sin(thePlayer.rotation* 0.01745329251)-worldWalls[i].posx2,2)+pow(thePlayer.posy+32+ worldWalls[i].dist2*cos(thePlayer.rotation* 0.0174532925)-worldWalls[i].posy2,2));

                //cout<<"distanceLeft: "<<worldWalls[i].dist1<<"distanceObject:"<<distanceToObject1<<"\n";
                if((distanceToObject1<lengthToLeft1||distanceToObject2<lengthToLeft2))
                    worldWalls[i].renderThis=true;
                    else
                    {
                        double leftToP1 = sqrt(pow(thePlayer.posx+32- worldWalls[i].dist1* sin(fovleft*0.01745329251)-(worldWalls[i].posx1),2)+pow(thePlayer.posy+32+ worldWalls[i].dist1*cos(fovleft*0.0174532925)-worldWalls[i].posy1,2));
                        double leftToP2 = sqrt(pow(thePlayer.posx+32- worldWalls[i].dist2* sin(fovleft*0.01745329251)-(worldWalls[i].posx2),2)+pow(thePlayer.posy+32+ worldWalls[i].dist2*cos(fovleft*0.0174532925)-worldWalls[i].posy2,2));
                        double rightToP1 = sqrt(pow(thePlayer.posx+32- worldWalls[i].dist1* sin(fovright*0.01745329251)-(worldWalls[i].posx1),2)+pow(thePlayer.posy+32+ worldWalls[i].dist1*cos(fovright*0.0174532925)-worldWalls[i].posy1,2));
                        double rightToP2 = sqrt(pow(thePlayer.posx+32- worldWalls[i].dist2* sin(fovright*0.01745329251)-(worldWalls[i].posx2),2)+pow(thePlayer.posy+32+ worldWalls[i].dist2*cos(fovright*0.0174532925)-worldWalls[i].posy2,2));

                    cout<<leftToP1<<" "<<leftToP2<<" "<<rightToP1<<" "<<rightToP2<<" "<<distanceToObject1<<" "<<distanceToObject2<< "\n";

                    bool faulty=false;

                        if(leftToP1<rightToP1)
                            if(leftToP1<=distanceToObject1/2)
                            worldWalls[i].p1closerToLeft=true;
                            else
                            faulty=true;

                        if(leftToP1>rightToP1)
                            if(rightToP1>=distanceToObject1/2)
                            faulty=true;

                        if(leftToP2<rightToP2)
                            if(leftToP2<=distanceToObject2/2)
                            worldWalls[i].p2closerToLeft=true;
                            else
                            faulty=true;

                        if(leftToP2>rightToP2)
                            if(rightToP2>=distanceToObject2/2)
                            faulty=true;

                        if(!faulty)
                        if(worldWalls[i].p1closerToLeft!=worldWalls[i].p2closerToLeft)
                            worldWalls[i].renderThis=true;
                    }
                //cout<<"dist1:"<<worldWalls[i].dist1<<" dist2:"<<worldWalls[i].dist2<<"/n";
                //dottest.render( worldWalls[i]posx, worldWalls[i]posy );

                    if(worldWalls[i].renderThis==true)
                    {
                    SDL_RenderDrawLine(theRenderer,worldWalls[i].posx1,worldWalls[i].posy1,worldWalls[i].posx2,worldWalls[i].posy2);
                    worldWalls[i].renderThis=false;
                    worldWalls[i].p1closerToLeft=false;
                    worldWalls[i].p2closerToLeft=false;
                    }
                }

				SDL_RenderPresent( theRenderer );

				//thePlayer.posx++;
				//thePlayer.posy++;
            }
    }
    CloseAll();
    return 0;
}
