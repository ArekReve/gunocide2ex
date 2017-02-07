/***************************************************************************
                          cFont.cpp  -  description
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
#include "cFont.h"
#include "mmgr.h"

cFont::cFont()
{
	font=NULL;
}

cFont::~cFont()
{
	TTF_CloseFont(font);
}

cFont* cFont::instance=NULL;

cFont* cFont::getInstance()
{
	if(instance==NULL)
	{
		instance=new cFont();
		return instance;
	}
	else return instance;
}

void cFont::delInstance()
{
	if(instance)
	{
		delete instance;
		instance=NULL;
	}
}

///initialisiert die TTF-Engine von SDL
bool cFont::initttf()
{
	if ( TTF_Init() < 0 )
	{
		printf("couldn't initialize TTF: %s\n",SDL_GetError());
		return false;
	}
	
	printf("initialized TTF\n");
	return true;
}

///lädt einen TTF-Font ein
bool cFont::load(const char *path,int size)
{
	font = TTF_OpenFont(path,size);
	if(font==NULL)
	{
		printf("failed to load font %s with size %i\n",path,size);
		return false;
	}
	
	printf("loaded succesfully font %s with size %i\n",path,size);
	return true;
}

///rendert einen FONT-String auf dem Bildschirm
void cFont::print(SDL_Surface *screen,int posx,int posy,const char *fmt, ...)
{
	SDL_Surface	*temp=NULL;
	SDL_Rect 	rc;
	static char buffer[1024];
	va_list args;

	memset (buffer, 0, sizeof(buffer));

	va_start(args, fmt);
	vsprintf(buffer, fmt, args);
	va_end(args);

	temp=TTF_RenderText_Solid(font,buffer,color);

	rc.x=posx;
	rc.y=posy;
	rc.w=temp->w;
	rc.h=temp->h;

	if(SDL_BlitSurface(temp,NULL,screen,&rc)<0)
		printf("Couldn't blit text to display: %s\n",SDL_GetError());
	SDL_FreeSurface(temp);
}

///rendert einen Font-String mit einem Schatten auf dem Bildschirm
void cFont::print_shaded(SDL_Surface *screen,int posx,int posy,const char *fmt, ...)
{
	SDL_Surface	*temp=NULL;
	SDL_Rect 	rc;
	static char buffer[1024];
	va_list args;

	memset (buffer, 0, sizeof(buffer));

	va_start(args, fmt);
	vsprintf(buffer, fmt, args);
	va_end(args);

	temp=TTF_RenderText_Shaded(font,buffer,color,shader);

	rc.x=posx;
	rc.y=posy;
	rc.w=temp->w;
	rc.h=temp->h;

	if(SDL_BlitSurface(temp,NULL,screen,&rc)<0)
		printf("Couldn't blit text to display: %s\n",SDL_GetError());
	SDL_FreeSurface(temp);
}

///setzt die hauptfarbe für die schrift
void cFont::setcolor(int r,int g,int b)
{
	color.r=r;
	color.g=g;
	color.b=b;
}

///setzt die schattenfarbe für die schrift
void cFont::setshader(int r,int g,int b)
{
	shader.r=r;
	shader.g=g;
	shader.b=b;
}
