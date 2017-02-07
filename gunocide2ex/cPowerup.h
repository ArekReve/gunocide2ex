/***************************************************************************
                          cPowerup.h  -  description
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
#ifndef CPOWERUP_H
#define CPOWERUP_H

#include "cSpriteAnim.h"
#include <list>
#include <memory.h>

#define POWERUP_GUN		1
#define POWERUP_LIFE	4
#define POWERUP_SCORE	3
#define POWERUP_SPECIAL	2

using namespace std;

class cPowerup
{
public:
	cPowerup();
	~cPowerup();

	void	setposx(int x);
	int		getposx();
	void	setposy(int y);
	int		getposy();
	void	setsprite(cSpriteAnim *anim);
	cAnim*	getsprite();
	void	settype(int type);
	int		gettype();

	void		update();

	void		render(SDL_Surface *Screen,cSpriteAnim *anim);
	SDL_Rect 	getcolrect(cSpriteAnim *anim,int offx,int offy);
private:
	cAnim	gfx_anim;

	int		type_powerup;
	int		posx,posy;
};

typedef list<cPowerup> LISTPOWERUPS;

class cPowerupMan
{
public:
	cPowerupMan();
	~cPowerupMan();

	static cPowerupMan*	getInstance();
	static void			delInstance();

	bool	load();
	void	update();
	void	render(SDL_Surface *screen);

	void	add(int type,int x,int y);
private:
	static cPowerupMan		*instance;

	LISTPOWERUPS		lPowerups;
	cSpriteAnim			gfx_sprite[4];
};
#endif
