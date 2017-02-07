/***************************************************************************
                          cSound.h  -  description
                             -------------------
    begin                : Wed Nov 21 2001
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
#ifndef CSOUND_H
#define CSOUND_H

#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL_mixer.h>


#define MAX_SOUNDS		10

class cSound
{
public:
	cSound();
	~cSound();
	
	static cSound* 	getInstance();
	static void		delInstance();

	bool			init();
	void				close();
	bool			playogg(const char *path);
	void				stopogg();
	bool			loadwav(int index,char *path);
	void				playwav(int index);
	bool			musicplaying();
private:
	static cSound	*instance;
	
	Mix_Music		*music_bg;
	bool			music_playing;
	bool			sound_on;
	Mix_Chunk 		*sample[MAX_SOUNDS];
};
#endif
