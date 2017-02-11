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

#include "cPlayer.h"
#include "cDisplay.h"
#include "cWeaponMan.h"
#include "cUtil.h"
#include "cSystem.h"
#include "cSound.h"
#include "mmgr.h"

cPlayer::cPlayer()
{
	posx=0;
	posy=420;
	bmoveleft=false;
	bmoveright=false;
	mvdirection=MOVE_NORMAL;
	mvstate=0;
	pDisplay=NULL;
	bshot=false;
	delay_shot=0;
	typ_player=PLAYER_BANSHEE;
	typ_weapon=7;
	delay_mg=80;
	delay_mg_act=0;
	delay_special=0;
	bspecial=false;
	score=0;
	lives=3;
	specials=3;
	shieldstate=0;

 	shielddelay=0;
	shot_treffer=0;
	shot_gefeuert=0;
	waves=0;
}

cPlayer::~cPlayer()
{
	if(pDisplay)
	{
		delete pDisplay;
		pDisplay=NULL;
	}
}

///einladen aller Spielergrafiken
bool cPlayer::load(int type)
{
	char 	buffer[256];
	int		anim=1;
	int		i=0;
	char	c;
	
	typ_player=type;
	switch(typ_player)
	{
	case PLAYER_RAGE:
		c='r';
		typ_weapon=10;
		gfx_shield.loadBMP("/usr/local/games/gunocide2ex/gfx/player/shield/rage.bmp");
		gfx_shield.setColorKey(0,0,0);
		break;
	case PLAYER_BANSHEE:
		c='b';
		typ_weapon=7;
		gfx_shield.loadBMP("/usr/local/games/gunocide2ex/gfx/player/shield/banshee.bmp");
		gfx_shield.setColorKey(0,0,0);
		break;
	case PLAYER_FURY:
		c='f';
		typ_weapon=13;
		gfx_shield.loadBMP("/usr/local/games/gunocide2ex/gfx/player/shield/fury.bmp");
		gfx_shield.setColorKey(0,0,0);
		break;
	}

	//spieler normal laden
	sprintf(buffer,"/usr/local/games/gunocide2ex/gfx/player/%c/normal.txt",c);
	if(!gfx_sprite[MOVE_NORMAL].load(buffer))
		return false;
	gfx_sprite[MOVE_NORMAL].setColorKey(0,0,0);
	anim_sprite[MOVE_NORMAL].rewind();
	anim_sprite[MOVE_NORMAL].setLooped(true);
	anim_sprite[MOVE_NORMAL].setDelay(50);
	anim_sprite[MOVE_NORMAL].setMaxNum(gfx_sprite[MOVE_NORMAL].getMaxNum());

	//spieler links laden
	anim=1;
	for(i=MOVE_LEFT;i<MOVE_LEFT+3;i++)
	{
		sprintf(buffer,"/usr/local/games/gunocide2ex/gfx/player/%c/links%i.txt",c,anim);
		anim++;

		if(!gfx_sprite[i].load(buffer))
			return false;
		gfx_sprite[i].setColorKey(0,0,0);
		anim_sprite[i].rewind();

		if(i==3)anim_sprite[i].setLooped(true);
		else anim_sprite[i].setLooped(false);

		anim_sprite[i].setDelay(50);
		anim_sprite[i].setMaxNum(gfx_sprite[i].getMaxNum());
	}

	//spieler rechts laden
	anim=1;
	for(i=MOVE_RIGHT;i<MOVE_RIGHT+3;i++)
	{
		sprintf(buffer,"/usr/local/games/gunocide2ex/gfx/player/%c/rechts%i.txt",c,anim);
		anim++;

		if(!gfx_sprite[i].load(buffer))
			return false;
		gfx_sprite[i].setColorKey(0,0,0);
		anim_sprite[i].rewind();

		if(i==6)anim_sprite[i].setLooped(true);
		else anim_sprite[i].setLooped(false);

		anim_sprite[i].setDelay(50);
		anim_sprite[i].setMaxNum(gfx_sprite[i].getMaxNum());
	}

	//Anzeige laden
	pDisplay=new cDisplay();
	if(!pDisplay->load(type,nr))
		return false;

   	printf("player graphics succesfully loaded\n");
	return true;
}

///rendern der Spielergrafiken
void cPlayer::render(SDL_Surface *screen)
{
	cSystem	*pSystem=cSystem::getInstance();

	//Spieler zeichnen
	if(shieldstate!=1 && !pSystem->getgameover())
		gfx_sprite[mvstate].render(screen,&anim_sprite[mvstate],posx,posy);

	///schild zeichnen
	if(shieldstate==2  && !pSystem->getgameover())
		gfx_shield.render(screen,posx,posy);


	//Anzeige zeichnen
	pDisplay->render(screen,nr);
}

///X-Koordinate des Spielers setzen
void cPlayer::setposx(int x)
{


	posx=x;
}

void cPlayer::setposy(int y)
{
	posy=y;
}

int cPlayer::getposy()
{
	return posy;
}

///X-Koordinate des Spielers zur�ckgeben
int cPlayer::getposx()
{
	return posx;
}

///links bewegen
void cPlayer::setbmoveleft(bool wert)
{
	bmoveleft=wert;
	mvstate=0;
	mvdirection=MOVE_LEFT;
}

///rechts bewegen
void cPlayer::setbmoveright(bool wert)
{
	bmoveright=wert;
	mvstate=0;
	mvdirection=MOVE_RIGHT;
}

///alle Daten des Spielers updaten
///z.B. bewegen usw...
void cPlayer::update()
{
	long int	act_time=0;
	cUtil		*pUtil=cUtil::getInstance();
	cWeaponMan	*pWeaponMan=cWeaponMan::getInstance();
	cSound		*pSound=cSound::getInstance();
	cSystem		*pSystem=cSystem::getInstance();

	//Animation updaten
	anim_sprite[mvstate].update();

	//Spielerposition updaten
	//wenn keine Bewegungstaste mehr gedr�ckt wurde in die Ausgangsposition
	//wechseln
	if(bmoveleft || bmoveright)
	{
		if(bmoveright)
			posx+=(int)(pUtil->sync(300.0f));
		else
			posx+=(int)(pUtil->sync(-300.0f));
	}
	else
	{
		mvdirection=MOVE_NORMAL;
		mvstate=0;
	}

	//�berpr�fen ob der Spieler ausserhalb des Bildschirms ist
	//wenn ja, ab wieder mit ihm rein ;)
	if(posx<0)posx=0;
	if(posx>590)posx=590;

	//>>>>muss f�r die sch�sse abge�ndert werden!!!<<<<
	//Animationsstufe weiterschalten wenn er sich zur Seite bewegt
	if(mvdirection!=MOVE_NORMAL)
	{
		if(mvdirection==MOVE_LEFT && mvstate<3)
		{
			if(anim_sprite[mvstate].getAct()
				==anim_sprite[mvstate].getMaxNum()-1)
			{
				anim_sprite[mvstate].rewind();
				mvstate++;
			}
		}
		if(mvdirection==MOVE_RIGHT && mvstate<6)
		{
			if(anim_sprite[mvstate].getAct()
				==anim_sprite[mvstate].getMaxNum()-1)
			{
				anim_sprite[mvstate].rewind();
				mvstate++;
			}
		}
	}
	else mvstate=0;

	///hat der Spieler geschossen??
	if(bshot && delay_mg>0)
	{
		act_time=SDL_GetTicks();
		if(act_time-delay_shot>SHOT_DELAY)
		{
			delay_mg-=12;
			delay_shot=act_time;
			addshots();
		}
	}
	
	//hat der Spieler einen Special aktiviert?
	if(bspecial)
	{
		act_time=SDL_GetTicks();
		if(act_time-delay_special>SHOT_DELAY)
		{
			delay_special=act_time;
			addspecial();
		}
	}

	///updaten des displays
	if(delay_mg<80)       //anzeige f�r die sch�sse..wieviel er noch ballern kann
	{
		act_time=SDL_GetTicks();
		if(act_time-delay_mg_act>100)
		{
			delay_mg_act=act_time;
			delay_mg+=5;
		}
	}

	if(delay_mg>0)
		pDisplay->setdelay(delay_mg/20);
	else
		pDisplay->setdelay(0);

	pDisplay->setpowerup(specials);
	pDisplay->setleben(lives);
	pDisplay->setscore(score);
	pDisplay->update();

	///schild aktualisieren
	if(shieldstate!=0 && !pSystem->getgameover())
	{
		if(shieldstate==1)
		{//spieler kommt wieder ins geschehen hier danach
			act_time=SDL_GetTicks();
			if(act_time-shielddelay>750)
			{
				shielddelay=act_time;
				shieldstate=2;
				pSound->playwav(8);
			}
		}
		else
		{
			act_time=SDL_GetTicks();
			if(act_time-shielddelay>2500)
			{
				shielddelay=act_time;
				shieldstate=0;
			}
		}
	}
}



///gibt eine SDL_Rect Struktur zur�ck, die f�r eine Kollisionsabfrage
///ben�tigt wird
SDL_Rect cPlayer::getcolrect(int offx,int offy)
{
	SDL_Rect rc;

	rc.x=posx+offx;
	rc.y=posy+offy;
	rc.w=posx+gfx_sprite[mvstate].getwidth(&anim_sprite[mvstate])-offx;
	rc.h=posy+gfx_sprite[mvstate].getheight(&anim_sprite[mvstate])-offy;

	return rc;
}

///wurde die Taste zum Feuern gedr�ckt ??
void cPlayer::setbshot(bool wert)
{
	bshot=wert;
}

///hinzuf�gen der sch�sse f�r den spieler
void cPlayer::addshots()
{
	cWeaponMan	*pWeaponMan=cWeaponMan::getInstance();
	cSound		*pSound=cSound::getInstance();
	cSystem		*pSystem=cSystem::getInstance();

	if(shieldstate==1 || pSystem->getgameover() || pSystem->getnextwave())
		return;

	switch(typ_player)
	{
		//hinzuf�gen der Sch�sse f�r den BANSHEE
		case PLAYER_BANSHEE:
			if(typ_weapon==7)
			{
				pWeaponMan->add(7,posx+9,posy+6,0,WeaponVars::Speed[7],nr,WeaponVars::Energy[7]);
				pWeaponMan->add(7,posx+47,posy+6,0,WeaponVars::Speed[7],nr,WeaponVars::Energy[7]);
				shot_gefeuert+=2;
				pSound->playwav(7);
			}
			if(typ_weapon==8)
			{
				pWeaponMan->add(8,posx+5,posy+2,0,WeaponVars::Speed[8],nr,WeaponVars::Energy[8]);
				pWeaponMan->add(8,posx+43,posy+2,0,WeaponVars::Speed[8],nr,WeaponVars::Energy[8]);
				shot_gefeuert+=2;
				pSound->playwav(7);
			}
			if(typ_weapon==9)
			{
				pWeaponMan->add(9,posx+19,posy+2,0,WeaponVars::Speed[9],nr,WeaponVars::Energy[9]);
				pWeaponMan->add(10,posx+5,posy+2,WeaponVars::Speed[9],
								WeaponVars::Speed[9]-150,nr,WeaponVars::Energy[9]);
				pWeaponMan->add(11,posx+43,posy+2,-WeaponVars::Speed[9],
								WeaponVars::Speed[9]-150,nr,WeaponVars::Energy[9]);
				shot_gefeuert+=3;
				pSound->playwav(7);
			}
			break;
		case PLAYER_RAGE:///sch�sse f�r den rage
			if(typ_weapon==10)
			{
				pWeaponMan->add(12,posx+10,posy+6,0,WeaponVars::Speed[10],nr,WeaponVars::Energy[10]);
				pWeaponMan->add(12,posx+48,posy+6,0,WeaponVars::Speed[10],nr,WeaponVars::Energy[10]);
				shot_gefeuert+=2;
				pSound->playwav(9);
			}
			if(typ_weapon==11)
			{
				pWeaponMan->add(13,posx+9,posy+5,0,WeaponVars::Speed[11],nr,WeaponVars::Energy[11]);
				pWeaponMan->add(13,posx+47,posy+5,0,WeaponVars::Speed[11],nr,WeaponVars::Energy[11]);
				shot_gefeuert+=2;
				pSound->playwav(9);
			}
			if(typ_weapon==12)
			{
				pWeaponMan->add(14,posx,posy+1,0,WeaponVars::Speed[12],nr,WeaponVars::Energy[12]);
				pWeaponMan->add(14,posx+38,posy+1,0,WeaponVars::Speed[12],nr,WeaponVars::Energy[12]);
				shot_gefeuert+=2;
				pSound->playwav(9);
			}
			break;
		case PLAYER_FURY:
			if(typ_weapon==13)
			{
				pWeaponMan->add(15,posx+8,posy+4,0,WeaponVars::Speed[13],nr,WeaponVars::Energy[13]);
				pWeaponMan->add(15,posx+46,posy+4,0,WeaponVars::Speed[13],nr,WeaponVars::Energy[13]);
				shot_gefeuert+=2;
				pSound->playwav(7);
			}
			if(typ_weapon==14)
			{
				pWeaponMan->add(16,posx+2,posy+5,0,WeaponVars::Speed[14],nr,WeaponVars::Energy[14]);
				pWeaponMan->add(16,posx+40,posy+5,0,WeaponVars::Speed[14],nr,WeaponVars::Energy[14]);
				shot_gefeuert+=2;
				pSound->playwav(7);
			}
			if(typ_weapon==15)
			{
				pWeaponMan->add(17,posx,posy-5,0,WeaponVars::Speed[15],nr,WeaponVars::Energy[15]);
				shot_gefeuert+=1;
				pSound->playwav(7);
			}
			break;
	}
}

///hinzuf�gen eines Specials
void cPlayer::addspecial()
{
	cWeaponMan	*pWeaponMan=cWeaponMan::getInstance();
	cSystem		*pSystem=cSystem::getInstance();
	//hat der Spieler noch specials?

	if(pSystem->getgameover() || pSystem->getnextwave())
		return;

	if(specials<=0 || shieldstate==1)return;
	specials--;

	switch(typ_player)
	{
		case PLAYER_BANSHEE:
			pWeaponMan->add(18,posx+12,posy,0,WeaponVars::Speed[16],nr,WeaponVars::Energy[16]);
			shot_gefeuert++;
			break;
		case PLAYER_RAGE:
			pWeaponMan->add(21,posx+22,posy-10,0,WeaponVars::Speed[17],nr,WeaponVars::Energy[16]);
			shot_gefeuert++;
			break;	
		case PLAYER_FURY:
			pWeaponMan->add(22,posx+2,posy+2,0,WeaponVars::Speed[18],nr,WeaponVars::Energy[16]);
			pWeaponMan->add(22,posx+48,posy+2,0,WeaponVars::Speed[18],nr,WeaponVars::Energy[16]);
			pWeaponMan->add(22,posx+24,posy-6,0,WeaponVars::Speed[18],nr,WeaponVars::Energy[16]);
			shot_gefeuert+=3;
			break;
	}
}

///aktiviert der spieler einen special ja oder nein?
void cPlayer::setbspecial(bool wert)
{
	bspecial=wert;
}

///addiert zu dem bestehenden score weitere punkte
void cPlayer::addscore(int score)
{

	this->score+=score;
}

void cPlayer::addpwlife()
{
	if(lives<5)lives++;
}

void cPlayer::addpwgun()
{
	switch(typ_player)
	{
		case PLAYER_BANSHEE:
			if(typ_weapon<9)typ_weapon++;
			break;
		case PLAYER_FURY:
			if(typ_weapon<15)typ_weapon++;
			break;
		case PLAYER_RAGE:
			if(typ_weapon<12)typ_weapon++;
			break;
	}
}

void cPlayer::addpwspecial()
{
	if(specials<3)specials++;
}

void cPlayer::dellife()
{
	cSystem *pSystem=cSystem::getInstance();

	if(pSystem->getgamemode()==GMODE_SUICIDE)
	{
		score-=10000;
		if(score<0)score=0;

		shieldstate=1;
		shielddelay=SDL_GetTicks();
		return;
	}

	if(lives>0)
	{
		lives--;
		shieldstate=1;
		shielddelay=SDL_GetTicks();
	}	
}

bool cPlayer::alive()
{
	if(lives>0)return true;
	return false;
}

int cPlayer::getshieldstate()
{
	return shieldstate;
}

void cPlayer::setshieldstate(int wert)
{
	shieldstate=wert;
	if(shieldstate!=0)
		shielddelay=SDL_GetTicks();
}

///einen schuss f�r die statistik des spielers hinzuf�gen
void cPlayer::addshot_treffer()
{
	shot_treffer++;
}

///anzahl der getroffen sch�sse zur�ckgeben
long cPlayer::getshot_treffer()
{
	if(shot_treffer>0)
		return shot_treffer;
	else
		return 1;
}

///wie oft hat der spieler geschossen
long cPlayer::getshot_anzahl()
{
	return shot_gefeuert;
}

///die werte f�r die schuss-statistik des spielers zur�cksetzen
void cPlayer::shot_reset()
{
	shot_gefeuert=0;
	shot_treffer=0;
}

///r�ckgabe der punkte des spielers
long int cPlayer::getscore()
{
	return score;
}

///der spieler hat einen weiteren angriff �berlebt
void cPlayer::addwave()
{
	waves++;
}

///wievele angriffe(stages) hat er �berlebt
int cPlayer::getwaves()
{
	return waves;
}

void cPlayer::setnr(int nr)
{
	this->nr=nr;
}

int cPlayer::getnr()
{
	return nr;
}
