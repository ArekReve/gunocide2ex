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

#include <time.h>
#include <stdio.h>
#include "cWeaponMan.h"
#include "cEnemyMan.h"
#include "cPowerup.h"
#include "cEffects.h"
#include "cSystem.h"
#include "cSound.h"
#include "cMenu.h"
#include "cKeyboard.h"
#include "cFont.h"
#include "cStage.h"
#include "cUtil.h"
#include "cPlayerMan.h"
#include "cStage.h"
#include "cHScore.h"
#include "mmgr.h"

cSystem::cSystem()
{
	srand((unsigned int)time((time_t *)NULL));
	gamestate=GSTATE_MENU;
	act_stage=1;
	stage_selected=0;
	gamemode=0;
	time_suicide=0;
	time_suicide_delay=0;
	bgameover=false;
	gameover_delay=0;
	bnextwave=false;
	nextwave_delay=0;
	nextwave_time=0;
	bpause=false;
}

cSystem::~cSystem()
{
	cSound::delInstance();
	cUtil::delInstance();
	cKeyboard::delInstance();
	cFont::delInstance();
	cMenu::delInstance();
	SDL_Quit();
}

bool cSystem::init()
{
	char 		buffer[256];

	cUtil		*pUtil=cUtil::getInstance();
	cSound	*pSound=NULL;
	cMenu		*pMenu=NULL;
	cFont		*pFont=NULL;

	initSDL();
	//Fensertitel einstellen
	sprintf(buffer,"Gunocide II EX v%s (c) 2001-02 by polyfrag",G2EX_VERSION);
	SDL_WM_SetCaption(buffer,NULL);

	///Font erstellen und laden
	if(!cFont::initttf())
		return false;
	pFont=cFont::getInstance();
	if(!pFont->load("/usr/local/games/gunocide2ex/arial.ttf",14))
		return false;

	///sound initialisieren
	pSound=cSound::getInstance();
	if(!pSound->init())
		return false;
	if(!loadsounds())
		return false;

	//Menu einladen
	pMenu=cMenu::getInstance();
	if(!pMenu->load())
		return false;

	if(!load())
		return false;

	pUtil->updatetime();
	return true;
}

//Initialisierung der SDL
bool cSystem::initSDL()
{
	cUtil	*pUtil=cUtil::getInstance();
	char	*buffer;

	if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO)<0)
	{
		printf("failed to initialize SDL\n");
		return false;
	}
	
	///vollbild oder fenstermodus	
	buffer=pUtil->getconfigvalue("/usr/local/games/gunocide2ex/config.cfg","SCREEN");
	if(!buffer)
		buffer=strdup("0\n");

	if(strcmp(buffer,"0\n")==0)
		screen = SDL_SetVideoMode(640, 480, 16, SDL_HWSURFACE|SDL_DOUBLEBUF);


	else
		screen = SDL_SetVideoMode(640, 480, 16, SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_FULLSCREEN);
	
   	if(screen==NULL)
	{
		printf("failed to set video mode 640x480x16 hardware/double buffer surface\n");
		return false;
	}

	SDL_ShowCursor(0);
	return true;
}

void cSystem::gameLoop()
{
	cUtil		*pUtil=cUtil::getInstance();
	cPlayerMan	*pPlayerMan=NULL;
	cWeaponMan	*pWeaponMan=NULL;
	cEnemyMan	*pEnemyMan=NULL;
	cEffectMan	*pEffectMan=NULL;
	cMenu		*pMenu=cMenu::getInstance();
	cKeyboard	*pKeyboard=cKeyboard::getInstance();
	cPowerupMan	*pPowerupMan=NULL;
	cStage		*pStage=NULL;

	while(gamestate!=GSTATE_EXIT)
	{
		if(gamestate==GSTATE_MENU)
		{
			pMenu->render(screen);
			pMenu->update();
		}
		else
		{
			pPlayerMan=cPlayerMan::getInstance();
			pWeaponMan=cWeaponMan::getInstance();
			pEnemyMan=cEnemyMan::getInstance();
			pEffectMan=cEffectMan::getInstance();
			pPowerupMan=cPowerupMan::getInstance();
			pStage=cStage::getInstance();


			pStage->render(screen);
			pWeaponMan->render(screen);
			pPowerupMan->render(screen);
			pEnemyMan->render(screen);
			pPlayerMan->render(screen);
			pEffectMan->render(screen);
			sysrender();

			//updaten der Spieler
			if(!bpause)
			{
				pPlayerMan->update();
				pEnemyMan->update();
				pEffectMan->update();
				pWeaponMan->update();
				pPowerupMan->update();
				check_env();
				sysupdate();
			}
			control();

		}//ingame
		SDL_Flip(screen);
		pUtil->updatetime();
		pKeyboard->update();
	}//ende des spielsloops
}

SDL_Surface* cSystem::getScreen()
{
	return screen;
}

///einladen aller SOUNDS
bool cSystem::loadsounds()
{
	cSound	*pSound=cSound::getInstance();
	char	buffer[256];
	
	for(int i=0;i<10;i++)
	{
		sprintf(buffer,"/usr/local/games/gunocide2ex/sfx/%i.wav",i+1);
		if(!pSound->loadwav(i,buffer))
			return false;
	}
	return true;	
}

///Tastensteuerung f�r die Spieler
void cSystem::control()
{
	cKeyboard	*pKeyboard=cKeyboard::getInstance();
	KeyEvent	*kevent=NULL;
	cPlayerMan	*pPlayerMan=NULL;

	cMenu		*pMenu=cMenu::getInstance();
	cSound		*pSound=cSound::getInstance();

	///spiel beenden und ab ins men�
	kevent=pKeyboard->getkey(SDLK_ESCAPE);
	if(kevent && kevent->pressed==1)
	{
		gamestate=GSTATE_MENU;
		pMenu->setmenustate(MSTATE_MAIN);
		cPlayerMan::delInstance();
		cWeaponMan::delInstance();
		cEnemyMan::delInstance();
		cStage::delInstance();
		cEffectMan::delInstance();
		cPowerupMan::delInstance();
		pSound->stopogg();
		pSound->playogg("/usr/local/games/gunocide2ex/sfx/menu.ogg\0");
	
		delete kevent;	
		kevent=NULL;
	} else kevent=NULL;

	///pause aktivieren ja oder nein??
	kevent=pKeyboard->getkey(SDLK_p);
	if(kevent && kevent->pressed==1)
	{
		if(bpause)
			bpause=false;
		else bpause=true;
	
		delete kevent;	
		kevent=NULL;
	} else kevent=NULL;


	///wenn die Pause aktiviert ist, soll der Spieler keinen weiteren
	///Eingaben �ber die Tastatur eingeben k�nnen
	if(bpause || bgameover || gamestate!=GSTATE_INGAME)
		return;

	pPlayerMan=cPlayerMan::getInstance();
	for(int p=0;p<pPlayerMan->getnum();p++)
	{
		///wenn der spieler tot ist, gehen wir einfach zum n�chsten
		if(!pPlayerMan->alive(p))
			continue;
		
		//rechts bewegen
		kevent=pKeyboard->getkey((SDLKey)keymap[p].right);
		if(kevent)
		{
			if(kevent->pressed==1)
				pPlayerMan->setbmoveright(p,true);
			else
				pPlayerMan->setbmoveright(p,false);

			delete kevent;
			kevent=NULL;
		} else kevent=NULL;

		//links		
		kevent=pKeyboard->getkey((SDLKey)keymap[p].left);
		if(kevent)
		{
			if(kevent->pressed==1)
				pPlayerMan->setbmoveleft(p,true);
			else

				pPlayerMan->setbmoveleft(p,false);

			delete kevent;
			kevent=NULL;
		} else kevent=NULL;

		//schuss
		kevent=pKeyboard->getkey((SDLKey)keymap[p].shot);
		if(kevent)
		{
			if(kevent->pressed==1)
			{
				pPlayerMan->setbshot(p,true);
			}
			else
				pPlayerMan->setbshot(p,false);

			delete kevent;
			kevent=NULL;
		} else kevent=NULL;

		///special
		kevent=pKeyboard->getkey((SDLKey)keymap[p].special);
		if(kevent)
		{
			if(kevent->pressed==1)
				pPlayerMan->setbspecial(p,true);
			else
				pPlayerMan->setbspecial(p,false);

			delete kevent;
			kevent=NULL;
		} else kevent=NULL;
	}
}

cSystem* cSystem::instance=NULL;

cSystem* cSystem::getInstance()
{
	if(instance==NULL)
	{
		instance=new cSystem();
		return instance;
	}
	else return instance;
}

void cSystem::delInstance()
{	
	if(instance)
	{
		delete instance;
		instance=NULL;
	}
}

void cSystem::setgamestate(int state)
{
	gamestate=state;
}


///�berpr�ft das Spiel auf den jeweiligen Spielmodus
///und wie es darauf reagieren soll
void cSystem::check_env()
{
	cEnemyMan	*pEnemyMan=cEnemyMan::getInstance();
	cStage		*pStage=cStage::getInstance();
	cPlayerMan	*pPlayerMan=cPlayerMan::getInstance();
	cSound		*pSound=cSound::getInstance();
	char		buffer[256];
	long int	act_time=0;

	///alle gegner sind tot
	if(pEnemyMan->checkenemies() && !bgameover && !bnextwave)
	{
			act_stage++;
			pPlayerMan->addwave(0);


			if(gamemode==GMODE_SUICIDE || gamemode==GMODE_ENDLESS)
			{
				if(act_stage>=((stage_selected*5)+1)*5)
					act_stage=(stage_selected*5)+1;
			}

			if(act_stage>25)
			{
				bgameover=true;
				gameover_delay=SDL_GetTicks();
			}
			else
			{
				bnextwave=true;
				nextwave_delay=SDL_GetTicks();
				if(gamemode==GMODE_SUICIDE)
					nextwave_time=1500;
				else
					nextwave_time=2500;
			}
	}

	if(bnextwave && !bgameover)
	{
		act_time=SDL_GetTicks();
		if(act_time-nextwave_delay>=nextwave_time)
		{
			bnextwave=false;
			nextwave_delay=0;
			sprintf(buffer,"/usr/local/games/gunocide2ex/lvl/stage%i.lvl",act_stage);
			
			cStage::delInstance();
			pStage=cStage::getInstance();
			pEnemyMan->destroy();
			pEnemyMan->create();
			if(!pStage->load(buffer,pEnemyMan))
				return;

			for(int i=0;i<pPlayerMan->getnum();i++)
			{
				pPlayerMan->setshieldstate(i,2);
			}
			
			if(gamemode==GMODE_STORY)
			{
				//die bonus punkte dem spieler gutschreiben
				if(pPlayerMan->getnum()==1)
				{
					pPlayerMan->addscore(0,(float)pPlayerMan->getshot_treffer(0)/
									(float)pPlayerMan->getshot_anzahl(0)*100*25);
				}
				else
				{
					pPlayerMan->addscore(0,(float)pPlayerMan->getshot_treffer(0)+pPlayerMan->getshot_treffer(1)/
									(float)(pPlayerMan->getshot_anzahl(0)+pPlayerMan->getshot_anzahl(1))*100*25);

					pPlayerMan->addscore(1,(float)pPlayerMan->getshot_treffer(0)+pPlayerMan->getshot_treffer(1)/
									(float)(pPlayerMan->getshot_anzahl(0)+pPlayerMan->getshot_anzahl(1))*100*25);
				}
				
			}

			if(gamemode==GMODE_ENDLESS)
			{
				pPlayerMan->addscore(0,2500);
				if(pPlayerMan->getnum()==2)
				{
					pPlayerMan->addscore(1,2500);
				}
			}

			pPlayerMan->shot_reset(0);
			///welche musik wird abgespielt
			if(gamemode==GMODE_STORY)
			{
				switch(act_stage)
				{
					case 6:
						pSound->stopogg();
						pSound->playogg("/usr/local/games/gunocide2ex/sfx/track2.ogg\0");
						break;
					case 11:
						pSound->stopogg();
						pSound->playogg("/usr/local/games/gunocide2ex/sfx/track3.ogg\0");
						break;
					case 16:
						pSound->stopogg();
						pSound->playogg("/usr/local/games/gunocide2ex/sfx/track4.ogg\0");
						break;
					case 21:
						pSound->stopogg();
						pSound->playogg("/usr/local/games/gunocide2ex/sfx/track5.ogg\0");
						break;
				}
			}
		}
	}
}


///einladen der system-grafiken..z.b. gameover screen usw...
bool cSystem::load()
{
	if(!gfx_sprite[0].loadBMP("/usr/local/games/gunocide2ex/gfx/display/gameover.bmp"))
		return false;
	gfx_sprite[0].setColorKey(0,0,0);

	if(!gfx_sprite[1].loadBMP("/usr/local/games/gunocide2ex/gfx/display/nextwave.bmp"))
		return false;
	gfx_sprite[1].setColorKey(0,0,0);

	if(!gfx_sprite[2].loadBMP("/usr/local/games/gunocide2ex/gfx/display/timer.bmp"))
		return false;
	gfx_sprite[2].setColorKey(0,0,0);

	///einladen der tastenkonfiguration
	loadkeymaps();
	return true;
}

void cSystem::sysrender()
{
	cFont		*pFont=cFont::getInstance();
	cPlayerMan	*pPlayerMan=cPlayerMan::getInstance();
	float		temp=0.0f;

	if(gamemode==GMODE_SUICIDE)
	{
		gfx_sprite[2].render(screen,284,4);
		pFont->print(screen,302,8,"%i\0",time_suicide);
	}

	if(bgameover)gfx_sprite[0].render(screen,110,200);

	if(bnextwave)
	{
		if(gamemode==GMODE_SUICIDE)
			gfx_sprite[1].render(screen,130,200);

		if(gamemode==GMODE_STORY)
		{
			if(pPlayerMan->getnum()==1)
			{
				gfx_sprite[1].render(screen,130,150);
				pFont->setcolor(255,255,255);
				pFont->print(screen,220,250,"SCORE: %20c%d\0",' ',pPlayerMan->getscore(0));
				pFont->print(screen,220,270,"SHOT COUNT: %10c%d\0",' ',pPlayerMan->getshot_anzahl(0));
				pFont->print(screen,220,290,"SHOT HITS: %15c%d\0",' ',pPlayerMan->getshot_treffer(0));
				temp=((float)pPlayerMan->getshot_treffer(0)/(float)pPlayerMan->getshot_anzahl(0));
				pFont->print(screen,220,310,"HIT RATIO: %17c%d %\0",' ',(int)(temp*100));
				pFont->print(screen,220,330,"BONUS SCORE: %8c%d\0",' ',(int)((temp*100)*25));
				pFont->setcolor(0,0,0);
			}
			else
			{
				gfx_sprite[1].render(screen,130,150);
				pFont->setcolor(255,255,255);
				pFont->print(screen,220,250,"SCORE: %20c%d\0",' ',pPlayerMan->getscore(0)+pPlayerMan->getscore(1));

				pFont->print(screen,220,270,"SHOT COUNT: %10c%d\0",' ',pPlayerMan->getshot_anzahl(0)+pPlayerMan->getshot_anzahl(1));
				pFont->print(screen,220,290,"SHOT HITS: %15c%d\0",' ',pPlayerMan->getshot_treffer(0)+pPlayerMan->getshot_treffer(1));
				

				temp=(float)(pPlayerMan->getshot_treffer(0)+pPlayerMan->getshot_treffer(1))/
					(float)(pPlayerMan->getshot_anzahl(0)+pPlayerMan->getshot_anzahl(1));

				pFont->print(screen,220,310,"HIT RATIO: %17c%d %\0",' ',(int)(temp*100));
				pFont->print(screen,220,330,"BONUS SCORE: %8c%d\0",' ',(int)((temp*100)*25));
				pFont->setcolor(0,0,0);

			}
		}

		if(gamemode==GMODE_ENDLESS)
		{
			if(pPlayerMan->getnum()==1)
			{
				gfx_sprite[1].render(screen,130,150);
				pFont->setcolor(255,255,255);
				pFont->print(screen,220,250,"SCORE: %20c%d\0",' ',pPlayerMan->getscore(0));
				pFont->print(screen,220,270,"WAVE: %23c%d\0",' ',pPlayerMan->getwaves(0));
				pFont->print(screen,220,290,"BONUS SCORE: %8c%d\0",' ',2500);
				pFont->setcolor(0,0,0);
			}
			else
			{
				gfx_sprite[1].render(screen,130,150);
				pFont->setcolor(255,255,255);
				pFont->print(screen,220,250,"SCORE: %20c%d\0",' ',pPlayerMan->getscore(0)+pPlayerMan->getscore(1));
				pFont->print(screen,220,270,"WAVE: %23c%d\0",' ',pPlayerMan->getwaves(0)+pPlayerMan->getwaves(1));
				pFont->print(screen,220,290,"BONUS SCORE: %8c%d\0",' ',2500);
				pFont->setcolor(0,0,0);
			}
		}
	}

	if(bpause)pFont->print(screen,300,400,"PAUSE\0");
}

///hier wird �berpr�ft ob die Spieler tot sind
///und andere Dinge
void cSystem::sysupdate()
{
	cUtil		*pUtil=cUtil::getInstance();
	cMenu		*pMenu=cMenu::getInstance();
	cSound		*pSound=cSound::getInstance();
	cPlayerMan	*pPlayerMan=cPlayerMan::getInstance();
	cHScore		*pHScore=cHScore::getInstance();

	long	act_time=0;
	float	temp=0;

	if(gamemode==GMODE_SUICIDE && !bgameover && !bnextwave)
	{
		act_time=SDL_GetTicks();
		if(act_time-time_suicide_delay>1000)
		{
			time_suicide_delay=act_time;
			time_suicide--;
			if(time_suicide<=0)
			{
				bgameover=true;
				gameover_delay=SDL_GetTicks();
			}
		}
	}

	///wenn der spieler tot ist, ist alles vorbei ;)
	if(pPlayerMan->getnum()==1)
	{
		if(!bgameover && pPlayerMan->alive(0)==false)
		{	
			bgameover=true;
			gameover_delay=SDL_GetTicks();
		}
	}
	else
	{
		if(!bgameover && !pPlayerMan->alive(0) && !pPlayerMan->alive(1))
		{	
			bgameover=true;
			gameover_delay=SDL_GetTicks();
		}

	}

	///wie lange wird gameover noch angezeigt bis es ins mne� zur�ck geht
	if(bgameover)
	{
		act_time=SDL_GetTicks();
		if(act_time-gameover_delay>=3500)
		{
			bgameover=false;
			gamestate=GSTATE_MENU;
			pMenu->setmenustate(MSTATE_HSCORE);
			
			///�berpr�fen ob der/die spieler in die highscore kommt
			///vorher bauen wir aber noch die hitratio zusammen
			if(pPlayerMan->getnum()==1)
			{
				temp=((float)pPlayerMan->getshot_treffer(0)/(float)pPlayerMan->getshot_anzahl(0));
				pHScore->checkfornewentry(pPlayerMan->getscore(0),temp*100,pPlayerMan->getwaves(0),gamemode-1);
			}
			else
			{
				temp=((float)(pPlayerMan->getshot_treffer(0)+pPlayerMan->getshot_treffer(1))/
					(float)pPlayerMan->getshot_anzahl(0)+pPlayerMan->getshot_anzahl(1));
				pHScore->checkfornewentry(pPlayerMan->getscore(0)+pPlayerMan->getscore(1),
											temp*100,
											pPlayerMan->getwaves(0)+pPlayerMan->getwaves(1),
											gamemode-1);
			}

			pHScore->setmode(gamemode);

			cPlayerMan::delInstance();
			cWeaponMan::delInstance();
			cEnemyMan::delInstance();
			cStage::delInstance();
			cEffectMan::delInstance();
			cPowerupMan::delInstance();

			pSound->stopogg();
			pSound->playogg("/usr/local/games/gunocide2ex/sfx/menu.ogg\0");
		}
	}
}

///Spielmodus setzen und gameover ausschalten
void cSystem::setgamemode(int mode)
{
	gamemode=mode;
	bgameover=false;
	if(gamemode==GMODE_SUICIDE)
		time_suicide=90;
}

int cSystem::getgamemode()
{
	return gamemode;
}

void cSystem::setactstage(int wert)
{
	stage_selected=wert;
	act_stage=(stage_selected*5)+1;
}

bool cSystem::getgameover()
{
	return bgameover;
}

bool cSystem::getnextwave()
{
	return bnextwave;
}

///einlesen der Tastaturkonfiguration
///Standard-Werte werden gesetzt wenn's fehlschl�gt
void cSystem::loadkeymaps()
{
	cUtil	*pUtil=cUtil::getInstance();
	char	*buffer;
	
	printf("reading key maps\n");
	
	///linke taste f�r spieler1
	buffer=pUtil->getconfigvalue("/usr/local/games/gunocide2ex/config.cfg","P1_LEFT");
	if(!buffer)
	{
		printf("failed to read left key for player one: using left arrow\n");
		keymap[0].left=SDLK_LEFT;
	}
	else
	{
		keymap[0].left=atoi(buffer);
		printf("player one left key:%s\n",SDL_GetKeyName((SDLKey)keymap[0].left));
	}

	///rechte taste f�r spieler1
	buffer=pUtil->getconfigvalue("/usr/local/games/gunocide2ex/config.cfg","P1_RIGHT");
	if(!buffer)
	{
		printf("failed to read right key for player one: using right arrow\n");
		keymap[0].right=SDLK_RIGHT;
	}
	else
	{
		keymap[0].right=atoi(buffer);
		printf("player one right key:%s\n",SDL_GetKeyName((SDLKey)keymap[0].right));
	}

	///schuss taste f�r spieler1
	buffer=pUtil->getconfigvalue("/usr/local/games/gunocide2ex/config.cfg","P1_SHOT");
	if(!buffer)
	{
		printf("failed to read shot key for player one: using right control\n");
		keymap[0].shot=SDLK_RCTRL;
	}
	else
	{
		keymap[0].shot=atoi(buffer);
		printf("player one shot key:%s\n",SDL_GetKeyName((SDLKey)keymap[0].shot));
	}

	///special taste f�r spieler1
	buffer=pUtil->getconfigvalue("/usr/local/games/gunocide2ex/config.cfg","P1_SPECIAL");
	if(!buffer)
	{
		printf("failed to read special key for player one: using right shift\n");
		keymap[0].special=SDLK_RSHIFT;
	}
	else
	{
		keymap[0].special=atoi(buffer);
		printf("player one special key:%s\n",SDL_GetKeyName((SDLKey)keymap[0].special));
	}

	/////////////spieler 2
	///linke taste f�r spieler1
	buffer=pUtil->getconfigvalue("/usr/local/games/gunocide2ex/config.cfg","P2_LEFT");
	if(!buffer)
	{
		printf("failed to read left key for player two: using left arrow\n");
		keymap[1].left=SDLK_LEFT;
	}
	else
	{
		keymap[1].left=atoi(buffer);
		printf("player two left key:%s\n",SDL_GetKeyName((SDLKey)keymap[1].left));
	}

	///rechte taste f�r spieler1
	buffer=pUtil->getconfigvalue("/usr/local/games/gunocide2ex/config.cfg","P2_RIGHT");
	if(!buffer)
	{
		printf("failed to read right key for player two: using right arrow\n");
		keymap[1].right=SDLK_RIGHT;
	}
	else
	{
		keymap[1].right=atoi(buffer);
		printf("player two right key:%s\n",SDL_GetKeyName((SDLKey)keymap[1].right));
	}

	///schuss taste f�r spieler1
	buffer=pUtil->getconfigvalue("/usr/local/games/gunocide2ex/config.cfg","P2_SHOT");
	if(!buffer)
	{
		printf("failed to read shot key for player two: using right control\n");
		keymap[1].shot=SDLK_RCTRL;
	}
	else
	{
		keymap[1].shot=atoi(buffer);
		printf("player two shot key:%s\n",SDL_GetKeyName((SDLKey)keymap[1].shot));
	}

	///special taste f�r spieler1
	buffer=pUtil->getconfigvalue("/usr/local/games/gunocide2ex/config.cfg","P2_SPECIAL");
	if(!buffer)
	{
		printf("failed to read special key for player two: using right shift\n");
		keymap[1].special=SDLK_RSHIFT;
	}
	else
	{
		keymap[1].special=atoi(buffer);
		printf("player two special key:%s\n",SDL_GetKeyName((SDLKey)keymap[1].special));
	}
}
