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
#ifndef CCREDITS_H
#define CCREDITS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <SDL/SDL.h>

typedef struct textline
{
	char*	line;
}TEXTLINE;

class cCredits
{
public:
	cCredits();
	~cCredits();

	bool	load();
	void	update();
	void	render(SDL_Surface *screen);

	static cCredits*	getInstance();
	static void			delInstance();
private:
	static cCredits*	instance;

	int					numlines;
	int					posy;
	TEXTLINE			*text;
};
#endif
