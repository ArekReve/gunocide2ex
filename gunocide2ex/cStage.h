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

#ifndef CSTAGE_H
#define CSTAGE_H

#include <SDL/SDL.h>
#include "cSprite.h"

class cEnemyMan;

class cStage
{
public:
	cStage();
	~cStage();

	bool	load(char *path,cEnemyMan *pEnemyMan);
	void	render(SDL_Surface *screen);

	static cStage*	getInstance();
	static void		delInstance();
private:
	static cStage		*instance;
	cSprite				*gfx_background;
};
#endif
