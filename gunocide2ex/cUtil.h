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

#ifndef CUTIL_H
#define CUTIL_H

#include <SDL/SDL.h>
#include <SDL/SDL_endian.h>
#include <memory.h>
#include <string.h>

class cUtil
{
public:
	cUtil();
	~cUtil();

	void	updatetime();
	float 	sync(float Faktor);
	char*	getconfigvalue(char *path,char *value);
	void	drawpixel(SDL_Surface *screen, int x, int y,Uint8 R, Uint8 G,Uint8 B);
	bool	collision(SDL_Rect objekt1,SDL_Rect objekt2);	

	static cUtil* 	getInstance();
	static void		delInstance();
private:
	static cUtil	*instance;

	long int	LastSyncTime;
	long int	SyncTime;
	long int	SyncDiff;
	long int	PassedTime;
};
#endif
