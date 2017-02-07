/***************************************************************************
                          cEffects.cpp  -  description
                             -------------------
    begin                : Tue Nov 20 2001
    copyright            : (C) 2001 by Alexander Bierbrauer
    email                : alex@polyfrag.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "cSprite.h"
#include "mmgr.h"

cSprite::cSprite()
{
	image=NULL;
}

cSprite::~cSprite()
{
}

///lädt ein BMP in das image-Surface
///gibt false zurück falls es schief geht
bool cSprite::loadBMP(char *path)
{
	image=SDL_LoadBMP(path);
	if(!image)
	{
		printf("failed to load sprite %s\n",path);
		return false;
	}
	
	image=SDL_DisplayFormat(image);
	printf("loaded sprite %s\n",path);
	return true;
}

///rendert das Sprite auf dem Bildschirm
///an den Koordinaten x und y
void cSprite::render(SDL_Surface *screen, int x, int y)
{
	SDL_Rect rc;

	rc.x=x;
	rc.y=y;
	rc.w=image->w;
	rc.h=image->h;

	SDL_BlitSurface(image,NULL,screen,&rc);
}

///setzt den Colorkey für ein Sprite
void cSprite::setColorKey(int r,int g,int b)
{
	SDL_SetColorKey(image,SDL_SRCCOLORKEY,SDL_MapRGB(image->format,r,g,b));
}

///gibt die Breite des Sprites zurück
int cSprite::getwidth()
{
	return image->w;
}

///gibt die Höhe des Sprites zurück
int cSprite::getheight()
{
	return image->h;
}