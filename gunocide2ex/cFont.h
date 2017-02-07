/***************************************************************************
                          cFont.h  -  description
                             -------------------
    begin                : Thu Nov 22 2001
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
#ifndef CFONT_H
#define CFONT_H

#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL.h>
#include <memory.h>
#include <stdarg.h>

class cFont
{
public:
	cFont();
	~cFont();

	static cFont*	getInstance();
	static void		delInstance();
	static bool		initttf();

	bool			load(const char *path,int size);
	void			setcolor(int r,int g,int b);
	void			setshader(int r,int g,int b);
	void			print(SDL_Surface *screen,int posx,int posy,const char *fmt, ...);
	void			print_shaded(SDL_Surface *screen,int posx,int posy,const char *fmt, ...);
private:
	static cFont*	instance;

	TTF_Font 		*font;
	SDL_Color		color;
	SDL_Color		shader;
};
#endif

