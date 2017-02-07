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

#ifndef CWEAPON_H
#define CWEAPON_H

#include "cSpriteAnim.h"

class cWeapon
{
public:
	cWeapon();
	~cWeapon();

	void	setposx(int wert);
	int		getposx();

	void	setposy(int wert);
	int		getposy();

	void	setdirx(int wert);
	int		getdirx();

	void	setdiry(int wert);
	int		getdiry();

	void	setsprite(int wert);
	int		getsprite();

	void	setanim(cSpriteAnim *anim);
	cAnim*	getanim();

	void	setowner(int wert);
	int		getowner();

	void	setenergy(int wert);
	int		getenergy();

	void	render(SDL_Surface *screen,cSpriteAnim *anim);
	void	update();

	SDL_Rect	getcolrect(cSpriteAnim *anim,int offx,int offy);
private:
	int		posx,posy;
	int		dirx,diry;

	cAnim	anim_sprite;
	int		typ_sprite;
	int		owner;
	int		energy;
	int		playertarget;
};
#endif
