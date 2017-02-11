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

#include "cWeaponMan.h"
#include "cEnemyMan.h"
#include "cWeapon.h"
#include "cEnemy.h"
#include "cPlayerMan.h"
#include "cSound.h"
#include "cEffects.h"
#include "cPowerup.h"
#include "cSystem.h"
#include "mmgr.h"

cWeaponMan::cWeaponMan()
{
}

cWeaponMan::~cWeaponMan()
{
	///komplette Waffenliste l�schen
	lWeapon.clear();
}

///l�dt alle Grafiken f�r die Waffen
bool cWeaponMan::load()
{
	char 	buffer[256];
	int 	i=0;

	///einladen der Waffen f�r die Gegner
	for(i=0;i<7;i++)
	{
		sprintf(buffer,"/usr/local/games/gunocide2ex/gfx/schuss/%i/ani.txt",i);
		if(!gfx_sprite[i].load(buffer))
			return false;
		gfx_sprite[i].setColorKey(0,0,0);
	}

	///einladen der Waffen f�r die Spieler
	///i-6 da die Dateien mit 1 Anfangen, nicht mit 0
	for(i=7;i<18;i++)
	{
		sprintf(buffer,"/usr/local/games/gunocide2ex/gfx/schuss/norm/%is.txt",i-6);
		if(!gfx_sprite[i].load(buffer))
			return false;
		gfx_sprite[i].setColorKey(0,0,0);
	}

	///einladen der Special f�r die Spieler
	///i-6 da die Dateien mit 1 Anfangen, nicht mit 0
	for(i=18;i<23;i++)
	{
		sprintf(buffer,"/usr/local/games/gunocide2ex/gfx/schuss/shunter/%i.txt",i-17);
		if(!gfx_sprite[i].load(buffer))
			return false;
		gfx_sprite[i].setColorKey(0,0,0);
	}

	printf("weapon graphics succesfully loaded\n");
	return true;
}

///Aktualisierung jeden Schusses
void cWeaponMan::update()
{
	cEnemy 		*pEnemy=NULL;
	cPlayerMan	*pPlayerMan=cPlayerMan::getInstance();
	cEnemyMan	*pEnemyMan=cEnemyMan::getInstance();
	cSound		*pSound=cSound::getInstance();
	cEffectMan	*pEffectMan=cEffectMan::getInstance();
	cPowerupMan	*pPowerupMan=cPowerupMan::getInstance();
	cSystem		*pSystem=cSystem::getInstance();
	LISTWEAPON::iterator akt_weapon;


	for(akt_weapon=lWeapon.begin();akt_weapon!=lWeapon.end();++akt_weapon)	
		{
		akt_weapon->update();

		///ist der Schuss aus dem Bildschirm??
		if(akt_weapon->getposy()>480 || akt_weapon->getposy()<-50)
		{
			akt_weapon=lWeapon.erase(akt_weapon);
			if(akt_weapon==lWeapon.end())
			return;
		}

		if(akt_weapon->getowner()==OWNER_ENEMY)
		{
			for(int p=0;p<pPlayerMan->getnum();p++)
			{
				///kollisiosnabfrage nur wenn der spieler noch lebt
				if(!pPlayerMan->alive(p))
					continue;

				///Kollisionsabfrage mit dem Spieler
				if(collision(akt_weapon->getcolrect(&gfx_sprite[akt_weapon->getsprite()],0,0),
						pPlayerMan->getcolrect(p,10,10)) && !pSystem->getgameover())
				{
					//sound HIT abspielen
					if(pPlayerMan->getshieldstate(p)==0)
					{
						pSound->playwav(0);
						pEffectMan->add(EFFECT_EXPLOSION,pPlayerMan->getposx(p),pPlayerMan->getposy(p));
						pEffectMan->add(EFFECT_PEXPLOSION,pPlayerMan->getposx(p)+50,pPlayerMan->getposy(p)+25);
						pEffectMan->add(EFFECT_FLASH,0,0);

						pPlayerMan->dellife(p);
					}
					else pSound->playwav(1);

					akt_weapon=lWeapon.erase(akt_weapon);
					if(akt_weapon==lWeapon.end())
						return;
				}
			}
		}
		else
		{
			//Kollisionsabfrage mit den Gegnern
			for(int i=0;i<MAX_ENEMIES;i++)
			{
				pEnemy=pEnemyMan->getenemy(i);

				if(pEnemy->getalive())
				{
					if(collision(akt_weapon->getcolrect(&gfx_sprite[akt_weapon->getsprite()],0,0),
							pEnemyMan->getcolrect(i)))
					{
						//sound HIT abspielen
						pSound->playwav(1);

						///dem spieler den treffer gutschreiben
						pPlayerMan->addshot_treffer(akt_weapon->getowner());

						////////////////////////////////////////////////////////////////////////
						//energie abziehen und �berpr�fen ob er tot ist
						pEnemy->setenergy(pEnemy->getenergy()-akt_weapon->getenergy());
						
						if(pEnemy->getenergy()<=0)
						{
							//hat er ein Powerup, wenn ja rein damit
							if(pEnemy->getpowerup()!=0)
							{
								pPowerupMan->add(pEnemy->getpowerup(),pEnemy->getposx(),pEnemy->getposy());
							}

							pEnemy->setalive(false);
							pEffectMan->add(EFFECT_EXPLOSION,pEnemy->getposx(),pEnemy->getposy());
							pEffectMan->add(EFFECT_PEXPLOSION,pEnemy->getposx()+50,pEnemy->getposy()+50);
							pEffectMan->add(EFFECT_FLASH,0,0);
							pSound->playwav(0);
							pPlayerMan->addscore(akt_weapon->getowner(),EnemyVars::Score[pEnemy->getsprite()]);
						}
						else
						{
							pPlayerMan->addscore(akt_weapon->getowner(),10);
							pEffectMan->add(EFFECT_PHIT,akt_weapon->getposx(),akt_weapon->getposy());
						}

						//2. phase eines specials ausf�hren
						if(akt_weapon->getsprite()==18 || akt_weapon->getsprite()==21)
							addspecial(akt_weapon->getsprite(),akt_weapon->getposx(),akt_weapon->getposy());

						akt_weapon=lWeapon.erase(akt_weapon);
						if(akt_weapon==lWeapon.end())
								return;
					}
				}
			}
		}
	}
}

///zeichnen aller Sch�sse
void cWeaponMan::render(SDL_Surface *screen)
{
  LISTWEAPON::iterator akt_weapon;


	for(akt_weapon=lWeapon.begin();akt_weapon!=lWeapon.end();++akt_weapon)
	{
		akt_weapon->render(screen,&gfx_sprite[akt_weapon->getsprite()]);
	}

}

///einen neuen Schuss der verketten Liste hinzuf�gen
void cWeaponMan::add(int type,int posx,int posy,int dirx,int diry,int owner,int energy)
{
	cWeapon *temp;
	
	temp=new cWeapon();
	temp->setsprite(type);
	temp->setposx(posx);
	temp->setposy(posy);
	temp->setdirx(dirx);
	temp->setdiry(diry);
	temp->setowner(owner);
	temp->setenergy(energy);

	///die liste ist leer also f�gen wir das teil am anfang ein
	lWeapon.push_back(*temp);
	delete temp;
}


///Kollisionsabfrage
bool cWeaponMan::collision(SDL_Rect objekt1,SDL_Rect objekt2)
{
	if (objekt1.h < objekt2.y)
		return false;
	if (objekt1.y > objekt2.h)
		return false;

	if (objekt1.w < objekt2.x)
		return false;
	if (objekt1.x> objekt2.w)
		return false;

	return true;
}

cWeaponMan* cWeaponMan::instance=NULL;

cWeaponMan* cWeaponMan::getInstance()
{
	if(instance==NULL)
	{
		instance = new cWeaponMan();
		return instance;
	}
	else return instance;
}

void cWeaponMan::delInstance()
{
	if(instance)
	{
		delete instance;
		instance=NULL;
	}
}

///f�hrt die specials des spielers aus wenn er trifft
///z.b. gehen die sch�sse auseinander oder os
void cWeaponMan::addspecial(int type,int posx,int posy)
{
	if(type==18)//banshee
	{
		//links
		add(19,posx,posy-25,-WeaponVars::Speed[16],0,OWNER_PLAYER1,WeaponVars::Energy[19]);
		//rechts
		add(20,posx,posy-25,WeaponVars::Speed[16],0,OWNER_PLAYER1,WeaponVars::Energy[20]);
	}
	if(type==21)
	{
		//rechts oben
		add(20,posx,posy-25,WeaponVars::Speed[18],-WeaponVars::Speed[18],OWNER_PLAYER1,WeaponVars::Energy[20]);
		//rechts unten
		add(20,posx,posy-25,WeaponVars::Speed[18],WeaponVars::Speed[18],OWNER_PLAYER1,WeaponVars::Energy[20]);
		//links unten
		add(20,posx,posy-25,-WeaponVars::Speed[18],WeaponVars::Speed[18],OWNER_PLAYER1,WeaponVars::Energy[20]);
		//links oben
		add(20,posx,posy-25,-WeaponVars::Speed[18],-WeaponVars::Speed[18],OWNER_PLAYER1,WeaponVars::Energy[20]);
	}
}

/************************************************************************
Variablen f�r die Geschwindigkeit der einzelnen Sch�sse
*************************************************************************/
int WeaponVars::Speed[19]={
						300,//enforcer
						500,//guardian
						300,//nemesis
						350,//stormlord
						300,//bulldog
						500,//widow
						600,//black
						-300, ///banshee 1
						-300, ///banshee 2
						-300,///banshee 3
						-300,///rage 1
						-300,///rge2
						-300,///rage3
						-300,///banshee1
						-300,///banshee 2
						-300,///banshee3
						-400,///special banshee
						-400,///special rage
						-400,///special fury
						};

int WeaponVars::Energy[23]={
						0,//enforcer	|0
						0,//guardian	|1
						0,//nemesis		|2
						0,//stormlord	|3
						0,//bulldog		|4
						0,//widow		|5
						0,//black		|6
						1, ///banshee 1	|7
						2, ///banshee 2	|8
						3,///banshee 3	|9
						1,///rage 1		|10
						2,///rge2		|11
						3,///rage3		|12
						1,///banshee1	|13
						2,///banshee 2	|14
						1,///banshee3	|15
						50,///special banshee	|16
						50,///special rage		|17
						50,///special fury		|18
						50,
						50,
						50,
						50
						};
