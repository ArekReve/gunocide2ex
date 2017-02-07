#ifndef CPARTICLE_H
#define CPARTICLE_H

class cParticle
{
public:
	cParticle(int time);
	~cParticle();

	void	setposx(int x);
	int	getposx();
	void	setposy(int y);
	int	getposy();
	void	setdirx(float x);
	float	getdirx();
	void	setdiry(float y);
	float	getdiry();

	void	setlifetime(int time);
	int	getlifetime();
	int	getstarttime();
private:
	int	posx;
	int	posy;
	int	time_life;
	int	time_start;
	float	dir_x;
	float	dir_y;
};
#endif
