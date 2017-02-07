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

#ifndef CSPRITE_H
#define CSPRITE_H

#include <SDL/SDL.h>

class cSprite
{
public:
	cSprite();
	~cSprite();

	bool 	loadBMP(char *path);
	void 	render(SDL_Surface *screen,int x,int y);
	void 	setColorKey(int r,int g,int b);

	int		getwidth();
	int		getheight();
private:
	SDL_Surface *image;
};
#endif
