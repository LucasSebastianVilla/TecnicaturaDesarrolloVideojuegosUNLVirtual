// Integradores.cpp: define el punto de entrada de la aplicación de consola.
//

#include "stdafx.h"
#include "Game.h"

int _tmain(int argc, _TCHAR* argv[])
{
	bool sigue=false;
	do
	{
		Game game;
		game.Loop();
		if (game.getOtravez()) { sigue = true; }
		else { sigue = false; }
	} while (sigue);
	return 0;
}

