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

#ifndef CENEMYMAN_H
#define CENEMYMAN_H

#include <SDL/SDL.h>
#include "cSpriteAnim.h"

#define MAX_ENEMIES		30

#define ENEMY_ENFORCER		0
#define ENEMY_GUARDIAN		1
#define ENEMY_NEMESIS		2
#define ENEMY_STORMLORD		3
#define ENEMY_BULLDOG		4
#define ENEMY_WIDOWMAKER	5
#define ENEMY_BLACKKNIGHT	6

class cEnemy;

class EnemyVars
{
public:
	static int Energy[7];
	static int Score[7];
};

class cEnemyMan
{
public:
	cEnemyMan();
	~cEnemyMan();

	bool		load();
 	bool		create();
	bool		destroy();
	void		add(int index,int x,int y,int typ,int pwuptyp);
	void		update();
	void		render(SDL_Surface *screen);
	cEnemy*		getenemy(int index);
	SDL_Rect	getcolrect(int index);

	bool		checkenemies();
	bool		getenemyattack();
	void		setenemyattack(bool wert);

	static cEnemyMan*	getInstance();
	static void			delInstance();
private:
 	static cEnemyMan*	instance;

	cSpriteAnim	 		gfx_sprite[7];
	cEnemy				*pEnemy;
	bool				benemyattack;
	long int			enemyattack_delay;

};
#endif
