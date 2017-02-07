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

#include <string.h>
#include "cSpriteAnim.h"
#include "mmgr.h"

/******************************************************************
*cAnim-Klasse
******************************************************************/
cAnim::cAnim()
{
	act_sprite=0;
	blooped=false;
	num_sprite=0;
	delay=act_delay=last_delay=0;
}

cAnim::~cAnim()
{
	act_sprite=0;
	blooped=false;
	num_sprite=0;
	delay=act_delay=last_delay=0;
}

///gibt die aktuelle Position der Animation zurück
int cAnim::getAct()
{
	return act_sprite;
}

///setzt die aktuelle Position der Animation
void cAnim::setAct(int wert)
{
	act_sprite=wert;
}

///setzt die Animation wieder auf den Anfang zurück
void cAnim::rewind()
{
	last_delay=SDL_GetTicks();
	act_sprite=0;
}

///gibt zurück ob die Animation wiederholt werden soll
///oder nicht
bool cAnim::getLooped()
{
	return blooped;
}

///setzt die Animation so, dass sie entweder wiederholt
///oder nicht
void cAnim::setLooped(bool wert)
{
	blooped=wert;
}

///gibt die max.Anzahl der Animationsstufen zurück
int cAnim::getMaxNum()
{
	return num_sprite;
}

///setzt die max.Anzahl der Animationsstufen
void cAnim::setMaxNum(int wert)
{
	num_sprite=wert;
}

///aktualisieren der Animation
void cAnim::update()
{
	if(act_sprite<(num_sprite-1))
	{
		act_delay=SDL_GetTicks();
		if(act_delay-last_delay>=delay)
		{
			act_sprite++;
			last_delay=act_delay;
		}
	}
	else
	{
		//wenn wiederholt werden soll muss auch hier auf die
		///Verzögerung geachtet werden!
		if(blooped)
		{
			act_delay=SDL_GetTicks();
			if(act_delay-last_delay>=delay)
			{
				rewind();
				last_delay=act_delay;
			}
		}
	}
}

///gibt die Verzögerung zurück die gesetzt wurde
///bis die nächste Animationsstufe gesetzt wird
long int cAnim::getDelay()
{
	return delay;
}

///setzt die Verzögerung bis zur nächsten Animationsstufe
void cAnim::setDelay(long int ms)
{
	delay=ms;
}

/******************************************************************
*cSpriteAnim-Klasse
******************************************************************/
cSpriteAnim::cSpriteAnim()
{
	gfx_sprite=NULL;
	num_sprite=0;
}

cSpriteAnim::~cSpriteAnim()
{
	if(gfx_sprite)
	{
		delete [] gfx_sprite;
		gfx_sprite=NULL;
	}

	num_sprite=0;
}

///einladen einer Animation aus einer Datei
bool cSpriteAnim::load(char *path)
{
	FILE 	*file;
	char	buffer[512];

	file=fopen(path,"r");
	if(!file)
	{
		printf("failed to load anim file:%s\n",path);
		return false;
	}

	printf("loading anim file:%s\n",path);

	num_sprite=getnum(file);
	if(num_sprite<1)
	{
		printf("anim file doesn't contain any animations: %s\n",path);
		fclose(file);
		return false;
	}

	printf("anim file contains %i animations :%s\n",num_sprite,path);

	gfx_sprite=new cSprite[num_sprite];
	///einlesen der Animationen
	for(int i=0;i<num_sprite;i++)
	{
		fscanf(file,"%s",buffer);
		if(!gfx_sprite[i].loadBMP(buffer))
		{
			printf("failed to load anim file:%s\n",path);
			fclose(file);
			return false;
		}
	}

	printf("loaded succesfully anim file :%s\n",path);
	fclose(file);
	return true;
}

///colorkey für jedes Sprite der Animation setzen
void cSpriteAnim::setColorKey(int r,int g,int b)
{
	if(!gfx_sprite)
		return;

	for(int i=0;i<num_sprite;i++)
	{
		gfx_sprite[i].setColorKey(r,g,b);
	}
}

///gibt die Anzahl der Animationen
///innerhalb einer Datei zurück
int cSpriteAnim::getnum(FILE *file)
{
	int 	num=0;
	char	buffer[512];

	if(!file)
		return -1;

	rewind(file);

	while(!feof(file))
	{
		strcpy(buffer,"\0");
		fscanf(file,"%s",buffer);
		if(strcmp(buffer,"\0")!=0)num++;
	}
	rewind(file);
	return num;
}

///rendert eine Animation auf dem Bildschirm
void cSpriteAnim::render(SDL_Surface *screen,cAnim *anim,int x,int y)
{
	if(anim->getAct()>num_sprite || anim->getAct()<=-1)
		return;

	gfx_sprite[anim->getAct()].render(screen,x,y);
}

///gibt die Anzahl der Animationsstufen zurück
int cSpriteAnim::getMaxNum()
{
	return num_sprite;
}

///Breite der aktuellen Animationsstufe zurückgeben
int cSpriteAnim::getheight(cAnim *anim)
{
	if(anim==NULL)return -1;
	return gfx_sprite[anim->getAct()].getheight();
}

///Höhe der aktuellen Animationsstufe zurückgeben
int cSpriteAnim::getwidth(cAnim *anim)
{
	if(anim==NULL)return -1;
	return gfx_sprite[anim->getAct()].getwidth();
}