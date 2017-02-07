/***************************************************************************
                          cEffectMan.cpp  -  description
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
#include "mmgr.h"

/**************************************************************************
cEffect-Klasse
***************************************************************************/
cEffect::cEffect()
{
	posx=posy=0;
	type=0;
}

cEffect::~cEffect()
{
}

void cEffect::add(int type,int posx,int posy)
{
	cEffectMan 	*pEffectMan=cEffectMan::getInstance();
	cSpriteAnim	*pgfx_sprite=pEffectMan->getGFX();

	this->type=type;
	this->posx=posx;
	this->posy=posy;
	anim_sprite.setMaxNum(pgfx_sprite->getMaxNum());
	anim_sprite.setDelay(100);
	anim_sprite.setLooped(false);
}

///gibt false zurück wenn die animation bzw. effekt zu ende ist
bool cEffect::update()
{
	if(anim_sprite.getAct()==anim_sprite.getMaxNum()-1)
		return false;

	anim_sprite.update();	
	return true;
}

void cEffect::render(SDL_Surface *screen)
{
	cEffectMan 	*pEffectMan=cEffectMan::getInstance();
	cSpriteAnim	*pgfx_sprite=pEffectMan->getGFX();

	pgfx_sprite->render(screen,&anim_sprite,posx,posy);
}
/**************************************************************************
cEffectMan-Klasse
***************************************************************************/
cEffectMan::cEffectMan()
{
	effect_flash=false;
}

cEffectMan::~cEffectMan()
{
	lEffects.clear();
	lParticles.clear();
}

cEffectMan* cEffectMan::instance=NULL;

cEffectMan* cEffectMan::getInstance()
{
	if(instance==NULL)
	{
		instance= new cEffectMan();
		return instance;
	}
	else return instance;
}

void cEffectMan::delInstance()
{
	if(instance)
	{
		delete instance;
		instance=NULL;
	}
}

///einladen aller Grafiken für die Effekte..wird nicht sehr viel sein
bool cEffectMan::load()
{
	if(!gfx_sprite.load("/usr/local/games/gunocide2ex/gfx/exp/exp.txt"))
	{
		printf("effekte konnten nicht eingeladen werden!\n");
		return false;
	}
	gfx_sprite.setColorKey(0,0,0);

	if(!gfx_particle.loadBMP("/usr/local/games/gunocide2ex/gfx/hit/1.bmp"))
		return false;
	
	gfx_particle.setColorKey(0,0,0);

	printf("effect graphics succesfully loaded\n");
	return true;
}

cSpriteAnim* cEffectMan::getGFX()
{
	return &gfx_sprite;
}

///hinzufügen der Liste der verketetten Liste
void cEffectMan::add(int type,int posx,int posy)
{
	cEffect 		*pEffect=NULL;
	cParticleSys	*pParticleSys=NULL;


	if(type==EFFECT_EXPLOSION)
	{
		pEffect=new cEffect();
		pEffect->add(type,posx,posy);

		lEffects.push_back(*pEffect);
		delete pEffect;
	}

	if(type==EFFECT_FLASH)
		effect_flash=true;

	if(type==EFFECT_PEXPLOSION)
	{
		pParticleSys=new cParticleSys();

		pParticleSys->setlifetime(1200);
		pParticleSys->setmaxparticles( 200);
		pParticleSys->setparticlerate(4000);
		pParticleSys->setspeedvar(300,300);
		pParticleSys->setstartvar(20,20);
		pParticleSys->setstartpos(posx,posy);
		
		lParticles.push_back(*pParticleSys);
		delete pParticleSys;
	}

	if(type==EFFECT_PHIT)
	{
		pParticleSys=new cParticleSys();

		pParticleSys->setlifetime(500);
		pParticleSys->setmaxparticles(50);
		pParticleSys->setparticlerate(1000);
		pParticleSys->setspeedvar(200,200);
		pParticleSys->setstartvar(5,5);
		pParticleSys->setstartpos(posx,posy);
		
		lParticles.push_back(*pParticleSys);
		delete pParticleSys;
	}
}

///aktualisieren der Effekte
void cEffectMan::update()
{
	LISTEFFECTS::iterator 		i;
	LISTPARTICLESYS::iterator	p;

	///Explosionsanimation
    for(i=lEffects.begin();i!=lEffects.end();++i)
	{
		if(!i->update())
			i=lEffects.erase(i);
	}

    for(p=lParticles.begin();p!=lParticles.end();++p)
	{
		p->update();

		if(!p->active())
		{
			p=lParticles.erase(p);
		}
	}


	///flash-effekt wieder ausschalten
	effect_flash=false;
}

///zeichnen der Effekte
void cEffectMan::render(SDL_Surface *screen)
{
	LISTEFFECTS::iterator 		i;
	LISTPARTICLESYS::iterator	p;

	///Explosionsanimation
    for(i=lEffects.begin();i!=lEffects.end();++i)
	{
		i->render(screen);
	}

    for(p=lParticles.begin();p!=lParticles.end();++p)
	{
		p->render(screen,&gfx_particle);
	}

	if(effect_flash)
		SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format,255,255,255));
}
