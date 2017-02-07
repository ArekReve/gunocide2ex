#include "cParticle.h"

cParticle::cParticle(int time)
{
	time_start=time;

	posx=0;
	posy=0;
	dir_x=0.0f;
	dir_y=0.0f;
	time_life=0;
}

cParticle::~cParticle()
{
}

void cParticle::setposx(int x)
{
	posx=x;
}

int cParticle::getposx()
{
	return posx;
}

void cParticle::setposy(int y)
{
	posy=y;
}

int cParticle::getposy()
{
	return posy;
}

void cParticle::setlifetime(int time)
{
	time_life=time;
}

int cParticle::getlifetime()
{
	return time_life;
}

int cParticle::getstarttime()
{
	return time_start;
}

void cParticle::setdirx(float x)
{
	dir_x=x;
}

float cParticle::getdirx()
{
	return dir_x;
}

void cParticle::setdiry(float y)
{
	dir_y=y;
}

float cParticle::getdiry()
{
	return dir_y;
}
