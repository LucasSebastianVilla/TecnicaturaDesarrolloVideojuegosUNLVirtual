#pragma once
#include <SFML\Graphics.hpp>
#include <iostream>

using namespace sf;
using namespace std;

class Tren {
private:
	bool enMovimiento;//habilita el Desplazamiento del tren
	bool Llegada= false;//Bandera booleana que indica si llego
	Texture *txTren;
	Sprite *spTren;
	bool bamboleo=true;//variable para simular el balanceo del tren en la via

public:
	Tren(){
	/** Constructor **/
		enMovimiento = true;//permito que el tren se mueva al comenzar el juego
		
		txTren = new Texture;
		spTren = new Sprite;
		
		txTren->loadFromFile("Recursos/Tren.png");
		spTren->setTexture(*txTren);
		spTren->setScale(0.5f, 0.5f);
		spTren->setPosition(0,75);//posiciono el tren
		
	};

	void dibujar(RenderWindow *Ventana) {Ventana->draw(*spTren);}
	bool moverTren() { return enMovimiento; };//valida si el tren se puede mover
	void posicionarTren(bool Mueve) { enMovimiento = Mueve; };//configuro si el tren se puede mover
	bool obtenerLlegada() { return Llegada; };//obtengo si llego a la meta
	FloatRect GetGlobalBounds() { return spTren->getGlobalBounds(); };//controlo la colision del tren
	Vector2f obtenerPosicion() { return spTren->getPosition(); };//obtengo la posicion del tren
	
	void Desplazamiento() {
	/** Muevo el tren y controlo su posicion **/
		if (enMovimiento) {//verifico que se pueda mover y lo desplazo, hago un pequeño movimiento hacia abajo y arriba para simular la via del tren
			if(bamboleo){spTren->setPosition(spTren->getPosition().x + 5, spTren->getPosition().y+0.5f);}
			else {spTren->setPosition(spTren->getPosition().x + 5, spTren->getPosition().y-0.5f);}
			bamboleo=!bamboleo;//cambio el valor para que recupere la posicion anterior
		}
		if (spTren->getPosition().x > 1300) {//llego al punto de retorno y lo posiciono mas abajo al principio
			spTren->setPosition(0, spTren->getPosition().y + 115);
		}
		if (spTren->getPosition().x > 650 && spTren->getPosition().y >= 530) {//si llego hasta el ultimo piso gano
			enMovimiento = false;//desactivo el movimiento
			Llegada = true;//marco que llegue
		}
	}

};
