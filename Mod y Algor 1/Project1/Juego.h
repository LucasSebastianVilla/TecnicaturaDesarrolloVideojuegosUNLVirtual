#pragma once
#include "Persona.h"

class Juego
{
private:
	sf::Clock tiempo;//controlo el tiempo para mostrar a los personajes
	int anterior=0; //variable para evitar que se repita el personaje
	Event evt;
public:
	void cargarFondo();//seteo el fondo del juego
	Juego();
	void play();
	int elijePersonaje();//elijo el personaje a mostrar
	void presentacion();//Sprite de presentacion del juego
	int calculaScore(int z1, int z2);//calculo si mate mas de 3 inocentes o si llegue a 10 puntos

};

