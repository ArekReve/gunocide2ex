######################################################################################
#Gunocide II EX makefile
#(c) 2001-02 by polyfrag
######################################################################################
all: g2ex
dist: g2ex

g2ex: cPlayer.o cSystem.o main.o cSprite.o cStage.o cSpriteAnim.o \
	 cEnemyMan.o cEnemy.o cDisplay.o cWeaponMan.o cWeapon.o cUtil.o \
	 cEffects.o cSound.o cMenu.o cKeyboard.o cFont.o cPowerup.o mmgr.o \
     cParticle.o cHScore.o cCredits.o cPlayerMan.o cParticleSys.o
		g++ cPlayer.o cSystem.o cSprite.o main.o cStage.o \
		cSpriteAnim.o cEnemyMan.o cEnemy.o cDisplay.o cWeaponMan.o \
		cWeapon.o cUtil.o cEffects.o cSound.o cMenu.o cKeyboard.o\
		cFont.o cPowerup.o mmgr.o cParticle.o cHScore.o cCredits.o\
		cPlayerMan.o cParticleSys.o\
		-g -o ./binary/g2ex -lpthread -L/usr/lib -lSDL -lSDL_ttf -lSDL_mixer


cStage.o: ./gunocide2ex/cStage.h ./gunocide2ex/cStage.cpp
		g++ -c -g ./gunocide2ex/cStage.cpp

cPlayer.o: ./gunocide2ex/cPlayer.h ./gunocide2ex/cPlayer.cpp
		g++ -c -g ./gunocide2ex/cPlayer.cpp

cSystem.o: ./gunocide2ex/cSystem.h ./gunocide2ex/cSystem.cpp
		g++ -c -g ./gunocide2ex/cSystem.cpp

cSprite.o: ./gunocide2ex/cSprite.h ./gunocide2ex/cSprite.cpp
		g++ -c -g ./gunocide2ex/cSprite.cpp

cSpriteAnim.o: ./gunocide2ex/cSpriteAnim.h ./gunocide2ex/cSpriteAnim.cpp
		g++ -c -g ./gunocide2ex/cSpriteAnim.cpp

cEnemy.o: ./gunocide2ex/cEnemy.h ./gunocide2ex/cEnemy.cpp
		g++ -c -g ./gunocide2ex/cEnemy.cpp

cEnemyMan.o: ./gunocide2ex/cEnemyMan.h ./gunocide2ex/cEnemyMan.cpp
		g++ -c -g ./gunocide2ex/cEnemyMan.cpp

cDisplay.o: ./gunocide2ex/cDisplay.h ./gunocide2ex/cDisplay.cpp
		g++ -c -g ./gunocide2ex/cDisplay.cpp

cWeaponMan.o: ./gunocide2ex/cWeaponMan.h ./gunocide2ex/cWeaponMan.cpp
		g++ -c -g ./gunocide2ex/cWeaponMan.cpp

cWeapon.o: ./gunocide2ex/cWeapon.h ./gunocide2ex/cWeapon.cpp
		g++ -c -g ./gunocide2ex/cWeapon.cpp

cUtil.o: ./gunocide2ex/cUtil.h ./gunocide2ex/cUtil.cpp
		g++ -c -g ./gunocide2ex/cUtil.cpp

main.o: ./gunocide2ex/main.cpp
		g++ -c -g ./gunocide2ex/main.cpp

cEffects.o: ./gunocide2ex/cEffects.h ./gunocide2ex/cEffects.cpp
		g++ -c -g ./gunocide2ex/cEffects.cpp

cSound.o: ./gunocide2ex/cSound.h ./gunocide2ex/cSound.cpp
		g++ -c -g ./gunocide2ex/cSound.cpp

cMenu.o: ./gunocide2ex/cMenu.h ./gunocide2ex/cMenu.cpp
		g++ -c -g ./gunocide2ex/cMenu.cpp

cKeyboard.o: ./gunocide2ex/cKeyboard.h ./gunocide2ex/cKeyboard.cpp
		g++ -c -g ./gunocide2ex/cKeyboard.cpp

cFont.o: ./gunocide2ex/cFont.h ./gunocide2ex/cFont.cpp
	g++ -c -g ./gunocide2ex/cFont.cpp

cPowerup.o: ./gunocide2ex/cPowerup.h ./gunocide2ex/cPowerup.cpp
	g++ -c -g ./gunocide2ex/cPowerup.cpp

mmgr.o: ./gunocide2ex/mmgr.h ./gunocide2ex/mmgr.cpp
	g++ -c -g ./gunocide2ex/mmgr.cpp

cParticle.o: ./gunocide2ex/cParticle.h ./gunocide2ex/cParticle.cpp
	g++ -c -g ./gunocide2ex/cParticle.cpp

cParticleSys.o: ./gunocide2ex/cParticleSys.h ./gunocide2ex/cParticleSys.cpp
	g++ -c -g ./gunocide2ex/cParticleSys.cpp

cHScore.o: ./gunocide2ex/cHScore.h ./gunocide2ex/cHScore.cpp
	g++ -c -g ./gunocide2ex/cHScore.cpp

cCredits.o: ./gunocide2ex/cCredits.h ./gunocide2ex/cCredits.cpp
	g++ -c -g ./gunocide2ex/cCredits.cpp

cPlayerMan.o: ./gunocide2ex/cPlayerMan.h ./gunocide2ex/cPlayerMan.cpp
	g++ -c -g ./gunocide2ex/cPlayerMan.cpp


##alle Objektdateien löschen
clean: 	
	@echo cleaning...
	@rm -f *.o
	@echo done...

##Spiel installieren
install:
	@echo installing Gunocide II EX...
	@echo ----------------------------
	@echo warning: you must be root to install this game
	@echo all needed files will be copied to /usr/local/games/gunocide2ex
	@echo
	@echo creating directories...
	@mkdir /usr/local/games/gunocide2ex
	@echo copying files...
	@cd binary/
	@cp -r binary/* /usr/local/games/gunocide2ex
	@echo setting rights...
	@chmod a+rwx /usr/local/games/gunocide2ex
	@echo creating link..binary/.
	@ln -s /usr/local/games/gunocide2ex/g2ex /usr/bin/g2ex
	@echo game installed...
	@echo enjoy playing this game...
