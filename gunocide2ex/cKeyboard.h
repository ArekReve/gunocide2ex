/***************************************************************************
                          cKeyboard.h  -  description
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
#ifndef CKEYBOARD_H
#define CKEYBOARD_H

#include <SDL/SDL.h>
#include <list>

using namespace std;

typedef struct sKeyEvent
{
	SDLKey	key;
	int		pressed;
} KeyEvent;

typedef list<KeyEvent> LISTKEYEVENTS;

class cKeyboard
{
public:
	cKeyboard();
	~cKeyboard();

	static  cKeyboard*	getInstance();
	static	void		delInstance();

	void		update();
	KeyEvent*	getkey(SDLKey searchkey);
	int			getmousex();
	int			getmousey();
	int			getmousepressed();
	char*		getlastkey();
private:
	void	addkey(SDLKey key,int pressed);

	static cKeyboard	*instance;

	LISTKEYEVENTS	lKeyEvents;

	int				mousex,mousey;
	int				mouse_pressed;
	char			clastkey[10];
};
#endif
