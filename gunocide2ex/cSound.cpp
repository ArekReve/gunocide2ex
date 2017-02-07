/***************************************************************************
                          cSound.cpp  -  description
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
#include "cSound.h"
#include "mmgr.h"
#include "cUtil.h"

cSound::cSound()
{
	for(int index=0;index<MAX_SOUNDS;index++)
	{
		sample[index]=NULL;
	}
	music_playing=false;
	music_bg=NULL;
	sound_on=true;
}

cSound::~cSound()
{
	for(int index=0;index<MAX_SOUNDS;index++)
	{
		if(sample[index]!=NULL)
		{
			Mix_FreeChunk(sample[index]);
			sample[index]=NULL;
		}
	}
	close();
}

cSound* cSound::instance=NULL;

cSound* cSound::getInstance()
{
	if(instance==NULL)
	{
		instance=new cSound();
		return instance;
	}
	else return instance;
}

void cSound::delInstance()
{
	if(instance)
	{
		delete instance;
		instance=NULL;
	}
}

///Initialisierung von FMOD
///gibt false zurück wenn etwas schief geht
bool cSound::init()
{
	cUtil	*pUtil=cUtil::getInstance();
	char	*system;

	system=pUtil->getconfigvalue("/usr/local/games/gunocide2ex/config.cfg","SOUNDSYSTEM");

	///welches soundsystem soll benutzt werden?
	if(strcmp(system,"0\n")==0)
	{
		printf("sound system switched off\n");
		sound_on=false;
		return true;
	}
	//initialisierung
	if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096) < 0) {
		printf("couldn't open audio: %s\n", SDL_GetError());
		return false;
	}
	else printf("sound system initialized\n");

	Mix_AllocateChannels(32);

	return true;
}

///spielt eine ogg-datei ab
bool cSound::playogg(const char *path)
{
	if(!sound_on)
		return true;

	music_bg=Mix_LoadMUS(path);
	if(!music_bg)
	{
		printf("Could not load ogg %s: %s\n",path,SDL_GetError());
		return false;
	}

	Mix_PlayMusic(music_bg,-1);
	    music_playing=true;
	return true;
}

void cSound::close()
{
	if(!sound_on)
		return;

	stopogg();
	Mix_CloseAudio();
	printf("sound system closed\n");
}

///hält ein ogg-file an
void cSound::stopogg()
{
	if(!sound_on)
		return;

	if(music_bg!=NULL && !Mix_PlayingMusic())
		return;

	Mix_FreeMusic(music_bg);
	music_bg=NULL;
	music_playing=false;
}

///einladen einer WAV-Datei
bool cSound::loadwav(int index,char *path)
{
	if(!sound_on)
		return true;

	if(index>=MAX_SOUNDS)
	{
		printf("could not load sample %s cause your index is >%i\n",path,MAX_SOUNDS);
		return false;
	}

	sample[index]=Mix_LoadWAV(path);

	if(sample[index]==NULL)
	{
		printf("Could not load sample %s:\n",path,SDL_GetError());
		return false;
	}
	else printf("loaded succesfully sample %s\n",path);
	return true;
}

///abspielen einer WAV-Datei
void cSound::playwav(int index)
{
	if(!sound_on)
		return;

	if(index<MAX_SOUNDS)
		Mix_PlayChannel(-1, sample[index], false);
}

bool cSound::musicplaying()
{
	return music_playing;
}
