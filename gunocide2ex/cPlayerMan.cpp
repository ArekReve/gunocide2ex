/***************************************************************************
                          cPlayerMan.cpp  -  description
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
#include "cPlayerMan.h"
#include "cPlayer.h"
#include "mmgr.h"

cPlayerMan::cPlayerMan()
{
	numplayer=0;
	pPlayer=NULL;
}

cPlayerMan::~cPlayerMan()
{
	if(pPlayer)
	{
		delete [] pPlayer;
	}
}

cPlayerMan* cPlayerMan::instance=NULL;

cPlayerMan* cPlayerMan::getInstance()
{
	if(instance==NULL)
	{
		instance=new cPlayerMan();
		return instance;
	}
	else return instance;
}

void cPlayerMan::delInstance()
{
	if(instance)
	{
		delete instance;
		instance=NULL;
	}
}

///erstellen der spieler 
bool cPlayerMan::create(int num)
{
	pPlayer=new cPlayer[num];
	if(!pPlayer)
	{
		printf("failed to allocate memory for players\n");
		return false;
	}
	
	numplayer=num;

	for(int i=0;i<numplayer;i++)
		pPlayer[i].setnr(i);
	return true;
}

///aktualisieren der Spieler
void cPlayerMan::update()
{
	for(int i=0;i<numplayer;i++)
	{
		if(pPlayer[i].alive())
			pPlayer[i].update();
	}
}

///zeichnen der spieler
void cPlayerMan::render(SDL_Surface *screen)
{
	for(int i=0;i<numplayer;i++)
	{
		if(pPlayer[i].alive())
			pPlayer[i].render(screen);
	}
}

///laden eines spielers
bool cPlayerMan::load(int player,int type)
{
	return pPlayer[player].load(type);
}

void cPlayerMan::setposx(int player,int x)
{
	pPlayer[player].setposx(x);
}

int cPlayerMan::getposx(int player)
{
	return pPlayer[player].getposx();
}

void cPlayerMan::setposy(int player,int y)
{
	pPlayer[player].setposy(y);
}

int cPlayerMan::getposy(int player)
{
	return pPlayer[player].getposy();
}

void cPlayerMan::setbmoveleft(int player,bool wert)
{
	pPlayer[player].setbmoveleft(wert);
}

void cPlayerMan::setbmoveright(int player,bool wert)
{
	pPlayer[player].setbmoveright(wert);
}

void cPlayerMan::setbshot(int player,bool wert)
{
	pPlayer[player].setbshot(wert);
}

void cPlayerMan::setbspecial(int player,bool wert)
{
	pPlayer[player].setbspecial(wert);
}

void cPlayerMan::addscore(int player,int score)
{
	pPlayer[player].addscore(score);
}

long int cPlayerMan::getscore(int player)
{
	return pPlayer[player].getscore();
}

void cPlayerMan::addpwgun(int player)
{
	pPlayer[player].addpwgun();
}

void cPlayerMan::addpwspecial(int player)
{
	pPlayer[player].addpwspecial();
}

void cPlayerMan::addpwlife(int player)
{
	pPlayer[player].addpwlife();
}

void cPlayerMan::dellife(int player)
{
	pPlayer[player].dellife();
}

bool cPlayerMan::alive(int player)
{
	return pPlayer[player].alive();
}

SDL_Rect cPlayerMan::getcolrect(int player,int offx, int offy)
{
	return pPlayer[player].getcolrect(offx,offy);
}

int cPlayerMan::getshieldstate(int player)
{
	return pPlayer[player].getshieldstate();
}

void cPlayerMan::setshieldstate(int player,int wert)
{
	pPlayer[player].setshieldstate(wert);
}

void cPlayerMan::addshot_treffer(int player)
{
	pPlayer[player].addshot_treffer();
}

long cPlayerMan::getshot_treffer(int player)
{
	return pPlayer[player].getshot_treffer();
}

void cPlayerMan::shot_reset(int player)
{
	pPlayer[player].shot_reset();
}

long cPlayerMan::getshot_anzahl(int player)
{
	return pPlayer[player].getshot_anzahl();
}

void cPlayerMan::addwave(int player)
{
	pPlayer[player].addwave();
}

int cPlayerMan::getwaves(int player)
{
	return pPlayer[player].getwaves();
}

void cPlayerMan::setnum(int num)
{
	numplayer=num;
}

int cPlayerMan::getnum()
{
	return numplayer;
}
