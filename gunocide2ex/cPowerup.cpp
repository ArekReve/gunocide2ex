/***************************************************************************
                          cPowerup.h  -  description
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
#include "cPowerup.h"
#include "cUtil.h"
#include "cPowerup.h"
#include "cPlayerMan.h"
#include "cSound.h"
#include "cSystem.h"
#include "mmgr.h"

/**************************************************************************
cPowerup-Klasse
***************************************************************************/
cPowerup::cPowerup()
{
	posx=0;
	posy=0;
	memset(&gfx_anim,0,sizeof(gfx_anim));
	type_powerup=0;
}

cPowerup::~cPowerup()
{
}

void cPowerup::render(SDL_Surface *screen,cSpriteAnim *anim)
{
	anim->render(screen,&gfx_anim,posx,posy);
}

void cPowerup::update()
{
	cUtil	*pUtil=cUtil::getInstance();

	posy+=(int)pUtil->sync(100.0f);
	
	gfx_anim.update();
}

void cPowerup::setposx(int x)
{
	posx=x;
}

int cPowerup::getposx()
{
	return posx;
}

void cPowerup::setposy(int y)
{
	posy=y;
}

int cPowerup::getposy()
{
	return posy;
}

void cPowerup::setsprite(cSpriteAnim *anim)
{
	gfx_anim.setMaxNum(anim->getMaxNum());
	gfx_anim.setDelay(100);
	gfx_anim.setLooped(true);
}

cAnim* cPowerup::getsprite()
{
	return &gfx_anim;
}

void cPowerup::settype(int type)
{
	type_powerup=type;
}

int cPowerup::gettype()
{
	return type_powerup;
}

///gibt eine SDL_Rect Struktur zurück, die für eine Kollisionsabfrage
///benötigt wird
SDL_Rect cPowerup::getcolrect(cSpriteAnim *anim,int offx,int offy)
{
	SDL_Rect rc;

	rc.x=getposx()+offx;
	rc.y=getposy()+offy;
	rc.w=getposx()+anim->getwidth(&gfx_anim)-offx;
	rc.h=getposy()+anim->getheight(&gfx_anim)-offy;

	return rc;
}
/**************************************************************************
cPowerupMan-Klasse
***************************************************************************/
cPowerupMan::cPowerupMan()
{
}

cPowerupMan::~cPowerupMan()
{
	lPowerups.clear();
}

cPowerupMan* cPowerupMan::instance=NULL;

cPowerupMan* cPowerupMan::getInstance()
{
	if(instance==NULL)
	{
		instance=new cPowerupMan();
		return instance;
	}
	else return instance;
}

void cPowerupMan::delInstance()
{
	if(instance)
	{
		delete instance;
		instance=NULL;
	}
}

///einladen aller Grafiken für die PowerUps
bool cPowerupMan::load()
{
	if(!gfx_sprite[POWERUP_GUN-1].load("/usr/local/games/gunocide2ex/gfx/powerups/gun/gun.txt"))
		return false;
	gfx_sprite[POWERUP_GUN-1].setColorKey(0,0,0);
	
	if(!gfx_sprite[POWERUP_LIFE-1].load("/usr/local/games/gunocide2ex/gfx/powerups/life/life.txt"))
		return false;
	gfx_sprite[POWERUP_LIFE-1].setColorKey(0,0,0);

	if(!gfx_sprite[POWERUP_SCORE-1].load("/usr/local/games/gunocide2ex/gfx/powerups/punkte/punkte.txt"))
		return false;
	gfx_sprite[POWERUP_SCORE-1].setColorKey(0,0,0);

	if(!gfx_sprite[POWERUP_SPECIAL-1].load("/usr/local/games/gunocide2ex/gfx/powerups/special/special.txt"))
		return false;
	gfx_sprite[POWERUP_SPECIAL-1].setColorKey(0,0,0);

	printf("powerup graphics succesfully loaded\n");
	return true;
}

void cPowerupMan::add(int type,int x,int y)
{
	cPowerup *temp=new cPowerup();

	temp->setposx(x);
	temp->setposy(y);
	temp->setsprite(&gfx_sprite[type-1]);
	temp->settype(type);
	
	lPowerups.push_back(*temp);
	delete temp;
}

void cPowerupMan::render(SDL_Surface *screen)
{
	LISTPOWERUPS::iterator i;

	for(i=lPowerups.begin();i!=lPowerups.end();++i)
	{
		i->render(screen,&gfx_sprite[i->gettype()-1]);
	}
}

void cPowerupMan::update()
{
	cPlayerMan	*pPlayerMan=cPlayerMan::getInstance();
	cUtil		*pUtil=cUtil::getInstance();
	cSound		*pSound=cSound::getInstance();
	cSystem		*pSystem=cSystem::getInstance();

	LISTPOWERUPS::iterator i;

	for(i=lPowerups.begin();i!=lPowerups.end();++i)
	{
		i->update();
		
		//das teil ist aus dem bildschirm also löschen wir es
		if(i->getposy()>480)
		{
			i=lPowerups.erase(i);
			if(i==lPowerups.end())
				return ;
		}

		///jetzt überprüfen wir ob das Teil mit einem Spieler kollidiert
		if(pSystem->getgameover())
			return;

		for(int p=0;p<pPlayerMan->getnum();p++)
		{
		if(pUtil->collision(i->getcolrect(&gfx_sprite[i->gettype()-1],0,0),pPlayerMan->getcolrect(p,5,5)))
		{
				pPlayerMan->addscore(p,1000);
				pSound->playwav(4);
				switch(i->gettype())
				{
					case POWERUP_SCORE:
						pPlayerMan->addscore(p,10000);
						break;
					case POWERUP_GUN:
						pPlayerMan->addpwgun(p);
						break;
					case POWERUP_LIFE:
						pPlayerMan->addpwlife(p);
						break;
					case POWERUP_SPECIAL:
						pPlayerMan->addpwspecial(p);
						break;
				}
				i=lPowerups.erase(i);
				if(i==lPowerups.end())
					return;
			}
		}
	}
}
