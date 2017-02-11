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
#include "cFont.h"
#include "cDisplay.h"
#include "mmgr.h"

cDisplay::cDisplay()
{
	num_leben=0;
	num_delay=0;
	num_powerup=0;
	num_score=0;
}

cDisplay::~cDisplay()
{
}

///Anzahl der Leben zum Anzeigen setzen
void cDisplay::setleben(int wert)
{
	num_leben=wert;
}


///Anzahl der Leben zur�ckerhalten
int cDisplay::getleben()
{
	return num_leben;
}

///Anzahl der PowerUps zum Anzeigen setzen
void cDisplay::setpowerup(int wert)
{
	num_powerup=wert;
}

///Anzahl der PowerUps zum Anzeigen zur�ckgeben
int cDisplay::getpowerup()
{
	return num_powerup;
}

void cDisplay::setdelay(int wert)
{
	num_delay=wert;
}


long int cDisplay::getscore()
{
	return num_score;
}

void cDisplay::setscore(long int score)
{
	num_score=score;
}

///Display des Spielers zeichnen
void cDisplay::render(SDL_Surface *screen,int player)
{
	cFont	*pFont=cFont::getInstance();
	int i=0;

	if(player==0)
	{
		//Anzeige der MiniGun
		gfx_mg[num_delay].render(screen,8,32);

		//score anzeigen
		gfx_score.render(screen,8,8);
		pFont->setcolor(0,255,255);
		pFont->print(screen,42,5,"%d\0",num_score);
		pFont->setcolor(255,255,255);

		//leben anzeigen
		for(i=0;i<num_leben;i++)gfx_leben.render(screen,8+(25*i),52);

		///special zeigen
		for(i=0;i<num_powerup;i++)gfx_special.render(screen,&gfx_specialanim,8+(22*i),78);
	}
	else
	{
		//Anzeige der MiniGun
		gfx_mg[num_delay].render(screen,520,32);

		//score anzeigen
		gfx_score.render(screen,520,8);
		pFont->setcolor(0,255,255);
		pFont->print(screen,554,5,"%d\0",num_score);
		pFont->setcolor(255,255,255);

		//leben anzeigen
		for(i=0;i<num_leben;i++)gfx_leben.render(screen,520+(25*i),52);

		///special zeigen
		for(i=0;i<num_powerup;i++)gfx_special.render(screen,&gfx_specialanim,520+(22*i),78);
	}
}


///Display aktualisieren
void cDisplay::update()
{
	gfx_specialanim.update();
}

///einladen der Grafiken f�r das Display
bool cDisplay::load(int playertyp,int nr)
{
	char buffer[256];

	//minigun anzeige
	for(int i=0;i<5;i++)
	{
		sprintf(buffer,"/usr/local/games/gunocide2ex/gfx/display/m%i.bmp",i);
		if(!gfx_mg[i].loadBMP(buffer))
			return false;
		gfx_mg[i].setColorKey(0,0,0);
	}

	//score
	if(nr==0)
	{
		if(!gfx_score.loadBMP("/usr/local/games/gunocide2ex/gfx/display/score1.bmp"))
			return false;
	}
	else
	{
		if(!gfx_score.loadBMP("/usr/local/games/gunocide2ex/gfx/display/score2.bmp"))
			return false;
	}
	gfx_score.setColorKey(0,0,0);

	//leben
	sprintf(buffer,"/usr/local/games/gunocide2ex/gfx/display/l%i.bmp",playertyp);
	if(!gfx_leben.loadBMP(buffer))
		return false;
	gfx_leben.setColorKey(0,0,0);

	//specialgrafik
	if(!gfx_special.load("/usr/local/games/gunocide2ex/gfx/display/special.txt"))
		return false;
	gfx_special.setColorKey(0,0,0);
	
	gfx_specialanim.setMaxNum(gfx_special.getMaxNum());
	gfx_specialanim.setLooped(true);
	gfx_specialanim.setDelay(200);

	printf("display graphics succesfully loaded\n");
	return true;
}
