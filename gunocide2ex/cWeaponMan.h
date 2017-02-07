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

#ifndef CWEAPONMAN_H
#define CWEAPONMAN_H

#include "cSpriteAnim.h"
#include <list>
#include "cWeapon.h"

#define OWNER_PLAYER1	0
#define OWNER_PLAYER2	1
#define OWNER_ENEMY		2

using namespace std;
typedef list<cWeapon> LISTWEAPON;

class WeaponVars
{
public:
	static int Speed[19];
	static int Energy[23];
};

class cWeaponMan
{
public:
	cWeaponMan();
	~cWeaponMan();

	bool	load();
	void	update();
	void	render(SDL_Surface *screen);
	void	add(int type,int posx,int posy,int dirx,int diry,int owner,int energy);
	
	static cWeaponMan*	getInstance();
	static void			delInstance();
private:
	void	addspecial(int type,int posx,int posy);

	static cWeaponMan	*instance;

	bool			collision(SDL_Rect objekt1,SDL_Rect objekt2);

	cSpriteAnim		gfx_sprite[23];

	LISTWEAPON		lWeapon;
};
#endif
