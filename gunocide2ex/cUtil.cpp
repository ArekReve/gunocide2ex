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

#include "cUtil.h"
#include "mmgr.h"

cUtil::cUtil()
{
	LastSyncTime=0;
	SyncTime=0;
	SyncDiff=0;
	PassedTime=0;
}

cUtil::~cUtil()
{
}

///Zeit zum synchronisieren wird aktualisiert
void cUtil::updatetime()
{
	LastSyncTime=SyncTime; // vorherige SyncZeit

	SyncTime=SDL_GetTicks(); //updaten der SyncZeit

	SyncDiff=SyncTime-LastSyncTime; // Zeitdifferenz

/*	PassedTime+=SyncDiff;
	if(PassedTime>=1000)
	{
		PassedTime=0;
	}*/
}

float cUtil::sync(float Faktor)
{
	return (SyncDiff * (Faktor / 1000.0f));
}

cUtil* cUtil::instance=NULL;

cUtil* cUtil::getInstance()
{
	if(instance==NULL)
	{
		instance=new cUtil();
		return instance;
	}
	else return instance;
}

void cUtil::delInstance()
{
	if(instance)
	{
		delete instance;
		instance=NULL;
	}
}

///Kollisionsabfrage
bool cUtil::collision(SDL_Rect objekt1,SDL_Rect objekt2)
{
	if (objekt1.h < objekt2.y) return false;
	if (objekt1.y > objekt2.h) return false;

	if (objekt1.w < objekt2.x) return false;
	if (objekt1.x> objekt2.w) return false;

	return true;
}

///liest einen wert aus einer config-datei
char* cUtil::getconfigvalue(char *path,char *value)
{
	FILE	*file;
	char	buffer[512];
	char	*token;

	file=fopen(path,"r");
	if(!file)
	{
		printf("config file: %s doesn't exist\n",path);
		return NULL;
	}

	while(!feof(file))
	{
		memset(buffer,0,sizeof(buffer));
		
		if(fgets(buffer,100,file)!=NULL)
		{
			if(buffer[0]!='#')
			{
				token=strtok(buffer,"=");
				if(strcmp(value,token)==0)
				{
					token=strtok(NULL,"=");
					return token;
				}
			}
		}
		
	}
	fclose(file);
	return NULL;
}

void cUtil::drawpixel(SDL_Surface *screen, int x, int y,Uint8 R, Uint8 G,Uint8 B)
{
    Uint32 color = SDL_MapRGB(screen->format, R, G, B);

    if ( SDL_MUSTLOCK(screen) ) {
        if ( SDL_LockSurface(screen) < 0 ) {
            return;
        }
    }
    switch (screen->format->BytesPerPixel) {
        case 1: { /* vermutlich 8 Bit */
Uint8 *bufp;

            bufp = (Uint8 *)screen->pixels + y*screen->pitch + x;
            *bufp = color;
        }
        break;

        case 2: { /* vermutlich 15 Bit oder 16 Bit */
Uint16 *bufp;

            bufp = (Uint16 *)screen->pixels + y*screen->pitch/2 + x;
            *bufp = color;
        }
        break;

        case 3: { /* langsamer 24-Bit-Modus, selten verwendet */
Uint8 *bufp;

            bufp = (Uint8 *)screen->pixels + y*screen->pitch + x * 3;
            if(SDL_BYTEORDER == SDL_LIL_ENDIAN) {
                bufp[0] = color;
                bufp[1] = color >> 8;
                bufp[2] = color >> 16;
            } else {
                bufp[2] = color;
                bufp[1] = color >> 8;
                bufp[0] = color >> 16;
            }
        }
        break;

        case 4: { /* vermutlich 32 Bit */
Uint32 *bufp;

            bufp = (Uint32 *)screen->pixels + y*screen->pitch/4 + x;
            *bufp = color;
        }
        break;
    }
    if ( SDL_MUSTLOCK(screen) ) {
        SDL_UnlockSurface(screen);
    }
}
