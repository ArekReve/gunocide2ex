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
#include "cEffects.h"
#include "cEnemy.h"
#include "cUtil.h"
#include "cSound.h"
#include "cEnemyMan.h"
#include "cPlayerMan.h"
#include "mmgr.h"

cEnemy::cEnemy()
{
	posx=posy=0;
	typ_sprite=-1;
	typ_powerup=-1;
	balive=false;
	anim_sprite.rewind();
	anim_sprite.setMaxNum(0);
	movex=0;
	bmleft=false;
	delay_shot=0;
	delay_start=0;
	bshot=false;
	energy=0;
	battack=false;
	battack_moveback=false;
	attack_posx=0;
	attack_posy=0;
	playertarget=0;
}

cEnemy::~cEnemy()
{
	posx=posy=0;
	typ_sprite=-1;
	typ_powerup=-1;
	balive=false;
	anim_sprite.rewind();
	anim_sprite.setMaxNum(0);
}

///setzt die X-Koordinate dieses Gegners
void cEnemy::setposx(int wert)
{
	posx=wert;
}

///gibt die X-Koordinate dieses Gegners zurück
int cEnemy::getposx()
{
	if(!battack)
		return posx+movex;
	else
		return attack_posx;
}

///setzt die Y-Koordinate dieses Gegners
void cEnemy::setposy(int wert)
{
	posy=wert;
}

///gibt die Y-Koordinate dieses Gegners zurück
int cEnemy::getposy()
{
	if(!battack)
		return posy;
	else
		return attack_posy;
}

///setzt den Spritetyp des Gegners
void cEnemy::setsprite(int wert)
{
	typ_sprite=wert;
}

///gibt den Spritetyp des Gegners zurück
int cEnemy::getsprite()
{
	return typ_sprite;
}

///setzt den Gegnern entweder lebendig oder nicht
void cEnemy::setalive(bool wert)
{
	balive=wert;
}

///gibt zurück ob der Gegner lebendig ist oder nicht
bool cEnemy::getalive()
{
	return balive;
}

///setzt den PowerUp-Typ
void cEnemy::setpowerup(int wert)
{
	typ_powerup=wert;
}

///gibt den PowerUp-Typ zurück
int cEnemy::getpowerup()
{
	return typ_powerup;
}

///setzt alle Daten für die Animationsklasse
void cEnemy::setanim(cSpriteAnim *anim)
{
	anim_sprite.rewind();
	anim_sprite.setLooped(true);
	anim_sprite.setDelay(50);
	anim_sprite.setMaxNum(anim->getMaxNum());
}

int cEnemy::getwidth(cSpriteAnim *anim)
{
	return anim->getwidth(&anim_sprite);
}

int cEnemy::getheight(cSpriteAnim *anim)
{
	return anim->getheight(&anim_sprite);
}

///gibt die Animationsklasse zurück
cAnim* cEnemy::getanim()
{
	return &anim_sprite;
}

///aktualisieren des Gegners
void cEnemy::update()
{
	long int	act_time;
	cUtil		*pUtil=cUtil::getInstance();
	cEnemyMan	*pEnemyMan=cEnemyMan::getInstance();
	cPlayerMan	*pPlayerMan=cPlayerMan::getInstance();

	anim_sprite.update();

	///Verzögerung des Schusses berechnen
	act_time=SDL_GetTicks();
	if(act_time-delay_start>=delay_shot)
	{
		delay_start=act_time;
		bshot=true;
	}

	///bewegen
	if(bmleft==false)
	{
		movex+=(int)pUtil->sync(100.0f);
		if(movex>50)
			bmleft=true;
	}
	else
	{
		movex+=(int)pUtil->sync(-100.0f);
		if(movex<-50)
			bmleft=false;
	}
	
	///soll der gegner angreifen??
	if(!battack)
	{
		if( (rand()%250+1)==1 && !pEnemyMan->getenemyattack())
		{
			attack_posx=getposx();
			attack_posy=getposy();

			battack=true;
			playertarget=rand()%pPlayerMan->getnum();
			if(playertarget>1)playertarget=1;

			pEnemyMan->setenemyattack(true);
		}
	}
	else ///der gegner ist am angreifen
	{
		if(!battack_moveback) //der gegner fliegt nach vorne
		{
			attack_posy+=(int)pUtil->sync(200.0f);
			if(attack_posy>=360)battack_moveback=true;

            if(pPlayerMan->getposx(playertarget)>getposx())attack_posx+=(int)pUtil->sync(100.0f);
			else attack_posx-=(int)pUtil->sync(100.0f);
		}
		else // der gegner fliegt zurück in die formation
		{	
			attack_posy-=(int)pUtil->sync(150.0f);

            if(posx>getposx())attack_posx+=(int)pUtil->sync(100.0f);
			else attack_posx-=(int)pUtil->sync(100.0f);

			if(attack_posy<=posy)
			{///der angriff ist vorüber
				battack=false;
				pEnemyMan->setenemyattack(false);
			}
		}
	}

	pUtil=NULL;
}

///zeichnen des Gegners
void cEnemy::render(SDL_Surface *screen,cSpriteAnim *anim)
{
	if(!battack)anim->render(screen,&anim_sprite,posx+movex,posy);
	else anim->render(screen,&anim_sprite,attack_posx,attack_posy);
}

///Die Verzögerung eines Schusses beim Gegner setzen
void cEnemy::setdelayshot(long wert)
{
	delay_shot=wert;
	delay_start=SDL_GetTicks();
}


///Verzögerungs des Schusses des Gegners erhalten
long int cEnemy::getdelayshot()
{
	return delay_shot;
}

///setzen ob der Gegner schiessen darf oder nicht
void cEnemy::setshot(bool wert)
{
	bshot=wert;
}

///darf der Gegner schissen oder nicht ?
bool cEnemy::getshot()
{
	return bshot;
}

///gibt eine SDL_Rect Struktur zurück, die für eine Kollisionsabfrage
///benötigt wird
SDL_Rect cEnemy::getcolrect(cSpriteAnim *anim,int offx,int offy)
{
	SDL_Rect rc;

	rc.x=getposx()+offx;
	rc.y=getposy()+offy;
	rc.w=getposx()+anim->getwidth(&anim_sprite)-offx;
	rc.h=getposy()+anim->getheight(&anim_sprite)-offy;

	return rc;
}

///energie für den Gegner setzen
void cEnemy::setenergy(int wert)
{
	energy=wert;
}

///energie des Gegners erhalten
int cEnemy::getenergy()
{
	return energy;
}
