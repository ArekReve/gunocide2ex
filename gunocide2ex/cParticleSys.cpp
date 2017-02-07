#include <time.h>
#include <math.h>
#include "cParticleSys.h"
#include "cUtil.h"

cParticleSys::cParticleSys()
{
	time_life=0;
	time_act=0;
	particles_max=0;
	particles_act=0;
	particles_rate=0;
	particles_act=0;
	speed_varx=0.0f;
	speed_vary=0.0f;
	start_varx=0.0f;
	start_vary=0.0f;
	start_posx=0;
	start_posy=0;
	time_act=SDL_GetTicks();
	bactive=true;
	bemit=true;

	srand((unsigned int)time(NULL));
}

cParticleSys::~cParticleSys()
{
	lParticles.clear();	
}

///hier werden alle Partikel aktualisiert usw.
void cParticleSys::update()
{
	LISTPARTICLES::iterator		p;
	int							time=SDL_GetTicks();
	cParticle					*pParticle=NULL;
	cUtil						*pUtil=cUtil::getInstance();

	//neue partikel erstellen
	if(bemit)
	{
	for(int i=0;i< particles_rate/(1000/(time-time_act));i++)
	{
		if(particles_act<particles_max)	
		{
			pParticle=new cParticle(time);
			
			pParticle->setposx(start_posx+((rand()%(int)start_varx)-start_varx/2));
			pParticle->setposy(start_posy+((rand()%(int)start_vary)-start_vary/2));
		
			//sin(i*3.141592654/180
			pParticle->setdirx(cos(rand()%360*(3.14/180))+
						(rand()%(int)speed_varx)-speed_varx/2);
			pParticle->setdiry(sin(rand()%360*(3.14/180))+
						(rand()%(int)speed_vary)-speed_vary/2);

			pParticle->setlifetime(time_life);

			lParticles.push_back(*pParticle);
			delete pParticle;
			pParticle=NULL;
			particles_act++;
		}
		else
		{
			bemit=false;
			break;
		}
	}
	}
	time_act=time;

	for(p=lParticles.begin();p!=lParticles.end();++p)
	{
		p->setposx(p->getposx()+pUtil->sync(p->getdirx()));
		p->setposy(p->getposy()+pUtil->sync(p->getdiry()));

		///wenn die lebenszeit des partikels zu ende ist
		///löschen wir es
		if(time-p->getstarttime()>p->getlifetime())
		{
			p=lParticles.erase(p);
		}
	}

	if(lParticles.empty() && bemit==false)
	{
		bactive=false;
	}
}

///zeichnen aller Partikel
void cParticleSys::render(SDL_Surface *screen,cSprite *sprite)
{
	LISTPARTICLES::iterator	p;

	for(p=lParticles.begin();p!=lParticles.end();++p)
	{
		sprite->render(screen,p->getposx(),p->getposy());
	}
}

void cParticleSys::setlifetime(int wert)
{
	time_life=wert;
}

void cParticleSys::setmaxparticles(int wert)
{
	particles_max=wert;
}

void cParticleSys::setparticlerate(int wert)
{
	particles_rate=wert;
}

void cParticleSys::setspeedvar(int x,int y)
{
	speed_varx=x;
	speed_vary=y;
}

void cParticleSys::setstartvar(int x,int y)
{
	start_varx=x;
	start_vary=y;
}

void cParticleSys::setstartpos(int x,int y)
{
	start_posx=x;
	start_posy=y;
}

bool cParticleSys::active()
{
	return bactive;
}
