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

const int playerHalf=32;
const int cameraOffset=16;

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

int AVAILABLE_WALLS=1;

wallType worldWalls[50];

int main(int argc, char* args[])
{
    thePlayer.posx=200;
    thePlayer.posy=200;

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


				int playerCenterX=thePlayer.posx+playerHalf;
				int playerCenterY=thePlayer.posy+playerHalf;
				float cameraX=playerCenterX-cameraOffset*sin(thePlayer.rotation*0.01745329251);
				float cameraY=playerCenterY+cameraOffset*cos(thePlayer.rotation*0.01745329251);


				SDL_SetRenderDrawColor( theRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				SDL_RenderClear( theRenderer );
				SDL_SetRenderDrawColor( theRenderer, 0, 0, 0, 0 );

				SDL_RenderClear( theRenderer );

				debug_topdownPlayer.render(thePlayer.posx,thePlayer.posy,NULL,thePlayer.rotation);
				//walltest.render(walltestposx,walltestposy);

				SDL_SetRenderDrawColor( theRenderer, 255, 255, 255, 0xFF );

                SDL_Point a = {1, 2};
                SDL_Point* b=&a;
                //angletest.render(cameraX, cameraY, NULL,thePlayer.rotation+45,b);



                double fovleft=thePlayer.rotation-thePlayer.fov/2;
                double fovright=thePlayer.rotation+thePlayer.fov/2;

                if(fovleft<0)
                    fovleft+=359;
                if(fovright>359)
                    fovright-=359;

                //if(walltestposx<cameraX- thePlayer.visionLength* sin(fovleft*0.01745329251))
                //if(walltestposy<cameraY- thePlayer.visionLength* cos(fovright*0.01745329251))
                    //walltest.render(walltestposx,walltestposy);}
                    //SDL_RenderDrawLine( theRenderer,walltestposx,walltestposy,walltestposz,walltestposh);}
                     //cout<<walltestposx<<" compared to "<<cameraX- thePlayer.visionLength* sin(fovleft*0.01745329251)<<"\n";


                for(int i=0;i<AVAILABLE_WALLS;i++)
                {

                float wallLength=sqrt(pow(worldWalls[i].posx2-worldWalls[i].posx1,2)+pow(worldWalls[i].posy2-worldWalls[i].posy1,2));

                double distanceBetween = sqrt(pow(cameraX-worldWalls[i].posx1,2)+pow(cameraY-worldWalls[i].posy1,2));

                worldWalls[i].dist1 = distanceBetween;
                worldWalls[i].dist2 = sqrt(pow(cameraX-worldWalls[i].posx2,2)+pow(cameraY-worldWalls[i].posy2,2));


                //SDL_RenderDrawLine( theRenderer, cameraX, cameraY,cameraX- distanceBetween* sin(thePlayer.rotation* 0.01745329251) ,cameraY+ distanceBetween*cos(thePlayer.rotation* 0.0174532925) );

                SDL_RenderDrawLine( theRenderer, cameraX- distanceBetween* sin(thePlayer.rotation* 0.01745329251) ,cameraY+ distanceBetween*cos(thePlayer.rotation* 0.0174532925),cameraX- distanceBetween* sin(fovright*0.01745329251) ,cameraY+ distanceBetween*cos(fovright*0.0174532925) );
                SDL_RenderDrawLine( theRenderer, cameraX- distanceBetween* sin(thePlayer.rotation* 0.01745329251) ,cameraY+ distanceBetween*cos(thePlayer.rotation* 0.0174532925),cameraX- distanceBetween* sin(fovleft*0.01745329251) ,cameraY+ distanceBetween*cos(fovleft*0.0174532925) );
                //SDL_RenderDrawLine( theRenderer, cameraX- distanceBetween* sin(thePlayer.rotation* 0.01745329251) ,cameraY+ distanceBetween*cos(thePlayer.rotation* 0.0174532925),worldWalls[i].posx1,worldWalls[i].posy1 );

                double lengthToLeft1=sqrt(pow(cameraX- worldWalls[i].dist1* sin(fovleft*0.01745329251)-(cameraX- worldWalls[i].dist1* sin(thePlayer.rotation* 0.01745329251)),2)+pow(cameraY+ worldWalls[i].dist1*cos(fovleft*0.0174532925)-(cameraY+ worldWalls[i].dist1*cos(thePlayer.rotation* 0.0174532925)),2));
                double lengthToLeft2=sqrt(pow(cameraX- worldWalls[i].dist2* sin(fovleft*0.01745329251)-(cameraX- worldWalls[i].dist2* sin(thePlayer.rotation* 0.01745329251)),2)+pow(cameraY+ worldWalls[i].dist2*cos(fovleft*0.0174532925)-(cameraY+ worldWalls[i].dist2*cos(thePlayer.rotation* 0.0174532925)),2));



                double distanceToObject1=sqrt(pow( cameraX- worldWalls[i].dist1* sin(thePlayer.rotation* 0.01745329251)-worldWalls[i].posx1,2)+pow(cameraY+ worldWalls[i].dist1*cos(thePlayer.rotation* 0.0174532925)-worldWalls[i].posy1,2));
                double distanceToObject2=sqrt(pow( cameraX- worldWalls[i].dist2* sin(thePlayer.rotation* 0.01745329251)-worldWalls[i].posx2,2)+pow(cameraY+ worldWalls[i].dist2*cos(thePlayer.rotation* 0.0174532925)-worldWalls[i].posy2,2));

                float planeSize=sqrt(pow(cameraX- max(worldWalls[i].dist1,worldWalls[i].dist2)* sin(fovleft*0.01745329251)-(cameraX- max(worldWalls[i].dist1,worldWalls[i].dist2)* sin(fovright*0.01745329251)),2)+pow(cameraY+  max(worldWalls[i].dist1,worldWalls[i].dist2)*cos(fovleft*0.0174532925)-(cameraY+  max(worldWalls[i].dist1,worldWalls[i].dist2)*cos(fovright*0.0174532925)),2));
                //cout<<"distanceLeft: "<<worldWalls[i].dist1<<"distanceObject:"<<distanceToObject1<<"\n";

                if(lengthToLeft1+distanceToObject1>planeSize)
                    cout<<"output";
                if((distanceToObject1<lengthToLeft1||distanceToObject2<lengthToLeft2))
                    {
                        worldWalls[i].renderThis=true;
                        if(distanceToObject1<lengthToLeft1)
                            worldWalls[i].edge2OutsideFov=true;
                            else
                        if(distanceToObject2<lengthToLeft2)
                            worldWalls[i].edge1OutsideFov=true;
                        if((distanceToObject1<lengthToLeft1&&distanceToObject2<lengthToLeft2))
                        {
                            worldWalls[i].edge2OutsideFov=false;
                            worldWalls[i].edge1OutsideFov=false;
                        }
                        worldWalls[i].bothSeen=true;

                    }
                    else
                    {

                        double leftToP1 = sqrt(pow(cameraX- worldWalls[i].dist1* sin(fovleft*0.01745329251)-(worldWalls[i].posx1),2)+pow(cameraY+ worldWalls[i].dist1*cos(fovleft*0.0174532925)-worldWalls[i].posy1,2));
                        double leftToP2 = sqrt(pow(cameraX- worldWalls[i].dist2* sin(fovleft*0.01745329251)-(worldWalls[i].posx2),2)+pow(cameraY+ worldWalls[i].dist2*cos(fovleft*0.0174532925)-worldWalls[i].posy2,2));
                        double rightToP1 = sqrt(pow(cameraX- worldWalls[i].dist1* sin(fovright*0.01745329251)-(worldWalls[i].posx1),2)+pow(cameraY+ worldWalls[i].dist1*cos(fovright*0.0174532925)-worldWalls[i].posy1,2));
                        double rightToP2 = sqrt(pow(cameraX- worldWalls[i].dist2* sin(fovright*0.01745329251)-(worldWalls[i].posx2),2)+pow(cameraY+ worldWalls[i].dist2*cos(fovright*0.0174532925)-worldWalls[i].posy2,2));

                    //cout<<leftToP1<<" "<<leftToP2<<" "<<rightToP1<<" "<<rightToP2<<" "<<distanceToObject1<<" "<<distanceToObject2<< "\n";

                    bool faulty=false;
                    int divider=2;

                        if(leftToP1<rightToP1)
                            if(leftToP1<=distanceToObject1/divider)
                            worldWalls[i].p1closerToLeft=true;
                            else
                            faulty=true;

                        if(leftToP1>rightToP1)
                            if(rightToP1>=distanceToObject1/divider)
                            faulty=true;

                        if(leftToP2<rightToP2)
                            if(leftToP2<=distanceToObject2/divider)
                            worldWalls[i].p2closerToLeft=true;
                            else
                            faulty=true;

                        if(leftToP2>rightToP2)
                            if(rightToP2>=distanceToObject2/divider)
                            faulty=true;

                        if(!faulty)
                        {
                            if(worldWalls[i].p1closerToLeft!=worldWalls[i].p2closerToLeft)
                            {
                                worldWalls[i].renderThis=true;
                                worldWalls[i].edge1OutsideFov=true;
                                worldWalls[i].edge2OutsideFov=true;
                            }
                        }
                        //worldWalls[i].renderThis=false;
                    }

                    if(worldWalls[i].renderThis==true)
                    {
                    SDL_RenderDrawLine(theRenderer,worldWalls[i].posx1,worldWalls[i].posy1,worldWalls[i].posx2,worldWalls[i].posy2);
                    worldWalls[i].renderThis=false;
                    worldWalls[i].p1closerToLeft=false;
                    worldWalls[i].p2closerToLeft=false;



                    float planeSize2=sqrt(pow(cameraX- worldWalls[i].dist2* sin(fovleft*0.01745329251)-(cameraX- worldWalls[i].dist2* sin(fovright*0.01745329251)),2)+pow(cameraY+ worldWalls[i].dist2*cos(fovleft*0.0174532925)-(cameraY+ worldWalls[i].dist2*cos(fovright*0.0174532925)),2));

                    float planeHeight = planeSize*SCREEN_HEIGHT/SCREEN_WIDTH;
                    float planeHeight2 = planeSize2*SCREEN_HEIGHT/SCREEN_WIDTH;

                    float distFromLeft1 = sqrt(pow(cameraX- worldWalls[i].dist1* sin(fovleft*0.01745329251)-worldWalls[i].posx1,2)+pow(cameraY+ worldWalls[i].dist1*cos(fovleft*0.0174532925)-worldWalls[i].posy1,2));
                    float distFromLeft2 = sqrt(pow(cameraX- worldWalls[i].dist2* sin(fovleft*0.01745329251)-worldWalls[i].posx2,2)+pow(cameraY+ worldWalls[i].dist2*cos(fovleft*0.0174532925)-worldWalls[i].posy2,2));




                    if(worldWalls[i].edge1OutsideFov!=worldWalls[i].edge2OutsideFov)
                    {
                        if(worldWalls[i].edge1OutsideFov)
                        {
                        distFromLeft1=-abs(distFromLeft1);
                        }


                    }
                    else
                    if(worldWalls[i].edge1OutsideFov&&worldWalls[i].edge2OutsideFov)
                    {
                        cout<<"kebab";
                        distFromLeft1=-abs(distFromLeft1);
                    }

                    distFromLeft2=distFromLeft2/planeSize2*planeSize;

                    float raportMinDist=worldWalls[i].dist1/(worldWalls[i].dist1+worldWalls[i].dist2);
                    float nearPosition= wallLength*raportMinDist;



                    float distToMin=distFromLeft1+ raportMinDist*abs(distFromLeft2-distFromLeft1);

                    float minDist=sqrt(abs(pow(worldWalls[i].dist1,2)-pow(nearPosition,2)));

                    bool ignoreMinimalCheck=false;

                    if(worldWalls[i].dist1<minDist)
                    {
                        minDist=worldWalls[i].dist1;
                        distToMin=distFromLeft1;
                        ignoreMinimalCheck=true;
                    }
                    else
                    if(worldWalls[i].dist2<minDist)
                    {
                        minDist=worldWalls[i].dist2;
                        distToMin=distFromLeft2;
                        ignoreMinimalCheck=true;
                    }


                    //cout<<"on wall "<<i<<" the min point is "<<nearPosition<<" out of "<<wallLength<<"\n";



                    //cout<<"the min distance is "<<minDist<<" compared to "<<worldWalls[i].dist1<<" and "<<worldWalls[i].dist2<<"\n";
                    //cout<<"screen pos is "<<distToMin<<" out of "<<distFromLeft1<<" and "<<distFromLeft2<<"\n";
                    //
                    //float heightDifference=worldWalls[i].dist1/worldWalls[i].dist2;
                    float sizeOfPixel=SCREEN_WIDTH/planeSize;

                    float heightOfPixel=SCREEN_HEIGHT/worldWalls[i].dist1;

                    float theBaseHeight1=worldWalls[i].dist1;
                    float theBaseHeight2=worldWalls[i].dist2;

                    int numberOfPixels=abs(distFromLeft1-distFromLeft2)*sizeOfPixel;


                    float baseHeight=planeHeight/min(worldWalls[i].dist1,worldWalls[i].dist2);

                    int incrementor=0;
                    for(float j=distFromLeft2*sizeOfPixel;j>distFromLeft1*sizeOfPixel;j-=sizeOfPixel)
                    {
                        incrementor++;

                    }
                    numberOfPixels=incrementor;

                    incrementor=0;
                    for(float j=distFromLeft2*sizeOfPixel;j>distToMin*sizeOfPixel;j-=sizeOfPixel)
                    {
                        incrementor++;
                    }
                    int numberOfPixels1=incrementor;

                    incrementor=0;
                    for(float j=distToMin*sizeOfPixel;j>distFromLeft1*sizeOfPixel;j-=sizeOfPixel)
                    {
                        incrementor++;
                    }
                    int numberOfPixels2=incrementor;

                    bool flipPixelSizeSign1=false;
                    bool flipPixelSizeSign2=false;

                    if(worldWalls[i].edge2OutsideFov&&!worldWalls[i].edge1OutsideFov)
                    {
                        float playerTailDist2 = sqrt(pow(cameraX- worldWalls[i].dist2* sin((thePlayer.rotation+90)*0.01745329251)-(worldWalls[i].posx2),2)+pow(cameraY+ worldWalls[i].dist2*cos((thePlayer.rotation+90)*0.0174532925)-worldWalls[i].posy2,2));
                        //cout<<"for wall "<<i<< "original="<<worldWalls[i].dist2<<" tail="<<playerTailDist2<<"\n";
                        if(worldWalls[i].dist2>playerTailDist2)
                            flipPixelSizeSign1=true;
                    }

                    if(worldWalls[i].edge1OutsideFov&&!worldWalls[i].edge2OutsideFov)
                    {
                        float playerTailDist1 = sqrt(pow(cameraX- worldWalls[i].dist1* sin((thePlayer.rotation-90)*0.01745329251)-(worldWalls[i].posx1),2)+pow(cameraY+ worldWalls[i].dist1*cos((thePlayer.rotation-90)*0.0174532925)-worldWalls[i].posy1,2));
                        //cout<<"for wall "<<i<< "original="<<worldWalls[i].dist2<<" tail="<<playerTailDist2<<"\n";
                        if(worldWalls[i].dist1>playerTailDist1)
                            flipPixelSizeSign2=true;
                    }

                    float distFromMiddleToWall1=sqrt(pow(cameraX+worldWalls[i].dist1*sin(thePlayer.rotation* 0.01745329251)-worldWalls[i].posx1,2)+pow(cameraY+worldWalls[i].dist1*cos(thePlayer.rotation* 0.01745329251)-worldWalls[i].posy1,2))/planeSize*100;
                    float distFromMiddleToWall2=sqrt(pow(cameraX+worldWalls[i].dist2*sin(thePlayer.rotation* 0.01745329251)-worldWalls[i].posx2,2)+pow(cameraY+worldWalls[i].dist2*cos(thePlayer.rotation* 0.01745329251)-worldWalls[i].posy2,2))/planeSize2*100;

                    //cout<<worldWalls[i].dist1<<" "<<distFromMiddleToWall1<<"\n";
                    //worldWalls[i].dist1-=distFromMiddleToWall1;
                    //worldWalls[i].dist2-=distFromMiddleToWall2;
                    /*
                    if(worldWalls[i].edge2OutsideFov&&!worldWalls[i].edge1OutsideFov)
                        worldWalls[i].dist2-=(distFromLeft2-planeSize);

                    if(!worldWalls[i].edge1OutsideFov&&worldWalls[i].edge2OutsideFov)
                        worldWalls[i].dist1+=distFromLeft1/2;
                    */

                    /*if(flipPixelSizeSign2)
                        if(scaledSize2>scaledSize1)
                        scaledSize1=scaledSize2+(scaledSize2-scaledSize1);*/

                    float scaledSize1=SCREEN_HEIGHT/worldWalls[i].dist1*SCREEN_HEIGHT/10;
                    float scaledSize2=SCREEN_HEIGHT/worldWalls[i].dist2*SCREEN_HEIGHT/10;
                    //float scaledSizeMid=SCREEN_HEIGHT/minDist*SCREEN_HEIGHT/10;



                    /*if(scaledSizeMid>max(scaledSize1,scaledSize2))
                        scaledSizeMid=max(scaledSize1,scaledSize2)+max(scaledSize1,scaledSize2)/10;*/

                    //cout<<"scaled size is "<<scaledSizeMid<<" out of "<<scaledSize1<<" and "<<scaledSize2<<"\n";


                     //should fix points outside the fov and behind the player




                    //int numberOfPixels1=abs(distFromLeft1-distToMin)*sizeOfPixel;
                    //int numberOfPixels2=abs(distFromLeft2-distToMin)*sizeOfPixel;
                    //int numberOfPixels2=numberOfPixels-numberOfPixels1;
                    //cout<<"nr of pixels: "<<numberOfPixels1<<" "<<numberOfPixels2<<"\n";

                    float scaledDifference=abs(scaledSize1-scaledSize2);
                    //float scaledDifference2=abs(scaledSizeMid-scaledSize2);
                    //cout<<scaledDifference1-scaledDifference2<<"\n";
                    float theVerticalPixelDifference=abs(scaledDifference/numberOfPixels);
                    //float theVerticalPixelDifference2=abs(scaledDifference2/numberOfPixels2);
                            //worldWalls[i].dist2=-worldWalls[i].dist2;

                            //cout<<theVerticalPixelDifference1<<" vs "<<theVerticalPixelDifference2<<"\n";
                            //cout<<numberOfPixels1<<" "<<numberOfPixels2<<" "<<numberOfPixels1+numberOfPixels2<<" = "<<numberOfPixels<<"\n";

                    float degreePerPixel=(thePlayer.fov)/planeSize;
                    float firstHalfFovSize=0;
                    float secondHalfFovSize=0;

                    if(max(distFromLeft1,distFromLeft2)>planeSize/2)
                        firstHalfFovSize=degreePerPixel/(min(max(distFromLeft1,distFromLeft2),planeSize)-planeSize/2)*sizeOfPixel;
                    if(min(distFromLeft1,distFromLeft2)<planeSize/2)
                        secondHalfFovSize=degreePerPixel/(max(min(distFromLeft1,distFromLeft2),(float)1)*sizeOfPixel);
                    //cout<<degreePerPixel<<" "<<firstHalfFovSize<<" "<<secondHalfFovSize<<"\n";

                    incrementor=0;
                    bool changedOnce=false;
                    for(float j=distFromLeft2*sizeOfPixel;j>(distFromLeft1-1)*sizeOfPixel;j-=sizeOfPixel)
                    {
                        float heightFinal;

                        if(scaledSize1>scaledSize2)
                            heightFinal=min(scaledSize1,scaledSize2)+abs(theVerticalPixelDifference)*incrementor;
                        else
                            heightFinal=max(scaledSize1,scaledSize2)-abs(theVerticalPixelDifference)*incrementor;


                        int addAngle;
                        if(j<planeSize*sizeOfPixel/2)
                        addAngle=secondHalfFovSize*incrementor;
                        else
                        addAngle=firstHalfFovSize*((numberOfPixels/2-incrementor));


                        SDL_Rect fillRect = { j,SCREEN_HEIGHT/2- ( heightFinal)/2 , ceil(sizeOfPixel), heightFinal};
                        SDL_SetRenderDrawColor( theRenderer, i*40, 20, 80, 255 );
                        SDL_SetRenderDrawBlendMode(theRenderer, SDL_BLENDMODE_BLEND);
                        SDL_RenderFillRect( theRenderer, &fillRect );

                        incrementor++;
                    }


                    cout<<worldWalls[i].edge1OutsideFov<<" "<<worldWalls[i].edge2OutsideFov<<"\n";
                    //cout<<"size of minPoint: "<<scaledSizeMid<<" out of "<<scaledSize1<<" and "<<scaledSize2<<"\n";
                    worldWalls[i].edge1OutsideFov=false;
                    worldWalls[i].edge2OutsideFov=false;
                    worldWalls[i].bothSeen=false;

                    }
                SDL_RenderDrawLine( theRenderer, cameraX, cameraY,cameraX- thePlayer.visionLength* sin(fovleft*0.01745329251) ,cameraY+ thePlayer.visionLength*cos(fovleft*0.0174532925) );
                SDL_RenderDrawLine( theRenderer, cameraX, cameraY,cameraX- thePlayer.visionLength* sin(fovright*0.01745329251) ,cameraY+ thePlayer.visionLength*cos(fovright*0.0174532925) );

                SDL_RenderDrawLine( theRenderer, playerCenterX, playerCenterY,playerCenterX- thePlayer.visionLength* sin(fovleft*0.01745329251) ,playerCenterY+ thePlayer.visionLength*cos(fovleft*0.0174532925) );
                SDL_RenderDrawLine( theRenderer, playerCenterX, playerCenterY,playerCenterX- thePlayer.visionLength* sin(fovright*0.01745329251) ,playerCenterY+ thePlayer.visionLength*cos(fovright*0.0174532925) );

                }
				SDL_RenderPresent( theRenderer );
            }
    }
    CloseAll();
    return 0;
}
