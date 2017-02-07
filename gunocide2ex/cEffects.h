#ifndef CEFFECTS_H
#define CEFFECTS_H

#include <stdlib.h>
#include <list>
#include "cSpriteAnim.h"
#include "cParticleSys.h"

#define EFFECT_EXPLOSION	0
#define EFFECT_FLASH		1
#define EFFECT_PEXPLOSION	2
#define EFFECT_PHIT			3

using namespace std;

class cEffect
{
public:
	cEffect();
	~cEffect();

	void 	add(int type,int posx,int posy);
	bool 	update();
	void 	render(SDL_Surface *screen);
private:
	int 	posx;
	int 	posy;
	int 	type;
	cAnim	anim_sprite;
};

typedef list<cEffect> LISTEFFECTS;
typedef list<cParticleSys> LISTPARTICLESYS;

class cEffectMan
{
public:
	cEffectMan();
	~cEffectMan();

	static cEffectMan*	getInstance();
	static void			delInstance();
	
	bool				load();
	void				add(int type,int posx,int posy);
	void				update();
	void				render(SDL_Surface *screen);
	
	cSpriteAnim*		getGFX();
private:
	static cEffectMan*	instance;
	cSpriteAnim			gfx_sprite;
	cSprite				gfx_particle;
	bool				effect_flash;

	LISTEFFECTS			lEffects;
	LISTPARTICLESYS		lParticles;
};
#endif

