#pragma once
#include "Afichmation.h"
#include <SFML/Audio.hpp>

using namespace std;
using namespace sf;

/*Clase encargada de controlar todo lo que sea referente al
bloque de ladrillos donde el personaje colisiona, tambien donde
se agregan los nros del arreglo*/


//DECLARACION DE CONSTANTES
const int BLOQUE_POSICION_TERRENO_Y = 250;
const int MAX_ELEVACION_GOLPE_Y = 270;

class Bloque {
private:
	Sprite *sbloque;
	bool enAscenso = false;
	int nroBloque;
	bool colorBloq = false;//falso AZUL - verdadero VERDE // Tambien se usa para el estado del bloque
	Sound acierto;//sonidos para demarcar cada evento en el juego
	sf::SoundBuffer sbacierto;//buffer para los sonidos
public:
	Bloque(Texture *tex, int BLOQUE_POSICION_TERRENO_X, int nro) {//construyo el bloque de ladrillos
		sbloque = new Sprite(*tex);
		nroBloque = nro;
		sbloque->setPosition(BLOQUE_POSICION_TERRENO_X, BLOQUE_POSICION_TERRENO_Y);
		if (!sbacierto.loadFromFile("Recursos/mbwoohoo.wav")) { cout << "No se pudo cargar el sonido" << endl; }
		acierto.setBuffer(sbacierto);
	}
	//~Bloque();
	
	void SetNumero(int nro){//configuro el nro en el bloque
		nroBloque=nro;
	}
	
	int GetNumero(){//Obtengo el nro que tiene el bloque
		return nroBloque;
	}
		
	void Dibujar(RenderWindow* w) {
		w->draw(*sbloque);
	}
	
	FloatRect GetGlobalBounds(){//obtengo los limites del bloque
		return sbloque->getGlobalBounds();
	}
	bool obtieneColor(){//obtengo el color que tiene el nro 
		return colorBloq;
	}
		
	void asignaColor(){//cambio el color del nro
		colorBloq=true;
		acierto.play();
	}
	
};
