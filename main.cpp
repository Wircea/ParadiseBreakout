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

int AVAILABLE_WALLS=5;

wallType worldWalls[50];

int main(int argc, char* args[])
{
    thePlayer.posx=SCREEN_WIDTH/2-200;
    thePlayer.posy=SCREEN_HEIGHT/2+100;

    worldWalls[0].posx1=214;
    worldWalls[0].posy1=50;
    worldWalls[0].posx2=582;
    worldWalls[0].posy2=73;

    worldWalls[1].posx1=582;
    worldWalls[1].posy1=73;
    worldWalls[1].posx2=680;
    worldWalls[1].posy2=230;

    worldWalls[2].posx1=680;
    worldWalls[2].posy1=230;
    worldWalls[2].posx2=770;
    worldWalls[2].posy2=320;

    worldWalls[3].posx1=230;
    worldWalls[3].posy1=70;
    worldWalls[3].posx2=400;
    worldWalls[3].posy2=72;

    worldWalls[4].posx1=770;
    worldWalls[4].posy1=320;
    worldWalls[4].posx2=720;
    worldWalls[4].posy2=350;

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
                    if( e.type == SDL_KEYDOWN )
					{
					    switch( e.key.keysym.sym )
						{
							case SDLK_UP:
							    thePlayer.posx += float(-20*cos((double)(thePlayer.rotation-90)*0.0174532925));
                                thePlayer.posy += float(-20*sin((double)(thePlayer.rotation-90)*0.0174532925));
                                break;
                            case SDLK_DOWN:
                                thePlayer.posx -= float(-20*cos((double)(thePlayer.rotation-90)*0.0174532925));
                                thePlayer.posy -= float(-20*sin((double)(thePlayer.rotation-90)*0.0174532925));
                                break;
						}
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

                double fovleft=thePlayer.rotation-thePlayer.fov/2;
                double fovright=thePlayer.rotation+thePlayer.fov/2;

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
                    {
                        worldWalls[i].renderThis=true;
                        if(distanceToObject1<lengthToLeft1)
                            worldWalls[i].edge2OutsideFov=true;
                        if(distanceToObject2<lengthToLeft2)
                            worldWalls[i].edge1OutsideFov=true;
                    }
                    else
                    {
                        double leftToP1 = sqrt(pow(thePlayer.posx+32- worldWalls[i].dist1* sin(fovleft*0.01745329251)-(worldWalls[i].posx1),2)+pow(thePlayer.posy+32+ worldWalls[i].dist1*cos(fovleft*0.0174532925)-worldWalls[i].posy1,2));
                        double leftToP2 = sqrt(pow(thePlayer.posx+32- worldWalls[i].dist2* sin(fovleft*0.01745329251)-(worldWalls[i].posx2),2)+pow(thePlayer.posy+32+ worldWalls[i].dist2*cos(fovleft*0.0174532925)-worldWalls[i].posy2,2));
                        double rightToP1 = sqrt(pow(thePlayer.posx+32- worldWalls[i].dist1* sin(fovright*0.01745329251)-(worldWalls[i].posx1),2)+pow(thePlayer.posy+32+ worldWalls[i].dist1*cos(fovright*0.0174532925)-worldWalls[i].posy1,2));
                        double rightToP2 = sqrt(pow(thePlayer.posx+32- worldWalls[i].dist2* sin(fovright*0.01745329251)-(worldWalls[i].posx2),2)+pow(thePlayer.posy+32+ worldWalls[i].dist2*cos(fovright*0.0174532925)-worldWalls[i].posy2,2));

                    //cout<<leftToP1<<" "<<leftToP2<<" "<<rightToP1<<" "<<rightToP2<<" "<<distanceToObject1<<" "<<distanceToObject2<< "\n";

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
                            {
                                worldWalls[i].renderThis=true;
                                worldWalls[i].edge1OutsideFov=true;
                                worldWalls[i].edge2OutsideFov=true;
                            }
                    }

                    if(worldWalls[i].renderThis==true)
                    {
                    SDL_RenderDrawLine(theRenderer,worldWalls[i].posx1,worldWalls[i].posy1,worldWalls[i].posx2,worldWalls[i].posy2);
                    worldWalls[i].renderThis=false;
                    worldWalls[i].p1closerToLeft=false;
                    worldWalls[i].p2closerToLeft=false;

                    float planeSize=sqrt(pow(thePlayer.posx+32- worldWalls[i].dist1* sin(fovleft*0.01745329251)-(thePlayer.posx+32- worldWalls[i].dist1* sin(fovright*0.01745329251)),2)+pow(thePlayer.posy+32+ worldWalls[i].dist1*cos(fovleft*0.0174532925)-(thePlayer.posy+32+ worldWalls[i].dist1*cos(fovright*0.0174532925)),2));
                    float planeSize2=sqrt(pow(thePlayer.posx+32- worldWalls[i].dist2* sin(fovleft*0.01745329251)-(thePlayer.posx+32- worldWalls[i].dist2* sin(fovright*0.01745329251)),2)+pow(thePlayer.posy+32+ worldWalls[i].dist2*cos(fovleft*0.0174532925)-(thePlayer.posy+32+ worldWalls[i].dist2*cos(fovright*0.0174532925)),2));

                    float planeHeight = planeSize*SCREEN_HEIGHT/SCREEN_WIDTH;
                    float planeHeight2 = planeSize2*SCREEN_HEIGHT/SCREEN_WIDTH;

                    float distFromLeft1 = sqrt(pow(thePlayer.posx+32- worldWalls[i].dist1* sin(fovleft*0.01745329251)-worldWalls[i].posx1,2)+pow(thePlayer.posy+32+ worldWalls[i].dist1*cos(fovleft*0.0174532925)-worldWalls[i].posy1,2));
                    float distFromLeft2 = sqrt(pow(thePlayer.posx+32- worldWalls[i].dist2* sin(fovleft*0.01745329251)-worldWalls[i].posx2,2)+pow(thePlayer.posy+32+ worldWalls[i].dist2*cos(fovleft*0.0174532925)-worldWalls[i].posy2,2));

                    distFromLeft2=distFromLeft2/planeSize2*planeSize;


                    if(worldWalls[i].edge1OutsideFov!=worldWalls[i].edge2OutsideFov)
                    if(worldWalls[i].edge1OutsideFov)
                    {
                        distFromLeft1=-distFromLeft1;
                    }

                    float heightDifference=abs(worldWalls[i].dist1/worldWalls[i].dist2);
                    float sizeOfPixel=SCREEN_WIDTH/planeSize;

                    float heightOfPixel=SCREEN_HEIGHT/worldWalls[i].dist1;

                    float theBaseHeight1=worldWalls[i].dist1;
                    float theBaseHeight2=worldWalls[i].dist2;

                    int numberOfPixels=abs(distFromLeft1*sizeOfPixel/sizeOfPixel-distFromLeft2*sizeOfPixel);

                    float theVerticalPixelDifference;

                    float baseHeight=planeHeight/min(worldWalls[i].dist1,worldWalls[i].dist2);

                    int incrementor=0;

                    for(float j=distFromLeft2*sizeOfPixel;j>distFromLeft1*sizeOfPixel;j-=sizeOfPixel)
                    {
                        incrementor++;

                    }
                    numberOfPixels=incrementor;

                    cout<<incrementor<<"\n";
                    float newDist2=worldWalls[i].dist2/planeSize*planeSize2;


                    float scaledSize1=SCREEN_HEIGHT/worldWalls[i].dist1*SCREEN_HEIGHT/4;
                    float scaledSize2=SCREEN_HEIGHT/worldWalls[i].dist2*SCREEN_HEIGHT/4;
                    float scaledDifference=scaledSize1-scaledSize2;
                    theVerticalPixelDifference=scaledDifference/numberOfPixels;


                    incrementor=0;
                    for(float j=distFromLeft2*sizeOfPixel;j>distFromLeft1*sizeOfPixel;j-=sizeOfPixel)
                    {
                        float heightFinal;



                        if(scaledSize1>scaledSize2)
                            heightFinal=min(scaledSize1,scaledSize2)+abs(theVerticalPixelDifference)*incrementor;
                        else
                            heightFinal=max(scaledSize1,scaledSize2)-abs(theVerticalPixelDifference)*incrementor;

                        SDL_Rect fillRect = { j,SCREEN_HEIGHT/2- ( heightFinal)/2 , ceil(sizeOfPixel), heightFinal};
                        SDL_SetRenderDrawColor( theRenderer, i*40, 20, 80, 255 );
                        SDL_SetRenderDrawBlendMode(theRenderer, SDL_BLENDMODE_BLEND);
                        SDL_RenderFillRect( theRenderer, &fillRect );

                        incrementor++;
                    }
                    worldWalls[i].edge1OutsideFov=false;
                    worldWalls[i].edge2OutsideFov=false;

                    }
                }
				SDL_RenderPresent( theRenderer );
            }
    }
    CloseAll();
    return 0;
}
