#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\System.hpp>
#include <SFML\Window.hpp>
#include "BaseObject.h"
#include <SFML/Audio.hpp>

using namespace sf;

class Game
{
public:

//métodos
	Game();
	~Game();
	void Loop();
	void Presentacion();
	int Aleatorio(int _NroLimite, int _NroNegativo);
	bool Colision(BaseObject *pers1, BaseObject *pers2);
	void setOtravez();
	bool getOtravez();
	int _calculaCondicion();
//variables
	Sprite	sfondo, sreticula;
	Sound disparo,mfondo, explosion, naves, inicio, loser, winner;
	Event evt;
private:
//métodos
	void _Draw();
	void _Update(float dt);
	void _ProcessEvents();
//variables
	float dv = 0.0f;
	RenderWindow* _pVentana;
	BaseObject* _pPersonaje[9];
	Texture	fondo, reticula;
	Font fuente;
	Text contador, contadorA, mensaje, mensaje1, contadorM;
	sf::SoundBuffer sbdisparo, sbmfondo, sbexplosion, sbnaves, sbinicio, sbloser, sbwinner;
	int puntaje = 0, puntajeA = 0, municion=6;
	int personaje;
	bool _otravez=true;
};