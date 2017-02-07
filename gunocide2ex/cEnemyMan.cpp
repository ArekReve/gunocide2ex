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

#include <stdlib.h>
#include "cWeaponMan.h"
#include "cEnemy.h"
#include "cEnemyMan.h"
#include "cPlayerMan.h"
#include "cSound.h"
#include "mmgr.h"

cEnemyMan::cEnemyMan()
{
	pEnemy=NULL;
	benemyattack=false;
	enemyattack_delay=0;
}

cEnemyMan::~cEnemyMan()
{
	if(pEnemy)
	{
		delete [] pEnemy;
		pEnemy=NULL;
	}
}

///einladen aller Spielergrafiken
bool cEnemyMan::load()
{
	char buffer[256];

	//falls es fehlschlägt eine Grafik zu laden, gehen wir direkt raus!
	for(int i=0;i<7;i++)
	{
		sprintf(buffer,"/usr/local/games/gunocide2ex/gfx/gegner/%i/%i.txt\0",i,i);
		if(!gfx_sprite[i].load(buffer))
			return false;

		gfx_sprite[i].setColorKey(0,0,0);
	}
	printf("enemy graphics succesfully loaded\n");
	return true;
}

///erstellen des Arrays an Gegnern
///ich gehe über den Pointer so das der Desktrutor bzw. Konstruktur
///für mich die Initialisierung und so weiter...anders ist's zu umständlich ;)
bool cEnemyMan::create()
{
	if(!pEnemy)
	{
		pEnemy=new cEnemy[MAX_ENEMIES];
	}
	return false;
}

///das Array an Gegnern aus dem Speicher löschen
bool cEnemyMan::destroy()
{
	if(pEnemy)
	{
		delete [] pEnemy;
		pEnemy=NULL;
		return true;
	}

	return false;
}

///einen Gegner hinzufügen
void cEnemyMan::add(int index,int x,int y,int typ,int pwuptyp)
{
	pEnemy[index].setposx(x);
	pEnemy[index].setposy(y);
	pEnemy[index].setsprite(typ);
	pEnemy[index].setpowerup(pwuptyp);
	pEnemy[index].setalive(true);
	pEnemy[index].setanim(&gfx_sprite[typ]);
	pEnemy[index].setdelayshot(500);
	pEnemy[index].setenergy(EnemyVars::Energy[typ]);
}

///aktualisieren aller Gegner
void cEnemyMan::update()
{
	cWeaponMan	*pWeaponMan=cWeaponMan::getInstance();
	cPlayerMan	*pPlayerMan=cPlayerMan::getInstance();
	cSound		*pSound=cSound::getInstance();
	int 		dirx=0;
	long int 	act_time=0;

	for(int i=0;i<MAX_ENEMIES;i++)
	{
		if(pEnemy[i].getalive())
		{
			pEnemy[i].update();
			//soll der Gegner schiessen?
			if(pEnemy[i].getshot() && ((rand()%250+1)==1))
			{

				pEnemy[i].setshot(false);
				pSound->playwav(5);
				
				switch(pEnemy[i].getsprite())
				{
				default:
					pWeaponMan->add(pEnemy[i].getsprite(),
							pEnemy[i].getposx()+(pEnemy[i].getwidth(&gfx_sprite[pEnemy[i].getsprite()])/2),
							pEnemy[i].getposy()+(pEnemy[i].getheight(&gfx_sprite[pEnemy[i].getsprite()])/2),
							0,
							WeaponVars::Speed[pEnemy[i].getsprite()],
							OWNER_ENEMY,WeaponVars::Energy[0]);
					break;
				case ENEMY_BULLDOG:
					pWeaponMan->add(pEnemy[i].getsprite(),//links
							pEnemy[i].getposx()+29,
							pEnemy[i].getposy()+37,
							0,
							WeaponVars::Speed[pEnemy[i].getsprite()],
							OWNER_ENEMY,WeaponVars::Energy[0]);
					pWeaponMan->add(pEnemy[i].getsprite(), ///rechts
							pEnemy[i].getposx()+55,
							pEnemy[i].getposy()+37,
							0,
							WeaponVars::Speed[pEnemy[i].getsprite()],
							OWNER_ENEMY,WeaponVars::Energy[0]);
					break;
				case ENEMY_STORMLORD:
					pWeaponMan->add(pEnemy[i].getsprite(),//links
							pEnemy[i].getposx()+11,
							pEnemy[i].getposy()+32,
							-100,
							WeaponVars::Speed[pEnemy[i].getsprite()],
							OWNER_ENEMY,WeaponVars::Energy[0]);
					pWeaponMan->add(pEnemy[i].getsprite(), ///rechts
							pEnemy[i].getposx()+51,
							pEnemy[i].getposy()+32,
							100,
							WeaponVars::Speed[pEnemy[i].getsprite()],
							OWNER_ENEMY,WeaponVars::Energy[0]);
					break;
				case ENEMY_BLACKKNIGHT:
					pWeaponMan->add(pEnemy[i].getsprite(),//links
							pEnemy[i].getposx()+22,
							pEnemy[i].getposy()+39,
							0,
							WeaponVars::Speed[pEnemy[i].getsprite()],
							OWNER_ENEMY,WeaponVars::Energy[0]);

					pWeaponMan->add(pEnemy[i].getsprite(), ///rechts
							pEnemy[i].getposx()+49,
							pEnemy[i].getposy()+39,
							0,
							WeaponVars::Speed[pEnemy[i].getsprite()],
							OWNER_ENEMY,WeaponVars::Energy[0]);
					break;
				}
			}
		}
	}

	//attack aktualisieren
	if(benemyattack)
	{
		act_time=SDL_GetTicks();
		if(act_time-enemyattack_delay>3000)
		{
			enemyattack_delay=0;
			benemyattack=false;
		}
	}
}


///zeichnen aller Gegner
void cEnemyMan::render(SDL_Surface *screen)
{
	for(int i=0;i<MAX_ENEMIES;i++)
	{
		if(pEnemy[i].getalive())
		{
			pEnemy[i].render(screen,&gfx_sprite[pEnemy[i].getsprite()]);
		}
	}
}

///die Adresse eines bestimmten Enemy-Objektes zurückgeben
cEnemy* cEnemyMan::getenemy(int index)
{
	return &pEnemy[index];
}

///gibt das Kollisionsrechtecke für einen Gegner zurück
SDL_Rect cEnemyMan::getcolrect(int index)

{
	return pEnemy[index].getcolrect(&gfx_sprite[pEnemy[index].getsprite()],0,0);
}

cEnemyMan* cEnemyMan::instance=NULL;

cEnemyMan* cEnemyMan::getInstance()
{
	if(instance==NULL)
	{
		instance=new cEnemyMan();
		return instance;
	}
	else return instance;
}

void cEnemyMan::delInstance()
{
	if(instance)
	{
		delete instance;
		instance=NULL;
	}
}

///geht alle Gegner durch und gibt ein true zurück wenn alle gegner
///tot sind oder wie auch immer
bool cEnemyMan::checkenemies()
{
	for(int i=0;i<MAX_ENEMIES;i++)
	{
		if(pEnemy[i].getalive())
			return false;
	}
	return true;
}

bool cEnemyMan::getenemyattack()
{
	return benemyattack;
}

void cEnemyMan::setenemyattack(bool wert)
{
	benemyattack=wert;
	if(benemyattack)
		enemyattack_delay=SDL_GetTicks();
}
/************************************************************************
Variablen für die Energie der Gegner
*************************************************************************/
int EnemyVars::Energy[7]={
						5,//enforcer
						10,//guardian
						15,//nemesis
						20,//stormlord
						25,//bulldog
						30,//widow
						35,//black
						};

int EnemyVars::Score[7]={
						100,//enforcer
						250,//guardian
						500,//nemesis
						600,//stormlord
						700,//bulldog
						800,//widow
						900,//black
						};
