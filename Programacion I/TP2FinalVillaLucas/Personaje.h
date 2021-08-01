#pragma once
#include "Afichmation.h"
#include<SFML\Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace sf;

const int POSICION_TERRENO_X = 400;

class Personaje {
private:
	Afichmation *sprite;
	bool teclaSaltoPresionada = false;
	bool sentidoDerecha = false;
	bool colision =true;
	Sound saltar;//sonidos para demarcar cada evento en el juego
	sf::SoundBuffer sbsaltar;//buffer para los sonidos
	bool controlarColision = true;
	int POSICION_TERRENO_Y = 495;
	
public:
	Personaje(Texture &tex) {
		sprite = new Afichmation(tex, true, 208, 249);
		sprite->Add("idle", { 0, 1, 2, 1 }, 8, true);
		sprite->Add("walking", { 3, 4, 5, 6, 7, 8, 9, 10, 11, 10, 9, 8, 7, 6, 5, 4 }, 12, true);
		sprite->Add("jump", { 12, 13, 14 }, 8, false);
		sprite->Add("collide", { 15 }, 8, false);
		sprite->Play("idle");
		sprite->setScale(.25f, .25f);
		sprite->setPosition(POSICION_TERRENO_X, POSICION_TERRENO_Y);
		if (!sbsaltar.loadFromFile("Recursos/mbsalto.wav")) { cout << "No se pudo cargar el sonido" << endl; }
		saltar.setBuffer(sbsaltar);
	}
	FloatRect GetGlobalBounds() {return sprite->getGlobalBounds();}
	void ControlaColision(bool c) {controlarColision = c;}
	bool EstadoControlColision() { return controlarColision; }
	bool saltoEnProceso = true;
	
	void setPosicion(int x, int y){sprite->setPosition(x, y);}
	Vector2f getPosicion(){return sprite->getPosition();}
	
	void setColision(){
		POSICION_TERRENO_Y = 495;
		//colision=colis;
	}
	bool obtenerColision(){return colision;}
	void Dibujar(RenderWindow* w) {w->draw(*sprite);}
	
	void Actualizar() {
		//Se chequea si se presionó la tecla de salto y si el proceso aún no inició
		if (teclaSaltoPresionada && !saltoEnProceso) {
			saltoEnProceso = true;
		}
		//Bloque que maneja el salto y su descenso a través del eje 'y'.
		if (saltoEnProceso) {
			//Se ejecuta el movimiento hacia arriba o abajo en eje 'y'
			//Se controla cuando el personaje vuelve a tocar el eje 'y' del terreno
			if (sprite->getPosition().y >= POSICION_TERRENO_Y) {
				saltoEnProceso = false; //Permite que finalize un ciclo de salto y que comienze uno nuevo
				sprite->setPosition(sprite->getPosition().x, POSICION_TERRENO_Y); //Se fuerza la posicion del personaje en la linea de terreno
			}
		}
		sprite->Update();
	}
	
	void ControlarSalto(Event *event) {
		if (event->type == sf::Event::KeyPressed && event->key.code == sf::Keyboard::Space) {
			POSICION_TERRENO_Y=POSICION_TERRENO_Y-75;
			if(POSICION_TERRENO_Y>0){
				saltar.play();
				sprite->setPosition(sprite->getPosition().x, POSICION_TERRENO_Y);
			} 
		}
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
		}
		else if (saltoEnProceso) {
			sprite->Play("jump");
			if(colision){sprite->Play("collide");}
		}
		else {
			sprite->Play("idle");
		}
	}	
};
