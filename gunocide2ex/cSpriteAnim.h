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

#ifndef CSPRITEANIM_H
#define CSPRITEANIM_H

#include <SDL/SDL.h>
#include "cSprite.h"

class cAnim
{
public:
	cAnim();
	~cAnim();

	int			getAct();
	void		setAct(int wert);
	bool		getLooped();
	void		setLooped(bool wert);
	int			getMaxNum();
	void		setMaxNum(int wert);
	long int	getDelay();
	void		setDelay(long ms);

	void		rewind();
	void		update();
private:
	int			act_sprite;
	int			num_sprite;
	bool		blooped;
	long int	act_delay;
	long int	last_delay;
	long int	delay;
};

class cSpriteAnim
{
public:
	cSpriteAnim();
	~cSpriteAnim();

	bool	load(char *path);
	void	setColorKey(int r,int g,int b);
	void	render(SDL_Surface *screen,cAnim *anim,int x,int y);
	int		getMaxNum();

	int		getheight(cAnim *anim);
	int		getwidth(cAnim *anim);
private:
	int		getnum(FILE *file);

	cSprite		*gfx_sprite;
	int			num_sprite;
};
#endif
