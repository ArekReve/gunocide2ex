/***************************************************************************
                          cKeyboard.cpp  -  description
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
#include "cKeyboard.h"
#include "mmgr.h"

cKeyboard::cKeyboard()
{
	mousex=0;
	mousey=0;
	mouse_pressed=-1;
	memset(clastkey,0,sizeof(clastkey));
}

cKeyboard::~cKeyboard()
{
}

cKeyboard* cKeyboard::instance=NULL;

cKeyboard* cKeyboard::getInstance()
{
	if(instance==NULL)
	{
		instance=new cKeyboard();
		return instance;
	}
	else return instance;
}

void cKeyboard::delInstance()
{
	if(instance)
	{
		delete instance;
		instance=NULL;
	}
}

///hier updaten wir die Klasse
///und fügen der Tastenliste neue hinzu falls eine Taste gedrückt wurde
///oder halt umgekehrt wenn eine losgelassen wurde
void cKeyboard::update()
{
	SDL_Event	event;
	
	///alle keyevents aus der liste löschen
	lKeyEvents.clear();
	strcpy(clastkey,"\0");

	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_KEYDOWN:
				addkey(event.key.keysym.sym,1);
				strcpy(clastkey,SDL_GetKeyName(event.key.keysym.sym));
				break;
			case SDL_KEYUP:
				addkey(event.key.keysym.sym,-1);
				break;
			case SDL_MOUSEMOTION:
				mousex=event.motion.x;
				mousey=event.motion.y;
				break;
			case SDL_MOUSEBUTTONDOWN:
				mouse_pressed=1;
				break;
			case SDL_MOUSEBUTTONUP:
				mouse_pressed=-1;
				break;
		}
	}
}

///hinzufügen eines events der tastenevent-liste
void cKeyboard::addkey(SDLKey key,int pressed)
{
	KeyEvent	*kevent;

	kevent=new KeyEvent();
	kevent->key=key;
	kevent->pressed=pressed;

	lKeyEvents.push_back(*kevent);
	delete kevent;
}


///sucht in der liste ob eine gesuchte taste existiert
///und gibt gegebenfalls den pointer zu diesem zurück
KeyEvent* cKeyboard::getkey(SDLKey searchkey)
{
	KeyEvent *temp=NULL;
	LISTKEYEVENTS::iterator i;

	for(i=lKeyEvents.begin();i!=lKeyEvents.end();++i)
	{
		if(i->key==searchkey)
		{
			temp=new KeyEvent();
			temp->key=i->key;
			temp->pressed=i->pressed;

			return temp;
		}
	}	
	return NULL;
}

///gibt die X-Koordinate der Maus zurück
int cKeyboard::getmousex()
{
	return mousex;
}

///gibt die Y-Koordinate der Maus zurück
int cKeyboard::getmousey()
{
	return mousey;
}

///wurde die taste gedrückt, ja oder nein ?
int cKeyboard::getmousepressed()
{
	int temp=mouse_pressed;
	mouse_pressed=-1;
	return temp;
}

///den String oder Char der letzten Taste zurückgeben
char*	cKeyboard::getlastkey()
{
	return clastkey;
}
