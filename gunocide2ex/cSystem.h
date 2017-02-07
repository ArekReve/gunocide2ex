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

#ifndef CSYSTEM_H
#define CSYSTEM_H

#include <stdlib.h>
#include <SDL/SDL.h>
#include "cSprite.h"

#define		G2EX_VERSION		"Final"
#define		GSTATE_INGAME		1
#define 		GSTATE_MENU			2
#define		GSTATE_EXIT			3

#define		GMODE_SUICIDE		1
#define		GMODE_ENDLESS		2
#define		GMODE_STORY			3

class cEnemyMan;

///wird für die Tastaturkonfiguration benutzt
typedef struct keymap
{
	int	left;
	int	right;
	int	shot;
	int special;
}KEYMAP;

class cSystem
{
public:
	cSystem();
	~cSystem();

	bool			init();
	bool			initSDL();
	void			gameLoop();
	SDL_Surface*	getScreen();
	void			setgamestate(int state);
	void			setgamemode(int mode);
	int				getgamemode();
	void			setactstage(int wert);
	bool			getgameover();
	bool			getnextwave();

	static cSystem*	getInstance();
	static void		delInstance();
private:
	bool			loadsounds();
	void			control();
	void			check_env();
	bool			load();
	void			loadkeymaps();
	void			sysrender();
	void			sysupdate();
	
	static cSystem*	instance;

	SDL_Surface		*screen;
	cSprite			gfx_sprite[3];
	int				gamestate;
	int				gamemode;
	int				time_suicide;
	long int		time_suicide_delay;
	bool			bgameover;
	bool			bnextwave;
	bool			bpause;
	long int		gameover_delay;
	long int		nextwave_delay;
	int				nextwave_time;

	int				act_stage;
	int				stage_selected;

	KEYMAP			keymap[2];
};

#endif
