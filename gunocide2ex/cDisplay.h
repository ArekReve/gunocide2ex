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

#ifndef CDISPLAY_H
#define CDISPLAY_H

#include "cSprite.h"
#include "cSpriteAnim.h"

class cDisplay
{
public:
	cDisplay();
	~cDisplay();

	void	setleben(int wert);
	int		getleben();

	void	setpowerup(int wert);
	int		getpowerup();

	void	setdelay(int wert);
	int		getdelay();

	void		setscore(long int wert);
	long int	getscore();

	bool	load(int playertyp,int nr);
	void	update();
	void	render(SDL_Surface *screen,int player);
private:
	cSprite		gfx_mg[5];
	cSprite		gfx_score;
	cSprite		gfx_leben;
	cSpriteAnim	gfx_special;
	
	int			num_leben;
	int			num_powerup;
	int			num_delay;
	long int	num_score;
	cAnim		gfx_specialanim;
};
#endif

