#ifndef CPARTICLE_SYS
#define CPARTICLE_SYS

#include <stdio.h>
#include <list>
#include "cSprite.h"
#include "cParticle.h"

using namespace std;
typedef list<cParticle> LISTPARTICLES;

class cParticleSys
{
public:
	cParticleSys();
	~cParticleSys();

	void	update();
	void	render(SDL_Surface *screen,cSprite *sprite);

	void	setlifetime(int wert);
	void	setmaxparticles(int wert);
	void	setparticlerate(int wert);
	void	setspeedvar(int x,int y);
	void	setstartvar(int x,int y);
	void	setstartpos(int x,int y);

	bool	active();
private:
	int		time_life;
	int		time_act;
	int		particles_max;
	int		particles_act;
	int		particles_rate;
	float	speed_varx;
	float	speed_vary;
	float	start_varx;
	float	start_vary;
	int		start_posx;
	int		start_posy;
	bool    bactive;
	bool	bemit;

	LISTPARTICLES	lParticles;
};
#endif
