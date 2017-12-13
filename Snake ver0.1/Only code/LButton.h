#pragma once

#ifndef _SDL_H
#include <SDL.h>
#endif
#ifndef _SDL_IMAGE_H
#include <SDL_image.h>
#endif


#include "LTexture.h"

enum LButtonSprite
{
	BUTTON_SPRITE_MOUSE_OUT = 0,
	BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
	BUTTON_SPRITE_MOUSE_DOWN = 2,
	BUTTON_SPRITE_MOUSE_UP = 3,
	BUTTON_SPRITE_TOTAL = 4
};

class LButton
{
public:
	//Initializes internal variables
	LButton();

	~LButton();

	//Sets top left position
	void setPosition(int x, int y);

	//Sets bottom size
	void setButtonSize(int w, int h);

	//Gets button sprite
	int getButtonSprite();

	//Handle mouse event
	void handleEvent(SDL_Event *e);

	//Shows button sprite
	void render(SDL_Renderer *gRenderer, LTexture &gButtonSpriteSheetTexture, SDL_Rect *gSpriteClips);

private:
	//Top left position
	SDL_Point mPosition;

	//Button size
	int ButtonWidth, ButtonHeight;

	//Currently used global sprite
	LButtonSprite mCurrentSprite;

};

inline LButton::LButton()
{
	mPosition.x = 0;
	mPosition.y = 0;
	ButtonWidth = 0;
	ButtonHeight = 0;
	
	mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
}

inline LButton::~LButton()
{
}

inline void LButton::setPosition(int x, int y)
{
	mPosition.x = x;
	mPosition.y = y;
}

inline void LButton::setButtonSize(int w, int h)
{
	ButtonWidth = w;
	ButtonHeight = h;
}

inline int LButton::getButtonSprite()
{
	return mCurrentSprite;
}

inline void LButton::handleEvent(SDL_Event * e)
{
	//If mouse event happened
	if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)
	{
		//Get mouse position
		int x, y;
		SDL_GetMouseState(&x, &y);

		//Check if mouse is in button
		bool inside = true;

		if (x < mPosition.x)
			inside = false;
		else if (x > mPosition.x + ButtonWidth)
			inside = false;
		else if (y < mPosition.y)
			inside = false;
		else if (y > mPosition.y + ButtonHeight)
			inside = false;
		
		if (!inside)
		{
			mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
		}
		else
		{
			//Set mouse overt sprite
			switch (e->type)
			{
			case SDL_MOUSEMOTION:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
				break;
			case SDL_MOUSEBUTTONDOWN:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
				break;
			case SDL_MOUSEBUTTONUP:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_UP;
				break;
			}
		}
	}
	

}

inline void LButton::render(SDL_Renderer *gRenderer, LTexture &gButtonSpriteSheetTexture,SDL_Rect *gSpriteClips)
{
	gButtonSpriteSheetTexture.render(gRenderer, mPosition.x, mPosition.y, &gSpriteClips[mCurrentSprite]);
}
