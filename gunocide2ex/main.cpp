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

#include "cSystem.h"
#include "mmgr.h"

int main( int argc, char* argv[] )
{
	cSystem *pSystem=cSystem::getInstance();

	if(!pSystem->init())
	{
		printf("it looks like something was going wrong...\n");
		printf("check if you have installed g2ex the correct way...\n");
		printf("if you don't know what to do check the FAQ or send a mail to alex@polyfrag.com\n");
		return -1;
	}

	pSystem->gameLoop();

	cSystem::delInstance();

	printf("thanks for playing Gunocide II EX\n");
	printf("we hope you enjoyed it!\n");
	return 0;
}
