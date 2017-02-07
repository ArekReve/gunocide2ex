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

#ifndef CPLAYER_H
#define CPLAYER_H

#include <SDL/SDL.h>
#include "cSpriteAnim.h"

#define MOVE_NORMAL	0
#define MOVE_LEFT	1
#define	MOVE_RIGHT	4

#define SHOT_DELAY	100

#define PLAYER_RAGE	0
#define PLAYER_FURY	1
#define PLAYER_BANSHEE	2

class cDisplay;

class cPlayer
{
public:
	cPlayer();
	~cPlayer();

	bool		load(int type);
	void		render(SDL_Surface *screen);
	void		update();

	void		setposx(int x);
	int			getposx();
	void		setposy(int y);
	int			getposy();

	void		setbmoveleft(bool wert);
	void		setbmoveright(bool wert);
	void		setbshot(bool wert);
	void		setbspecial(bool wert);
	void		addscore(int score);
	long int	getscore();
	void		addpwgun();
	void		addpwspecial();
	void		addpwlife();
	void		dellife();
	bool		alive();
	SDL_Rect	getcolrect(int offx, int offy);
	int			getshieldstate();
	void		setshieldstate(int wert);
	void		addshot_treffer();
	long		getshot_treffer();
	void		shot_reset();
	long		getshot_anzahl();
	void		addwave();
	int			getwaves();
	void		setnr(int nr);
	int			getnr();

private:
	void		addshots();
	void		addspecial();

	cSpriteAnim	gfx_sprite[7];
	cSprite		gfx_shield;
	cAnim		anim_sprite[7];
	cDisplay	*pDisplay;

	int			posx,posy;
	bool		bmoveleft;
	bool		bmoveright;
	int			mvdirection;
	int			mvstate;		//<- in welcher Animation bei einer Bewegung
	bool		bshot;
	bool		bspecial;
	long int	delay_shot;
	long int	delay_special;
	int			typ_player;
	int			typ_weapon;
	int			delay_mg;
	long int	delay_mg_act;	
	long int	score;
	int			lives;
	int			specials;
	int			shieldstate;
	int			shielddelay;
	long		shot_treffer;
	long		shot_gefeuert;
	int			waves;
	int			nr;
};


#endif
