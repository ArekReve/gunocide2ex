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

#include "cStage.h"
#include "cEnemyMan.h"
#include "mmgr.h"

cStage::cStage()
{
	gfx_background=NULL;
}

cStage::~cStage()
{
	if(gfx_background)delete gfx_background;
}

///einladen einer Stage
bool cStage::load(char *path,cEnemyMan *pEnemyMan)
{
	char 	buffer[256];
	int	temp=0;
	FILE 	*file;

	int	tx=0,ty=0;
	int	ttyp=0,atyp=0;
	int	typ_powerup=0;
	int	index=0;
	int	balive=0;

	//Datei öffnen und false zurückgeben wenn es schief geht
	file=fopen(path,"r");
	if(!file)
	{
		printf("failed to open stage file:%s\n",path);
		return false;
	}

	rewind(file);
	//einlesen des Hintergrundes
	fscanf(file,"%i",&temp);

	sprintf(buffer,"/usr/local/games/gunocide2ex/gfx/bgs/stage%i.bmp\0",temp);
	gfx_background=new cSprite();
	if(!gfx_background->loadBMP(buffer))
	{
		printf("failed to load background graphics %s\n",buffer);
		fclose(file);
		return false;
	}

	///einladen der Gegnerdaten und dem EnemyManager hinzufügen
	while(!feof(file))
	{
		fscanf(file,"%i",&tx);		//X-Koordinate
		fscanf(file,"%i",&ty);		//Y-Koordinate
  		fscanf(file,"%i",&ttyp);	//Gegner-Typ
		fscanf(file,"%i",&atyp);	//Angriffstyp?? wird nicht mehr gebraucht
		fscanf(file,"%i",&typ_powerup); //PowerUp-Typ
		fscanf(file,"%i",&balive);	//gibts den Gegner eigentlich? Hätte man damals schöner machen sollen ;)

		if(balive==1)
		{
			pEnemyMan->add(index,tx,ty,ttyp,typ_powerup);
			index++;
		}
	}

	fclose(file);
	return true;
}

///zeichnen der Stage
void cStage::render(SDL_Surface *screen)
{
	if(gfx_background)gfx_background->render(screen,0,0);
}

cStage* cStage::instance=NULL;

cStage* cStage::getInstance()
{
	if(instance==NULL)
	{
		instance=new cStage();
		return instance;
	}
	else return instance;
}

void cStage::delInstance()
{
	if(instance)
	{
		delete instance;
		instance=NULL;
	}
}
