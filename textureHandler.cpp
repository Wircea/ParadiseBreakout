#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <iostream>

using namespace std;

SDL_Renderer* renderRef;
int* testint;

void display()
{
    cout<<"aas";
}

class textureType
{
	public:
		textureType(){
            theTexture = NULL;
            mWidth = 0;
            mHeight = 0;
		};
		~textureType()
		{
		free();
		};

		bool loadFromFile( std::string path);
		void free();
		void setColor( Uint8 red, Uint8 green, Uint8 blue );
		void setBlendMode( SDL_BlendMode blending );
		void setAlpha( Uint8 alpha );
		void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );
		int getWidth();
		int getHeight();
        SDL_Texture* theTexture;
	private:

		int mWidth;
		int mHeight;
};

void textureType::free()
{
	if( theTexture != NULL )
	{
		SDL_DestroyTexture( theTexture );
		theTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void textureType::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
	SDL_SetTextureColorMod( theTexture, red, green, blue );
}

void textureType::setBlendMode( SDL_BlendMode blending )
{
	SDL_SetTextureBlendMode( theTexture, blending );
}

void textureType::setAlpha( Uint8 alpha )
{
	SDL_SetTextureAlphaMod( theTexture, alpha );
}



int textureType::getWidth()
{
	return mWidth;
}

int textureType::getHeight()
{
	return mHeight;
}

bool textureType::loadFromFile( std::string path)
{
	free();
	SDL_Texture* newTexture = NULL;
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

            newTexture = SDL_CreateTextureFromSurface( renderRef, loadedSurface );

		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}
		SDL_FreeSurface( loadedSurface );
	}
	theTexture = newTexture;
	return theTexture != NULL;
}

void textureType::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };
	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}
	SDL_RenderCopyEx( renderRef, theTexture, clip, &renderQuad, angle, center, flip );
}


