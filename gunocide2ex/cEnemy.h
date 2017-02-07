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

#ifndef CENEMY_H
#define CENEMY_H

#include <SDL/SDL.h>
#include "cSpriteAnim.h"

class cEnemy
{
public:
	cEnemy();
	~cEnemy();

	void	setanim(cSpriteAnim *anim);
	cAnim*	getanim();

	void	setposx(int wert);
	int		getposx();

	void	setposy(int wert);
	int		getposy();

	void	setsprite(int wert);
	int		getsprite();

	void	setpowerup(int wert);
	int		getpowerup();

	void	setalive(bool wert);
	bool	getalive();

	bool	getshot();
	void	setshot(bool wert);

	void	setenergy(int wert);
	int		getenergy();

	int		getwidth(cSpriteAnim *anim);
	int		getheight(cSpriteAnim *anim);

	void		setdelayshot(long int wert);
	long int	getdelayshot();

	SDL_Rect 	getcolrect(cSpriteAnim *anim,int offx,int offy);

	void	update();
	void	render(SDL_Surface *screen,cSpriteAnim *anim);
private:
	cAnim		anim_sprite;

	int			movex;
	int			posx,posy;
	int			attack_posx,attack_posy;
	int			typ_sprite;
	int			typ_powerup;
	int			energy;
	long int	delay_shot;
	long int	delay_start;
	bool		balive;
	bool		bshot;
	bool		bmleft;
	bool		battack;
	bool		battack_moveback;
	int			playertarget;
};
#endif
