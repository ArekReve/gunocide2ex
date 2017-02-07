/***************************************************************************
                          cMenu.cpp  -  description
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
#include "cMenu.h"
#include "cKeyboard.h"
#include "cSystem.h"
#include "cFont.h"
#include "cUtil.h"
#include "cPlayerMan.h"
#include "cPlayer.h"
#include "cPowerup.h"
#include "cSound.h"
#include "cEffects.h"
#include "cEnemyMan.h"
#include "cWeaponMan.h"
#include "cStage.h"
#include "cHScore.h"
#include "cCredits.h"
#include "mmgr.h"

cMenu::cMenu()
{
	menustate=MSTATE_POLYFRAG;
	memset(text_main,0,sizeof(text_main));
	memset(text_gmode,0,sizeof(text_gmode));
	num_player=1;
	stageselected=0;
	playerselected=NULL;
	scoreselected=1;
	pselected=0;
}

cMenu::~cMenu()
{
	cHScore::delInstance();
	cCredits::delInstance();
}

cMenu* cMenu::instance=NULL;

cMenu* cMenu::getInstance()
{
	if(instance==NULL)

	{
		instance=new cMenu();
		return instance;
	}else return instance;
}

void cMenu::delInstance()
{
	if(instance)
	{
		delete instance;
		instance=NULL;
	}
}

///aktualisieren des Menüs
void cMenu::update()
{
	switch(menustate)
	{
		case MSTATE_POLYFRAG:
			update_polyfrag();
			break;
		case MSTATE_TITEL:
			update_startbild();
			break;
		case MSTATE_MAIN:
			update_main();
			break;
		case MSTATE_CREDITS:
			update_credits();
			break;
		case MSTATE_GMODE:
			update_modi();
			break;
		case MSTATE_LEVELSELECT:
			update_stageselect();
			break;
		case MSTATE_PLAYERSELECT:
			update_playerselect();
			break;
		case MSTATE_HSCORE:
			update_highscore();
			break;
	}
}

///Das Menü zeichnen
void cMenu::render(SDL_Surface *screen)
{
	switch(menustate)
	{
		case MSTATE_POLYFRAG:
			render_polyfrag(screen);
			break;
		case MSTATE_TITEL:
			render_startbild(screen);
			break;
		case MSTATE_MAIN:
			render_main(screen);
			break;
		case MSTATE_CREDITS:
			render_credits(screen);
			break;
		case MSTATE_GMODE:
			render_modi(screen);
			break;
		case MSTATE_LEVELSELECT:
			render_stageselect(screen);
			break;
		case MSTATE_PLAYERSELECT:
			render_playerselect(screen);
			break;
		case MSTATE_HSCORE:
			render_highscore(screen);
			break;
	}
}

///einladen aller Grafiken usw. für das Menü
bool cMenu::load()
{
	cSound		*pSound=cSound::getInstance();
	cHScore		*pHScore=cHScore::getInstance();
	cCredits	*pCredits=cCredits::getInstance();

	if(!gfx_mouse.loadBMP("/usr/local/games/gunocide2ex/gfx/menu/cursor.bmp"))
		return false;
	gfx_mouse.setColorKey(0,0,0);

	if(!gfx_sprite[0].loadBMP("/usr/local/games/gunocide2ex/gfx/menu/sbild.bmp"))
		return false;

	if(!gfx_sprite[1].loadBMP("/usr/local/games/gunocide2ex/gfx/menu/start.bmp"))
		return false;

	if(!gfx_sprite[2].loadBMP("/usr/local/games/gunocide2ex/gfx/menu/credits.bmp"))
		return false;

	if(!gfx_sprite[3].loadBMP("/usr/local/games/gunocide2ex/gfx/menu/modi.bmp"))
		return false;
	
	if(!gfx_sprite[4].loadBMP("/usr/local/games/gunocide2ex/gfx/menu/1p.bmp"))
		return false;

	if(!gfx_sprite[5].loadBMP("/usr/local/games/gunocide2ex/gfx/menu/2p.bmp"))
		return false;

	if(!gfx_sprite[6].loadBMP("/usr/local/games/gunocide2ex/gfx/menu/zselect.bmp"))
		return false;

	if(!gfx_sprite[7].loadBMP("/usr/local/games/gunocide2ex/gfx/menu/pselect1.bmp"))
		return false;

	if(!gfx_sprite[8].loadBMP("/usr/local/games/gunocide2ex/gfx/menu/pselect2.bmp"))
		return false;

	if(!gfx_sprite[9].loadBMP("/usr/local/games/gunocide2ex/gfx/menu/hs.bmp"))
		return false;

	if(!gfx_sprite[10].loadBMP("/usr/local/games/gunocide2ex/gfx/menu/polyfrag.bmp"))
		return false;

	if(!gfx_stage[0].loadBMP("/usr/local/games/gunocide2ex/gfx/menu/s1.bmp"))
		return false;

	if(!gfx_stage[1].loadBMP("/usr/local/games/gunocide2ex/gfx/menu/s2.bmp"))
		return false;

	if(!gfx_stage[2].loadBMP("/usr/local/games/gunocide2ex/gfx/menu/s3.bmp"))
		return false;

	if(!gfx_stage[3].loadBMP("/usr/local/games/gunocide2ex/gfx/menu/s4.bmp"))
		return false;	

	if(!gfx_stage[4].loadBMP("/usr/local/games/gunocide2ex/gfx/menu/s5.bmp"))
		return false;

	if(!gfx_player[PLAYER_RAGE].loadBMP("/usr/local/games/gunocide2ex/gfx/menu/rage.bmp"))
		return false;

	if(!gfx_player[PLAYER_FURY].loadBMP("/usr/local/games/gunocide2ex/gfx/menu/fury.bmp"))
		return false;

	if(!gfx_player[PLAYER_BANSHEE].loadBMP("/usr/local/games/gunocide2ex/gfx/menu/banshee.bmp"))
		return false;

	//einladen der Highscores
	if(!pHScore->load())
		return false;

	//credits-Datei einladen
	if(!pCredits->load())
		return false;

	pSound->stopogg();
	pSound->playogg("/usr/local/games/gunocide2ex/sfx/menu.ogg");
	return true;
}

void cMenu::update_startbild()
{
	cKeyboard	*pKeyboard=cKeyboard::getInstance();
	KeyEvent	*kevent=NULL;
	cSystem		*pSystem=cSystem::getInstance();

	///spiel beenden und ab ins menü
	kevent=pKeyboard->getkey(SDLK_ESCAPE);
	if(kevent && kevent->pressed==1)
	{
		pSystem->setgamestate(GSTATE_EXIT);
		delete kevent;	
		kevent=NULL;
	} else kevent=NULL;

	//ins hauptmenü
	kevent=pKeyboard->getkey(SDLK_SPACE);
	if(kevent && kevent->pressed==1)
	{
		menustate=MSTATE_MAIN;
		delete kevent;	
		kevent=NULL;
	} else kevent=NULL;
}

void cMenu::update_polyfrag()
{
	cKeyboard	*pKeyboard=cKeyboard::getInstance();
	KeyEvent	*kevent=NULL;
	cSystem		*pSystem=cSystem::getInstance();

	///spiel beenden und ab ins menü
	kevent=pKeyboard->getkey(SDLK_ESCAPE);
	if(kevent && kevent->pressed==1)
	{
		pSystem->setgamestate(GSTATE_EXIT);
		delete kevent;	
		kevent=NULL;
	} else kevent=NULL;

	//ins hauptmenü
	kevent=pKeyboard->getkey(SDLK_SPACE);
	if(kevent && kevent->pressed==1)
	{
		menustate=MSTATE_TITEL;
		delete kevent;	
		kevent=NULL;
	} else kevent=NULL;
}

void cMenu::render_startbild(SDL_Surface *screen)
{
	cFont	*pFont=cFont::getInstance();

	gfx_sprite[0].render(screen,0,0);
	pFont->setcolor(255,0,0);
	pFont->print(screen,250,460,"PRESS SPACE\0");
	pFont->setcolor(255,255,255);
}

void cMenu::render_polyfrag(SDL_Surface *screen)
{
	cFont	*pFont=cFont::getInstance();

	gfx_sprite[10].render(screen,0,0);
	pFont->setcolor(255,0,0);
	pFont->print(screen,250,460,"PRESS SPACE\0");
	pFont->setcolor(255,255,255);
}

void cMenu::render_main(SDL_Surface *screen)
{

	cKeyboard	*pKeyboard=cKeyboard::getInstance();
	cFont		*pFont=cFont::getInstance();

	//Hintergrund
	gfx_sprite[1].render(screen,0,0);

	///Version
	pFont->setcolor(255,255,255);
	pFont->print(screen,570,460,"v%s\0",G2EX_VERSION);

	//anzeigen der selektierten nachricht
	if(text_main[0])pFont->print(screen,pKeyboard->getmousex()+40,pKeyboard->getmousey()+16
					,"EXIT GAME");
	if(text_main[1])pFont->print(screen,pKeyboard->getmousex()+40,pKeyboard->getmousey()+16
					,"NEW GAME");
	if(text_main[2])pFont->print(screen,pKeyboard->getmousex()+40,pKeyboard->getmousey()+16
					,"NOT AVAILABLE");
	if(text_main[3])pFont->print(screen,pKeyboard->getmousex()+40,pKeyboard->getmousey()+16
					,"HIGHSCORE LIST");
	//Maus
	gfx_mouse.render(screen,pKeyboard->getmousex(),pKeyboard->getmousey());
}

void cMenu::update_main()
{
	cKeyboard	*pKeyboard=cKeyboard::getInstance();
	KeyEvent	*kevent=NULL;
	cSystem		*pSystem=cSystem::getInstance();
	cUtil		*pUtil=cUtil::getInstance();
	cSound		*pSound=cSound::getInstance();

	SDL_Rect	selection[4];
    SDL_Rect	mouse;

	mouse.x=pKeyboard->getmousex();
	mouse.y=pKeyboard->getmousey();
	mouse.w=pKeyboard->getmousex()+16;
	mouse.h=pKeyboard->getmousey()+16;

	///beenden
	selection[0].y=330;
	selection[0].x=250;
	selection[0].h=410;
	selection[0].w=400;

	///neues spiel
	selection[1].y=110;
	selection[1].x=250;
	selection[1].h=210;
	selection[1].w=400;

	///online hscore
	selection[2].y=230;
	selection[2].x=50;

	selection[2].h=310;
	selection[2].w=200;

	///highscore liste
	selection[3].y=230;
	selection[3].x=420;
	selection[3].h=310;
	selection[3].w=610;

	///spiel beenden und ab ins menü
	kevent=pKeyboard->getkey(SDLK_ESCAPE);
	if(kevent && kevent->pressed==1)
	{
		pSystem->setgamestate(GSTATE_EXIT);
		delete kevent;	
		kevent=NULL;
	} else kevent=NULL;

	///mal schaun was gerade die maus selektiert
	for(int i=0;i<4;i++)
	{
		if(pUtil->collision(selection[i],mouse))
		{
			text_main[i]=true;
			///wir schaun auch noch gerade nach wenn der mausbutton gedrückt wurde
			if(pKeyboard->getmousepressed()>0)
			{
				pSound->playwav(2);
				switch(i)
				{
					case 0:
						menustate=MSTATE_CREDITS;
						break;
					case 1:
						menustate=MSTATE_GMODE;
						break;
					case 3:
						menustate=MSTATE_HSCORE;
						break;
				}
			}
		}
		else
			text_main[i]=false;
	}
}

///hier werden die Credits auf dem Bildschirm angezeigt
void cMenu::render_credits(SDL_Surface *screen)
{
	cFont		*pFont=cFont::getInstance();
	cCredits	*pCredits=cCredits::getInstance();

	gfx_sprite[2].render(screen,0,0);

	pCredits->render(screen);
}

void cMenu::update_credits()
{
	cKeyboard	*pKeyboard=cKeyboard::getInstance();
	KeyEvent	*kevent=NULL;
	cSystem		*pSystem=cSystem::getInstance();
	cCredits	*pCredits=cCredits::getInstance();

	
	pCredits->update();

	///spiel beenden
	kevent=pKeyboard->getkey(SDLK_ESCAPE);
	if(kevent && kevent->pressed==1)
	{
		pSystem->setgamestate(GSTATE_EXIT);
		delete kevent;	
		kevent=NULL;
	} else kevent=NULL;
}

void cMenu::render_modi(SDL_Surface *screen)
{
	cKeyboard	*pKeyboard=cKeyboard::getInstance();
	cFont		*pFont=cFont::getInstance();

	gfx_sprite[3].render(screen,0,0);

	//1 oder 2 Spieler?
	if(num_player==1)
		gfx_sprite[4].render(screen,57,27);
	else
		gfx_sprite[5].render(screen,57,27);	

	//anzeigen der selektierten nachricht
	if(text_gmode[0])pFont->print(screen,pKeyboard->getmousex()+40,pKeyboard->getmousey()+16
					,"BACK TO MAIN MENU");
	if(text_gmode[1])pFont->print(screen,pKeyboard->getmousex()+40,pKeyboard->getmousey()+16
					,"ONE OR TWO PLAYERS");
	if(text_gmode[2])pFont->print(screen,pKeyboard->getmousex()+40,pKeyboard->getmousey()+16
					,"SUICIDE MODE");
	if(text_gmode[3])pFont->print(screen,pKeyboard->getmousex()+40,pKeyboard->getmousey()+16
					,"STORY MODE");
	if(text_gmode[4])pFont->print(screen,pKeyboard->getmousex()+40,pKeyboard->getmousey()+16
					,"ENDLESS MODE");

	//Maus
	gfx_mouse.render(screen,pKeyboard->getmousex(),pKeyboard->getmousey());
}

void cMenu::update_modi()
{
	cKeyboard	*pKeyboard=cKeyboard::getInstance();
	KeyEvent	*kevent=NULL;
	cSystem		*pSystem=cSystem::getInstance();
	cUtil		*pUtil=cUtil::getInstance();
	cSound		*pSound=cSound::getInstance();
	cPlayerMan	*pPlayerMan=cPlayerMan::getInstance();

	SDL_Rect	selection[5];
    SDL_Rect	mouse;

	mouse.x=pKeyboard->getmousex();
	mouse.y=pKeyboard->getmousey();

	mouse.w=pKeyboard->getmousex()+16;
	mouse.h=pKeyboard->getmousey()+16;

	//zurück
	selection[0].y=330;
	selection[0].x=250;
	selection[0].h=400;
	selection[0].w=390;

	//spieleranzahl
	selection[1].y=20;
	selection[1].x=60;
	selection[1].h=90;
	selection[1].w=180;
	
	//suicide modus
	selection[2].y=220;
	selection[2].x=20;
	selection[2].h=310;
	selection[2].w=190;

	//story modus
	selection[3].y=120;
	selection[3].x=250;
	selection[3].h=220;
	selection[3].w=390;

	///endless
	selection[4].y=220;
	selection[4].x=420;
	selection[4].h=310;
	selection[4].w=600;

	///ins hauptmenü zurück
	kevent=pKeyboard->getkey(SDLK_ESCAPE);
	if(kevent && kevent->pressed==1)
	{
		menustate=MSTATE_MAIN;
		delete kevent;	
		kevent=NULL;
	} else kevent=NULL;

	///mal schaun was gerade die maus selektiert
	for(int i=0;i<5;i++)
	{
		if(pUtil->collision(selection[i],mouse))

		{
			text_gmode[i]=true;
			///wir schaun auch noch gerade nach wenn der mausbutton gedrückt wurde
			if(pKeyboard->getmousepressed()>0)
			{
				pSound->playwav(2);
				switch(i)
				{
					case 0:
						menustate=MSTATE_MAIN;
						break;
					case 1://Anzahl der Spieler festlegen
						if(num_player==1)
							num_player=2;
						else
							num_player=1;
						break;
					case 2://suicide modus
						pSystem->setgamemode(GMODE_SUICIDE);
						menustate=MSTATE_LEVELSELECT;
						
						pPlayerMan->create(num_player);

						//falls es schon array mit den ausgewählten Spielern gibt löschen wir es hier
						if(playerselected)
							delete [] playerselected;
						playerselected=new int[num_player];

						memset(playerselected,0,sizeof(int)*num_player);
						break;
					case 3://story
						pSystem->setgamemode(GMODE_STORY);
						pSystem->setactstage(0);
						menustate=MSTATE_PLAYERSELECT;
						pPlayerMan->create(num_player);
						
						//falls es schon array mit den ausgewählten Spielern gibt löschen wir es hier
						if(playerselected)
							delete [] playerselected;

						playerselected=new int[num_player];
						memset(playerselected,0,sizeof(int)*num_player);
						break;
					case 4://endless modus
						pSystem->setgamemode(GMODE_ENDLESS);
						menustate=MSTATE_LEVELSELECT;
						pPlayerMan->create(num_player);

						//falls es schon array mit den ausgewählten Spielern gibt löschen wir es hier
						if(playerselected)
							delete [] playerselected;
						playerselected=new int[num_player];
						memset(playerselected,0,sizeof(int)*num_player);
						break;
				}
			}
		}
		else
			text_gmode[i]=false;
	}
}

void cMenu::render_stageselect(SDL_Surface *screen)
{
	cKeyboard	*pKeyboard=cKeyboard::getInstance();
	cFont		*pFont=cFont::getInstance();

	gfx_sprite[6].render(screen,0,0);
	
	//selektierte Stage zeichnen
	gfx_stage[stageselected].render(screen,150,0);

	//Maus
	gfx_mouse.render(screen,pKeyboard->getmousex(),pKeyboard->getmousey());
}

void cMenu::update_stageselect()
{
	cKeyboard	*pKeyboard=cKeyboard::getInstance();
	KeyEvent	*kevent=NULL;
	cSystem		*pSystem=cSystem::getInstance();
	cUtil		*pUtil=cUtil::getInstance();
	cSound		*pSound=cSound::getInstance();

	SDL_Rect	selection[3];
    SDL_Rect	mouse;

	mouse.x=pKeyboard->getmousex();
	mouse.y=pKeyboard->getmousey();
	mouse.w=pKeyboard->getmousex()+16;
	mouse.h=pKeyboard->getmousey()+16;

	//links
	selection[0].y=230;
	selection[0].x=30;
	selection[0].h=305;
	selection[0].w=200;

	//rechts
	selection[1].y=230;
	selection[1].x=420;
	selection[1].h=305;
	selection[1].w=600;



	//ok
	selection[2].y=330;
	selection[2].x=250;
	selection[2].h=400;
	selection[2].w=380;

	///ins spielmodus-menü zurück
	kevent=pKeyboard->getkey(SDLK_ESCAPE);
	if(kevent && kevent->pressed==1)
	{
		menustate=MSTATE_GMODE;
		delete kevent;	
		kevent=NULL;
	} else kevent=NULL;

	///mal schaun was gerade die maus selektiert
	for(int i=0;i<3;i++)
	{
		if(pUtil->collision(selection[i],mouse))
		{
			///wir schaun auch noch gerade nach wenn der mausbutton gedrückt wurde
			if(pKeyboard->getmousepressed()>0)
			{
				pSound->playwav(2);
				switch(i)
				{
					case 0:
						if(stageselected>0)stageselected--;
						break;
					case 1://Anzahl der Spieler festlegen
						if(stageselected<4)stageselected++;
						break;
					case 2://suicide modus
						menustate=MSTATE_PLAYERSELECT;
						pSystem->setactstage(stageselected);
						break;
				}
			}
		}
	}
}

void cMenu::render_playerselect(SDL_Surface *screen)
{
	cKeyboard	*pKeyboard=cKeyboard::getInstance();
	cFont		*pFont=cFont::getInstance();

	if(pselected==0)
		gfx_sprite[7].render(screen,0,0);
	else
		gfx_sprite[8].render(screen,0,0);
	
	//selektierten Spieler zeichnen
	gfx_player[playerselected[pselected]].render(screen,150,0);

	//Maus
	gfx_mouse.render(screen,pKeyboard->getmousex(),pKeyboard->getmousey());
}

void cMenu::update_playerselect()
{
	cKeyboard	*pKeyboard=cKeyboard::getInstance();
	KeyEvent	*kevent=NULL;
	cSystem		*pSystem=cSystem::getInstance();
	cUtil		*pUtil=cUtil::getInstance();
	cSound		*pSound=cSound::getInstance();

	SDL_Rect	selection[3];
    SDL_Rect	mouse;

	mouse.x=pKeyboard->getmousex();
	mouse.y=pKeyboard->getmousey();
	mouse.w=pKeyboard->getmousex()+16;
	mouse.h=pKeyboard->getmousey()+16;

	//links
	selection[0].y=230;
	selection[0].x=30;
	selection[0].h=305;
	selection[0].w=200;

	//rechts
	selection[1].y=230;
	selection[1].x=420;
	selection[1].h=305;
	selection[1].w=600;

	//ok
	selection[2].y=330;
	selection[2].x=250;
	selection[2].h=400;
	selection[2].w=380;

	///ins stageselect-menü zurück
	kevent=pKeyboard->getkey(SDLK_ESCAPE);
	if(kevent && kevent->pressed==1)
	{
		if(pSystem->getgamemode()!=GMODE_STORY)
			menustate=MSTATE_LEVELSELECT;
		else
			menustate=MSTATE_GMODE;

		cPlayerMan::delInstance();
		delete kevent;	
		kevent=NULL;
	} else kevent=NULL;

	///mal schaun was gerade die maus selektiert
	for(int i=0;i<3;i++)
	{
		if(pUtil->collision(selection[i],mouse))
		{
			///wir schaun auch noch gerade nach wenn der mausbutton gedrückt wurde
			if(pKeyboard->getmousepressed()>0)
			{
				pSound->playwav(2);
				switch(i)
				{
					case 0:
						if(playerselected[pselected]>0)playerselected[pselected]--;
						break;
					case 1://Anzahl der Spieler festlegen
						if(playerselected[pselected]<2)playerselected[pselected]++;
						break;
					case 2://spiel starten
						if(num_player==1)
						{
							pSystem->setgamestate(GSTATE_INGAME);
							loadgamedata();
						}
						else
						{
							pselected++;
							if(playerselected[0]!=playerselected[1] && pselected==2)
							{
								pSystem->setgamestate(GSTATE_INGAME);
								loadgamedata();
								pselected=0;
							}
						}
						break;
				}
			}
		}
	}
}

////menuü-status setzen
void cMenu::setmenustate(int state)
{
	menustate=state;
}

///alle klassen sowie daten für das spiel werden erstellt
bool cMenu::loadgamedata()
{
	cPlayerMan	*pPlayerMan=cPlayerMan::getInstance();
	cPowerupMan	*pPowerupMan=cPowerupMan::getInstance();
	cEffectMan	*pEffectMan=NULL;
	cWeaponMan	*pWeaponMan=NULL;
	cEnemyMan	*pEnemyMan=NULL;
	cStage		*pStage=NULL;
	cSound		*pSound=cSound::getInstance();
	cUtil		*pUtil=cUtil::getInstance();
	char		buffer[256];

	//Spieler laden
	if(!pPlayerMan->load(0,playerselected[0]))
		return false;
	if(num_player==2)
	{
		if(!pPlayerMan->load(1,playerselected[1]))
			return false;
	}

	delete [] playerselected;
	playerselected=NULL;

	//PowerUps laden
	if(!pPowerupMan->load())
		return false;

	//gegnergrafiken laden
	pEnemyMan=cEnemyMan::getInstance();
	if(!pEnemyMan->load())
		return false;

	//Stage laden

	pStage=cStage::getInstance();
	pEnemyMan->destroy();
	pEnemyMan->create();
	
	sprintf(buffer,"/usr/local/games/gunocide2ex/lvl/stage%i.lvl\0",(stageselected*5)+1);
	if(!pStage->load(buffer,pEnemyMan))
		return false;

	//Waffen laden
	pWeaponMan=cWeaponMan::getInstance();
	if(!pWeaponMan->load())
		return false;


	//Effekte einladen
	pEffectMan=cEffectMan::getInstance();
	if(!pEffectMan->load())
		return false;

	///musik abspielen
	sprintf(buffer,"/usr/local/games/gunocide2ex/sfx/track%i.ogg\0",stageselected+1);
	pSound->stopogg();
	pSound->playogg(buffer);

	pUtil->updatetime();
	return true;
}

void cMenu::render_highscore(SDL_Surface *screen)
{
	cKeyboard	*pKeyboard=cKeyboard::getInstance();
	cFont		*pFont=cFont::getInstance();
	cHScore		*pHScore=cHScore::getInstance();

	gfx_sprite[9].render(screen,0,0);
	pHScore->render(screen);
	
	//Maus
	gfx_mouse.render(screen,pKeyboard->getmousex(),pKeyboard->getmousey());
}

void cMenu::update_highscore()
{
	cHScore		*pHScore=cHScore::getInstance();
	cKeyboard	*pKeyboard=cKeyboard::getInstance();
	KeyEvent	*kevent=NULL;

	if(pHScore->entername())
		pHScore->update();

	///ins stageselect-menü zurück
	kevent=pKeyboard->getkey(SDLK_ESCAPE);
	if(kevent && kevent->pressed==1)
	{
		pHScore->save();
		menustate=MSTATE_MAIN;
		delete kevent;	
		kevent=NULL;
	} else kevent=NULL;

	//nächste highscores anzeigen
	if(pKeyboard->getmousepressed()>0)
	{
		scoreselected++;
		if(scoreselected>3)
			scoreselected=1;

		pHScore->setmode(scoreselected);
	}
}
