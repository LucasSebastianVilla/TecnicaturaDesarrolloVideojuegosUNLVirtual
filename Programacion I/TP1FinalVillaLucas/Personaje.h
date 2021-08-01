#pragma once
#include "Afichmation.h"
#include <SFML/Audio.hpp>

using namespace sf;

//DECLARACION DE CONSTANTES
const int POSICION_TERRENO_Y = 485;
const int MAX_ASCENSO_SALTO_Y = 170;

class Personaje {
private:
	Afichmation *sprite;
	bool teclaSaltoPresionada = false;
	bool enAscenso = false;
	//bool saltoEnProceso = true;
	bool sentidoDerecha = false;
	bool colision =true;
	Sound saltar;//sonidos para demarcar cada evento en el juego
	sf::SoundBuffer sbsaltar;//buffer para los sonidos
	bool controlarColision = true;
public:
	bool saltoEnProceso = true;
	Personaje(Texture &tex) {
		sprite = new Afichmation(tex, true, 208, 249);
		sprite->Add("idle", { 0, 1, 2, 1 }, 8, true);
		sprite->Add("walking", { 3, 4, 5, 6, 7, 8, 9, 10, 11, 10, 9, 8, 7, 6, 5, 4 }, 12, true);
		sprite->Add("jump", { 12, 13, 14 }, 8, false);
		sprite->Add("collide", { 15 }, 8, false);
		sprite->Play("idle");
		sprite->setScale(.3f, .3f);
		sprite->setPosition(50, POSICION_TERRENO_Y - 500);
		if (!sbsaltar.loadFromFile("Recursos/mbsalto.wav")) { cout << "No se pudo cargar el sonido" << endl; }
		saltar.setBuffer(sbsaltar);
	}

	void ControlarDesplazamiento() {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			sprite->FlipX(false);
			sprite->setPosition(sprite->getPosition().x - 5, sprite->getPosition().y);
			if (!saltoEnProceso)
				sprite->Play("walking");
			else{
				sprite->Play("jump");
				if(colision){sprite->Play("collide");}
			}
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			sprite->FlipX(true);
			sprite->setPosition(sprite->getPosition().x + 5, sprite->getPosition().y);
			if (!saltoEnProceso)
				sprite->Play("walking");
			else{
				sprite->Play("jump");}
		}
		else if (saltoEnProceso) {
			sprite->Play("jump");
			if(colision){sprite->Play("collide");}
		}
		else {
			sprite->Play("idle");
		}
	}
	
	void setColision(bool colis){
		colision=colis;
	}
	
	void ControlarSalto(Event *event) {
		if (event->type == sf::Event::KeyPressed && event->key.code == sf::Keyboard::Space) {
			saltar.play();
			teclaSaltoPresionada = true;
		}
		else if (event->type == sf::Event::KeyReleased && event->key.code == sf::Keyboard::Space) {
			teclaSaltoPresionada = false;
			controlarColision = true;//acá es mejor
		}
	}
	
	bool obtenerColision(){
		return colision;
	}
	
	void Dibujar(RenderWindow* w) {
		w->draw(*sprite);
	}

	void Actualizar() {
		//Se chequea si se presionó la tecla de salto y si el proceso aún no inició
		if (teclaSaltoPresionada && !saltoEnProceso) {
			saltoEnProceso = true;
			enAscenso = true;
		}

		//Bloque que maneja el salto y su descenso a través del eje 'y'.
		if (saltoEnProceso) {
			//Se controla cuando se llega al maximo de altura de salto
			if (sprite->getPosition().y <= POSICION_TERRENO_Y - MAX_ASCENSO_SALTO_Y) {
				enAscenso = false;
			}
			//Se ejecuta el movimiento hacia arriba o abajo en eje 'y'
			if (enAscenso) {
				sprite->setPosition(sprite->getPosition().x, sprite->getPosition().y - 5);
			}
			else {
				sprite->setPosition(sprite->getPosition().x, sprite->getPosition().y + 5);
			}
			//Se controla cuando el personaje vuelve a tocar el eje 'y' del terreno
			if (sprite->getPosition().y >= POSICION_TERRENO_Y) {
				saltoEnProceso = false; //Permite que finalize un ciclo de salto y que comienze uno nuevo
				sprite->setPosition(sprite->getPosition().x, POSICION_TERRENO_Y); //Se fuerza la posicion del personaje en la linea de terreno
			}
		}
		sprite->Update();
	}
	
	FloatRect GetGlobalBounds() {
		return sprite->getGlobalBounds();
	}
	
	void ControlaColision(bool c) {
		controlarColision = c;
	}
	
	bool EstadoControlColision() { return controlarColision; }

};
