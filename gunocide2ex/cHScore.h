/***************************************************************************
                          cHScore.h  -  description
                             -------------------
    begin                : Sun Dec 2 2001
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
#ifndef CHSCORE_H
#define CHSCORE_H

#include <stdlib.h>
#include <memory.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <string.h>

typedef struct struct_ScoreEntry
{
	char 		name[10];			//Name des Spielers
	long int    score;			//Anzahl der Punkte
	int			waves;			//Anzahl der überlebten Stages
	int			hitratio;		//Trefferrate
}SCOREENTRY;

class cHScore
{
public:
	cHScore();
	~cHScore();

	bool		load();
    bool		save();

	void		render(SDL_Surface *screen);
	void		update();
	void		checkfornewentry(long tscore,int ratio,int waves,int gmode);
	void		setmode(int mode);
	bool		entername();

	static cHScore* getInstance();
	static void		delInstance();	
private:
	bool		createdummytables();

	static cHScore*		instance;
	SCOREENTRY			score[3][10];
	int					showmode;
	bool				bnewscore;
	int					namepos;
	int					scorepos;
};
#endif
