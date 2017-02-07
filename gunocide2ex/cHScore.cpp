/***************************************************************************
                          cHScore.cpp  -  description
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
#include "cHScore.h"
#include "cSystem.h"
#include "cFont.h"
#include "cKeyboard.h"
#include "cSound.h"
#include "mmgr.h"

cHScore::cHScore()
{
	memset(&score,0,sizeof(SCOREENTRY)*(3*10));
	showmode=0;
	bnewscore=false;
	namepos=0;
	scorepos=0;
}

cHScore::~cHScore()
{
}

///einladen der highscores
///falls es die datei nicht gibt, erstellen wir einfach eine dummy-datei
///sofern man die rechte besitzt..wenn auch dies schief geht, haben wir halt ein problem ;)
bool cHScore::load()
{
	FILE 	*file=NULL;

	file=fopen("/usr/local/games/gunocide2ex/hscore.dat\0","rb");
	if(!file)
	{
		printf("failed to open hscore.dat\n");
		printf("creating dummy highscore tables...\n");
		createdummytables();
		if(!save())
			return false;
		else return true;
	}

	//abspeichern der scores
	for(int type=0;type<3;type++)
	{
		for(int s=0;s<10;s++)
		{
			fscanf(file,"%s %i %i %i\n",&score[type][s].name,
                       					&score[type][s].score,
										&score[type][s].hitratio,
										&score[type][s].waves);
		}
	}

	printf("hishcores loaded\n");
	fclose(file);
	return true;
}

///erstellen der dummy tables für die highscore liste
bool cHScore::createdummytables()
{
	//generieren der dummy highscores
	for(int type=0;type<3;type++)
	{
		for(int s=0;s<10;s++)
		{
			strcpy(score[type][s].name,"NoMoRe@\0");
			score[type][s].score=666;
			score[type][s].hitratio=100;
			score[type][s].waves=0;
		}
	}
	printf("dummy tables generated,saving to hscore.dat\n");
	return true;
}

cHScore* cHScore::instance=NULL;

cHScore* cHScore::getInstance()
{
	if(instance==NULL)
	{
		instance=new cHScore();
		return instance;
	}
	else return instance;
}

void cHScore::delInstance()
{
	if(instance)
	{
		delete instance;
		instance=NULL;
	}
}

///abspeichern der Highscores
///in der Datei hscore.dat
///gibt true oder false zurück..je nachdem =)
bool cHScore::save()
{
	FILE	*file=NULL;

	file=fopen("/usr/local/games/gunocide2ex/hscore.dat\0","wb");
	if(!file)
	{
		printf("failed to create hscore.dat\n");
		return false;
	}

	//abspeichern der scores
	for(int type=0;type<3;type++)
	{
		for(int s=0;s<10;s++)
		{
			fprintf(file,"%s %i %i %i\n",score[type][s].name,
                       					score[type][s].score,
										score[type][s].hitratio,
										score[type][s].waves);
		}
	}

	printf("highscores saved\n");
	fclose(file);
	return true;
}

///anzeigen der Highscores abhängig vom gewählten Modus
void cHScore::render(SDL_Surface *screen)
{
	cFont	*pFont=cFont::getInstance();

	//eine for-schleife funzt hier nicht, da die abstände zwischen den einzelnen
	//score-plätzen der grafik nicht gleich gross sind..verflucht sei oli ;)
	pFont->setcolor(255,0,0);
	switch(showmode)
	{
		case GMODE_SUICIDE-1:
			pFont->print(screen,240,80,"GAME MODE: SUICIDE\0");
			
			if(bnewscore && scorepos==0)
				pFont->setcolor(0,255,255);
			else
				pFont->setcolor(255,0,0);
			pFont->print(screen,53,102,"%s\0",score[showmode][0].name);
			pFont->print(screen,200,102,"%i\0",score[showmode][0].score);
			pFont->print(screen,400,102,"%i %HIT RATIO\0",score[showmode][0].hitratio);

			if(bnewscore && scorepos==1)
				pFont->setcolor(0,255,255);
			else
				pFont->setcolor(255,0,0);
			pFont->print(screen,53,142,"%s\0",score[showmode][1].name);
			pFont->print(screen,200,142,"%i\0",score[showmode][1].score);
			pFont->print(screen,400,142,"%i %HIT RATIO\0",score[showmode][1].hitratio);

			if(bnewscore && scorepos==2)
				pFont->setcolor(0,255,255);
			else
				pFont->setcolor(255,0,0);
			pFont->print(screen,53,184,"%s\0",score[showmode][2].name);
			pFont->print(screen,200,184,"%i\0",score[showmode][2].score);
			pFont->print(screen,400,184,"%i %HIT RATIO\0",score[showmode][2].hitratio);

			if(bnewscore && scorepos==3)
				pFont->setcolor(0,255,255);
			else
				pFont->setcolor(255,0,0);
			pFont->print(screen,53,221,"%s\0",score[showmode][3].name);
			pFont->print(screen,200,221,"%i\0",score[showmode][3].score);
			pFont->print(screen,400,221,"%i %HIT RATIO\0",score[showmode][3].hitratio);

			if(bnewscore && scorepos==4)
				pFont->setcolor(0,255,255);
			else
				pFont->setcolor(255,0,0);
			pFont->print(screen,53,261,"%s\0",score[showmode][4].name);
			pFont->print(screen,200,261,"%i\0",score[showmode][4].score);
			pFont->print(screen,400,261,"%i %HIT RATIO\0",score[showmode][4].hitratio);


			if(bnewscore && scorepos==5)
				pFont->setcolor(0,255,255);
			else
				pFont->setcolor(255,0,5);
			pFont->print(screen,53,301,"%s\0",score[showmode][5].name);
			pFont->print(screen,200,301,"%i\0",score[showmode][5].score);
			pFont->print(screen,400,301,"%i %HIT RATIO\0",score[showmode][5].hitratio);

			if(bnewscore && scorepos==6)
				pFont->setcolor(0,255,255);
			else
				pFont->setcolor(255,0,0);
			pFont->print(screen,53,342,"%s\0",score[showmode][6].name);
			pFont->print(screen,200,342,"%i\0",score[showmode][6].score);
			pFont->print(screen,400,342,"%i %HIT RATIO\0",score[showmode][6].hitratio);

			if(bnewscore && scorepos==7)
				pFont->setcolor(0,255,255);
			else
				pFont->setcolor(255,0,0);
			pFont->print(screen,53,382,"%s\0",score[showmode][7].name);
			pFont->print(screen,200,382,"%i\0",score[showmode][7].score);
			pFont->print(screen,400,382,"%i %HIT RATIO\0",score[showmode][7].hitratio);

			if(bnewscore && scorepos==8)
				pFont->setcolor(0,255,255);
			else
				pFont->setcolor(255,0,0);
			pFont->print(screen,53,422,"%s\0",score[showmode][8].name);
			pFont->print(screen,200,422,"%i\0",score[showmode][8].score);
			pFont->print(screen,400,422,"%i %HIT RATIO\0",score[showmode][8].hitratio);

			if(bnewscore && scorepos==9)
				pFont->setcolor(0,255,255);
			else

				pFont->setcolor(255,0,0);
			pFont->print(screen,53,458,"%s\0",score[showmode][9].name);
			pFont->print(screen,200,458,"%i\0",score[showmode][9].score);
			pFont->print(screen,400,458,"%i %HIT RATIO\0",score[showmode][9].hitratio);
			break;
		case GMODE_STORY-1:
			pFont->print(screen,240,80,"GAME MODE: STORY\0");

						if(bnewscore && scorepos==0)
				pFont->setcolor(0,255,255);
			else
				pFont->setcolor(255,0,0);
			pFont->print(screen,53,102,"%s\0",score[showmode][0].name);
			pFont->print(screen,200,102,"%i\0",score[showmode][0].score);
			pFont->print(screen,400,102,"%i %HIT RATIO\0",score[showmode][0].hitratio);

			if(bnewscore && scorepos==1)
				pFont->setcolor(0,255,255);
			else
				pFont->setcolor(255,0,0);
			pFont->print(screen,53,142,"%s\0",score[showmode][1].name);
			pFont->print(screen,200,142,"%i\0",score[showmode][1].score);
			pFont->print(screen,400,142,"%i %HIT RATIO\0",score[showmode][1].hitratio);

			if(bnewscore && scorepos==2)
				pFont->setcolor(0,255,255);
			else
				pFont->setcolor(255,0,0);
			pFont->print(screen,53,184,"%s\0",score[showmode][2].name);
			pFont->print(screen,200,184,"%i\0",score[showmode][2].score);
			pFont->print(screen,400,184,"%i %HIT RATIO\0",score[showmode][2].hitratio);

			if(bnewscore && scorepos==3)
				pFont->setcolor(0,255,255);
			else
				pFont->setcolor(255,0,0);
			pFont->print(screen,53,221,"%s\0",score[showmode][3].name);
			pFont->print(screen,200,221,"%i\0",score[showmode][3].score);
			pFont->print(screen,400,221,"%i %HIT RATIO\0",score[showmode][3].hitratio);

			if(bnewscore && scorepos==4)
				pFont->setcolor(0,255,255);
			else
				pFont->setcolor(255,0,0);
			pFont->print(screen,53,261,"%s\0",score[showmode][4].name);
			pFont->print(screen,200,261,"%i\0",score[showmode][4].score);
			pFont->print(screen,400,261,"%i %HIT RATIO\0",score[showmode][4].hitratio);


			if(bnewscore && scorepos==5)
				pFont->setcolor(0,255,255);
			else
				pFont->setcolor(255,0,5);
			pFont->print(screen,53,301,"%s\0",score[showmode][5].name);
			pFont->print(screen,200,301,"%i\0",score[showmode][5].score);
			pFont->print(screen,400,301,"%i %HIT RATIO\0",score[showmode][5].hitratio);

			if(bnewscore && scorepos==6)
				pFont->setcolor(0,255,255);
			else
				pFont->setcolor(255,0,0);
			pFont->print(screen,53,342,"%s\0",score[showmode][6].name);
			pFont->print(screen,200,342,"%i\0",score[showmode][6].score);
			pFont->print(screen,400,342,"%i %HIT RATIO\0",score[showmode][6].hitratio);

			if(bnewscore && scorepos==7)
				pFont->setcolor(0,255,255);
			else
				pFont->setcolor(255,0,0);
			pFont->print(screen,53,382,"%s\0",score[showmode][7].name);
			pFont->print(screen,200,382,"%i\0",score[showmode][7].score);
			pFont->print(screen,400,382,"%i %HIT RATIO\0",score[showmode][7].hitratio);

			if(bnewscore && scorepos==8)
				pFont->setcolor(0,255,255);
			else
				pFont->setcolor(255,0,0);
			pFont->print(screen,53,422,"%s\0",score[showmode][8].name);
			pFont->print(screen,200,422,"%i\0",score[showmode][8].score);
			pFont->print(screen,400,422,"%i %HIT RATIO\0",score[showmode][8].hitratio);

			if(bnewscore && scorepos==9)
				pFont->setcolor(0,255,255);
			else
				pFont->setcolor(255,0,0);
			pFont->print(screen,53,458,"%s\0",score[showmode][9].name);
			pFont->print(screen,200,458,"%i\0",score[showmode][9].score);
			pFont->print(screen,400,458,"%i %HIT RATIO\0",score[showmode][9].hitratio);
			break;
		case GMODE_ENDLESS-1:
			pFont->print(screen,240,80,"GAME MODE: ENDLESS\0");
			
			if(bnewscore && scorepos==0)
				pFont->setcolor(0,255,255);
			else
				pFont->setcolor(255,0,0);
			pFont->print(screen,53,102,"%s\0",score[showmode][0].name);
			pFont->print(screen,200,102,"%i\0",score[showmode][0].score);
			pFont->print(screen,400,102,"%i WAVES\0",score[showmode][0].waves);

			if(bnewscore && scorepos==1)
				pFont->setcolor(0,255,255);
			else
				pFont->setcolor(255,0,0);
			pFont->print(screen,53,142,"%s\0",score[showmode][1].name);
			pFont->print(screen,200,142,"%i\0",score[showmode][1].score);
			pFont->print(screen,400,142,"%i WAVES\0",score[showmode][1].waves);

			if(bnewscore && scorepos==2)
				pFont->setcolor(0,255,255);
			else
				pFont->setcolor(255,0,0);
			pFont->print(screen,53,184,"%s\0",score[showmode][2].name);
			pFont->print(screen,200,184,"%i\0",score[showmode][2].score);
			pFont->print(screen,400,184,"%i WAVES\0",score[showmode][2].waves);

			if(bnewscore && scorepos==3)
				pFont->setcolor(0,255,255);
			else
				pFont->setcolor(255,0,0);
			pFont->print(screen,53,221,"%s\0",score[showmode][3].name);
			pFont->print(screen,200,221,"%i\0",score[showmode][3].score);
			pFont->print(screen,400,221,"%i WAVES\0",score[showmode][3].waves);

			if(bnewscore && scorepos==4)
				pFont->setcolor(0,255,255);
			else
				pFont->setcolor(255,0,0);
			pFont->print(screen,53,261,"%s\0",score[showmode][4].name);
			pFont->print(screen,200,261,"%i\0",score[showmode][4].score);
			pFont->print(screen,400,261,"%i WAVES\0",score[showmode][4].waves);


			if(bnewscore && scorepos==5)
				pFont->setcolor(0,255,255);
			else
				pFont->setcolor(255,0,5);
			pFont->print(screen,53,301,"%s\0",score[showmode][5].name);
			pFont->print(screen,200,301,"%i\0",score[showmode][5].score);
			pFont->print(screen,400,301,"%i WAVES\0",score[showmode][5].waves);

			if(bnewscore && scorepos==6)
				pFont->setcolor(0,255,255);
			else
				pFont->setcolor(255,0,0);
			pFont->print(screen,53,342,"%s\0",score[showmode][6].name);
			pFont->print(screen,200,342,"%i\0",score[showmode][6].score);
			pFont->print(screen,400,342,"%i WAVES\0",score[showmode][6].waves);

			if(bnewscore && scorepos==7)
				pFont->setcolor(0,255,255);
			else
				pFont->setcolor(255,0,0);
			pFont->print(screen,53,382,"%s\0",score[showmode][7].name);
			pFont->print(screen,200,382,"%i\0",score[showmode][7].score);
			pFont->print(screen,400,382,"%i WAVES\0",score[showmode][7].waves);

			if(bnewscore && scorepos==8)
				pFont->setcolor(0,255,255);
			else
				pFont->setcolor(255,0,0);
			pFont->print(screen,53,422,"%s\0",score[showmode][8].name);
			pFont->print(screen,200,422,"%i\0",score[showmode][8].score);
			pFont->print(screen,400,422,"%i WAVES\0",score[showmode][8].waves);

			if(bnewscore && scorepos==9)
				pFont->setcolor(0,255,255);
			else
				pFont->setcolor(255,0,0);
			pFont->print(screen,53,458,"%s\0",score[showmode][9].name);
			pFont->print(screen,200,458,"%i\0",score[showmode][9].score);
			pFont->print(screen,400,458,"%i WAVES\0",score[showmode][9].waves);
			break;
	}
	pFont->setcolor(255,255,255);
}

///überprüfen ob die score es in die highscore-liste geschafft hat
///wenn ja, setzen wir eine flag dass der name eingegeben werden muss sobald
///wir im menü sind
void cHScore::checkfornewentry(long tscore,int ratio,int waves,int gmode)
{
	bool		check=false;
	SCOREENTRY	temp;

	///wenn der score nicht mal den letzten platz schlägt
	///können wir getrost rausgehen...der spieler ist einfach ein looser ;)
	if(tscore<score[gmode][9].score)
		return;

	score[gmode][9].score=tscore;
	score[gmode][9].waves=waves;
	score[gmode][9].hitratio=ratio;
	strcpy(score[gmode][9].name,"NEW ENTRY\0");
	
	memset(&temp,0,sizeof(SCOREENTRY));

	bnewscore=true;

	///hier ein ganz einfacher bubble-sort
	///für ne highscore-liste benötigen wir kein merge oder ähnliches
	while(!check)
	{
		check=true;

		for(int i=9;i>0;i--)
		{
			if(score[gmode][i].score>score[gmode][i-1].score)
			{
				check=false;

				///vertauschen der einträge
				memcpy(&temp,&score[gmode][i],sizeof(SCOREENTRY));
				
				memcpy(&score[gmode][i],&score[gmode][i-1],sizeof(SCOREENTRY));

				memcpy(&score[gmode][i-1],&temp,sizeof(SCOREENTRY));
			}
		}
	}

	///jetzt schaun wir noch mal nach auf welchem platz er steht
	for(int i=0;i<10;i++)
	{
		if(strcmp(score[gmode][i].name,"NEW ENTRY\0")==0)
		{
			scorepos=i;
			break;
		}
	}
}

///setzt welche highscores angezeigt werden sollen
void cHScore::setmode(int mode)
{
	showmode=mode-1;
}

bool cHScore::entername()
{
	return bnewscore;
}

///der neue name wird eingetragen
void cHScore::update()
{
	cKeyboard	*pKeyboard=cKeyboard::getInstance();
	cSound		*pSound=cSound::getInstance();
	char		buffer[10];
	KeyEvent	*kevent=NULL;

	strcpy(buffer,pKeyboard->getlastkey());

	if(strcmp(pKeyboard->getlastkey(),"\0")!=0 && strlen(buffer)<2)
	{
		///nur zeichen zwischen a - z werden angenommen!
		if((buffer[0]>='a' && buffer[0]<='z') || (buffer[0]>='A' && buffer[0]<='Z'))
		{
			score[showmode][scorepos].name[namepos]=buffer[0];
			if(namepos<9)
				namepos++;
		}
	}

	//wir gehen einen buchstaben zurück
	kevent=pKeyboard->getkey(SDLK_BACKSPACE);
	if(kevent && kevent->pressed==1)
	{
		score[showmode][scorepos].name[namepos]=' ';
		if(namepos>0)namepos--;
		delete kevent;	
		kevent=NULL;
	} else kevent=NULL;

	//ein leerzeichen einfügen
	kevent=pKeyboard->getkey(SDLK_SPACE);
	if(kevent && kevent->pressed==1)
	{
		score[showmode][scorepos].name[namepos]=' ';
		if(namepos<9)namepos++;
		delete kevent;	
		kevent=NULL;
	} else kevent=NULL;

	//fertig mit der eingabe
	kevent=pKeyboard->getkey(SDLK_RETURN);
	if(kevent && kevent->pressed==1)
	{
		namepos=0;
		scorepos=0;
		bnewscore=false;
		pSound->playwav(3);

		delete kevent;	
		kevent=NULL;
	} else kevent=NULL;
}
