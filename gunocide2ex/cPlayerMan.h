/***************************************************************************
                          cPlayerMan.h  -  description
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
#ifndef CPLAYERMAN_H
#define CPLAYERMAN_H

#include <stdlib.h>
#include <SDL/SDL.h>

class cPlayer;

class cPlayerMan
{
public:
	cPlayerMan();
	~cPlayerMan();

	bool				create(int num);
	bool				load(int player,int type);
	void				update();
	void				render(SDL_Surface *screen);
	void				setnum(int num);
	int					getnum();

	void				setposx(int player,int x);
	int					getposx(int player);
	void				setposy(int player,int y);
	int					getposy(int player);

	void				setbmoveleft(int player,bool wert);
	void				setbmoveright(int player,bool wert);
	void				setbshot(int player,bool wert);
	void				setbspecial(int player,bool wert);
	void				addscore(int player,int score);
	long int			getscore(int player);
	void				addpwgun(int player);
	void				addpwspecial(int player);
	void				addpwlife(int player);
	void				dellife(int player);
	bool				alive(int player);
	SDL_Rect			getcolrect(int player,int offx, int offy);
	int					getshieldstate(int player);
	void				setshieldstate(int player,int wert);
	void				addshot_treffer(int player);
	long				getshot_treffer(int player);
	void				shot_reset(int player);
	long				getshot_anzahl(int player);
	void				addwave(int player);
	int					getwaves(int player);

	static cPlayerMan*	getInstance();
	static void			delInstance();
private:
	static cPlayerMan*	instance;

	cPlayer		*pPlayer;
	int			numplayer;
};
#endif
