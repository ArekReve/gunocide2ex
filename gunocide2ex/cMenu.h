/***************************************************************************
                          cMenu.h  -  description
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
#ifndef CMENU_H
#define CMENU_H

#include "cSprite.h"


#define MSTATE_TITEL				0
#define MSTATE_MAIN				1
#define MSTATE_HSCORE			2
#define MSTATE_GMODE			3
#define MSTATE_LEVELSELECT	4
#define MSTATE_PLAYERSELECT	5
#define MSTATE_CREDITS			6
#define MSTATE_POLYFRAG		7

class cMenu
{
public:
	cMenu();
	~cMenu();
	
	bool			load();
	void			update();
	void			render(SDL_Surface *screen);
	void			setmenustate(int state);

	static cMenu* 	getInstance();
	static void	   	delInstance();
private:
	void			render_polyfrag(SDL_Surface *screen);
	void			render_startbild(SDL_Surface *screen);
	void			render_main(SDL_Surface *screen);
	void			render_credits(SDL_Surface *screen);
	void			render_modi(SDL_Surface *screen);
	void			render_stageselect(SDL_Surface *screen);
	void			render_playerselect(SDL_Surface *screen);
	void			render_highscore(SDL_Surface *screen);
	void			update_polyfrag();
	void			update_startbild();
	void			update_main();
	void			update_credits();
	void			update_modi();
	void			update_stageselect();
	void			update_playerselect();
	void			update_highscore();
	bool			loadgamedata();

	static cMenu	*instance;
	
    	cSprite			gfx_sprite[11];
	cSprite			gfx_stage[5];
	cSprite			gfx_player[3];
	cSprite			gfx_mouse;
	int				menustate;

	bool			text_main[4];
	bool			text_gmode[5];
	int				pselected;
	int				num_player;
	int				stageselected;
	int				*playerselected;
	int				scoreselected;
};
#endif
