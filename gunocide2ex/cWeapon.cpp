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

#include "cWeapon.h"
#include "cUtil.h"
#include "cPlayerMan.h"
#include "cEnemyMan.h"
#include "mmgr.h"

cWeapon::cWeapon()
{
	cPlayerMan *pPlayerMan=cPlayerMan::getInstance();

	posx=posy=0;
	dirx=diry=0;
	anim_sprite.rewind();
	anim_sprite.setMaxNum(0);
	owner=0;
	energy=0;
	
	playertarget=rand()%pPlayerMan->getnum();
	if(playertarget>1)
		playertarget=1;
}

cWeapon::~cWeapon()
{
	posx=posy=0;
	dirx=diry=0;
	anim_sprite.rewind();
	anim_sprite.setMaxNum(0);
	owner=0;
}

///setzt die X-Koordinate für den Schuss
void cWeapon::setposx(int wert)
{
	posx=wert;
}

///gibt die X-Koordinate des Schusses zurück


int cWeapon::getposx()
{
	return posx;
}

///setzt die Y-Koordinate für den Schuss
void cWeapon::setposy(int wert)
{
	posy=wert;
}

///gibt die Y-Koordinate des Schusses zurück
int cWeapon::getposy()
{
	return posy;
}

///setzt die X-Richtung für den Schuss
void cWeapon::setdirx(int wert)
{
	dirx=wert;
}

///gibt die X-Richtung des Schusses zurück
int cWeapon::getdirx()
{
	return dirx;
}

///setzt die Y-Richtung für den Schuss
void cWeapon::setdiry(int wert)
{
	diry=wert;

}

///gibt die Y-Richtung des Schusses zurück
int cWeapon::getdiry()
{
	return diry;
}

///setzt den Sprite-Typ des Schusses
void cWeapon::setsprite(int wert)
{
	typ_sprite=wert;
}

///gibt den Sprite-Typ des Schusses zurück
int cWeapon::getsprite()
{
	return typ_sprite;
}

///setzt die Spriteanimation
void cWeapon::setanim(cSpriteAnim *anim)
{
	anim_sprite.rewind();
	anim_sprite.setLooped(true);
	anim_sprite.setDelay(150);
	anim_sprite.setMaxNum(anim->getMaxNum());
	anim_sprite.setLooped(true);
}

///gibt die SpriteAnimation zurück
cAnim* cWeapon::getanim()
{
	return &anim_sprite;
}

///zeichnen des Schusses auf dem Bildschirm
void cWeapon::render(SDL_Surface *screen,cSpriteAnim *anim)
{
	anim->render(screen,&anim_sprite,posx,posy);
}

///aktualisieren des Schusses..z.B. bewegen
void cWeapon::update()
{
	cPlayerMan	*pPlayerMan=cPlayerMan::getInstance();

	cUtil	*pUtil=cUtil::getInstance();

	posx+=(int)pUtil->sync((float)dirx);
	posy+=(int)pUtil->sync((float)diry);

	anim_sprite.update();

	///was macht dieses kleine stück code wohl?
	///hmm..sieht nach 'ner lenkrakete aus ;)
	if(typ_sprite==ENEMY_GUARDIAN || typ_sprite==ENEMY_WIDOWMAKER)
	{
		if(pPlayerMan->getposx(playertarget)>getposx())
			posx+=(int)pUtil->sync(150);			
		else
			posx-=(int)pUtil->sync(150);
	}
}

///gibt eine SDL_Rect Struktur zurück, die für eine Kollisionsabfrage
///benötigt wird
SDL_Rect cWeapon::getcolrect(cSpriteAnim *anim,int offx,int offy)
{
	SDL_Rect rc;

	rc.x=getposx()+offx;
	rc.y=getposy()+offy;
	rc.w=getposx()+anim->getwidth(&anim_sprite)-offx;
	rc.h=getposy()+anim->getheight(&anim_sprite)-offy;

	return rc;
}

///vom wem ist der schuss
void cWeapon::setowner(int wert)
{
	owner=wert;
}

///von wem ist der schuss
int cWeapon::getowner()
{
	return owner;
}

void cWeapon::setenergy(int wert)
{
	energy=wert;
}

int cWeapon::getenergy()
{
	return energy;
}
