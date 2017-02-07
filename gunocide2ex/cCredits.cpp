/***************************************************************************
                          cCredits.cpp  -  description
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
#include "cCredits.h"
#include "cFont.h"
#include "cUtil.h"
#include "mmgr.h"

cCredits::cCredits()
{
	numlines=0;
	text=NULL;
	posy=480;
}

cCredits::~cCredits()
{
	if(text)
	{
		delete [] text;
		text=NULL;
	}
}


cCredits* cCredits::instance=NULL;

cCredits* cCredits::getInstance()
{
	if(instance==NULL)
	{
		instance=new cCredits();
		return instance;
	}
	else return instance;
}

void cCredits::delInstance()
{
	if(instance)
	{
		delete instance;
		instance=NULL;
	}
}

///einladen der credits-datei
bool cCredits::load()
{
	FILE	*file;
	char	buffer[256];
	int		size=0;

	file=fopen("/usr/local/games/gunocide2ex/credits","r");
	if(!file)
	{
		printf("hey....where's the credit file?\n");
		return false;
	}

	//wieviele zeilen hat die datei?
	while(!feof(file))
	{
		fgets(buffer,256,file);
		numlines++;
	}

	if(numlines<=0)
	{
		printf("hmpf....no content in the credits file??\n");
		fclose(file);
		return false;
	}

	text=new TEXTLINE[numlines];

	rewind(file);

	//die ganze datei einlesen
	for(int i=0;i<numlines;i++)
	{
		fgets(buffer,256,file);
		text[i].line=strdup(buffer);
		//jetzt fixen wir noch das letzte zeichen, da ansonsten
		///das steuerzeichen auf dem bildschirm ausgegeben wird...das sieht beschissen aus
		size = strlen(text[i].line);
		if(size>=2)
		{
			text[i].line[size-1]='\0';
			text[i].line[size-2]='\0';
		}
		memset(buffer,0,sizeof(buffer));
	}


	fclose(file);
	return true;
}

void cCredits::update()
{
	cUtil	*pUtil=cUtil::getInstance();

	posy-=(int)pUtil->sync(55);

	if(posy<-((numlines*15)+480))
		posy=480;
}

void cCredits::render(SDL_Surface *screen)
{
	cFont	*pFont=cFont::getInstance();
	int		posx=0;

	pFont->setcolor(0,0,0);

	for(int i=0;i<numlines;i++)
	{
		if(strlen(text[i].line)<2)
			continue;


		posx=320-((strlen(text[i].line)*6)/2);

		pFont->print(screen,posx,posy+(i*15),text[i].line);
	}

	pFont->setcolor(255,255,255);
}
