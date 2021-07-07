#include "Juego.h"
#include "SFML/Graphics.hpp"
#include <SFML/Window.hpp>
#include <iostream>
#include "Windows.h"
#include "Persona.h"
#include "Time.h"

/*IDEAS: Hacer un case donde con dos rand voy a seleccionar tipo de personaje y posicion
Otra opcion es meter en la clase personaje las 2 opciones de tipo con un case y en el constructor
pasarle el parametro de cual es la que quiero, podria ver si tambien se puede hacer con la posicion eso de
pasarla por parametros*/



using namespace sf;
Juego::Juego() {};

void Juego::cargarFondo() {
	Sprite sfondo, scursor,simpacto,spunto, samigo, senemigo, sdisparo, scartel;
	Texture fondo, cursor, impacto,punto,amigo,enemigo, disparo, cartel;
	int puntos=0, vidas=0, enemigos=0;
	int control;

	presentacion();//presento el juego y sus reglas
	
	/*leo todas las texturas y se los asigno a sus respectivos sprites*/
	fondo.loadFromFile("Imagenes/fondo.png");//fondo del juego, la pulperia
	cursor.loadFromFile("Imagenes/cursor.png");//reticulo del jugador
	impacto.loadFromFile("Imagenes/impacto.png");//impacto del disparo del jugador
	punto.loadFromFile("Imagenes/0.png");//contador de puntos
	amigo.loadFromFile("Imagenes/0.png");//contador de amigos muertos
	enemigo.loadFromFile("Imagenes/0.png");//contador de enemigos muertos
	disparo.loadFromFile("Imagenes/disparo.png");//impacto del disparo del enemigo
	sfondo.setTexture(fondo);
	scursor.setTexture(cursor);
	simpacto.setTexture(impacto);
	spunto.setTexture(punto);
	samigo.setTexture(amigo);
	senemigo.setTexture(enemigo);
	sdisparo.setTexture(disparo);
	
	/*creo la ventana*/
	sf::RenderWindow App(sf::VideoMode(600, 562, 32), "El Pistolero de TDVJ");
	App.clear();//limpio

	/*posiciono los sprites esenciales*/
	scursor.setPosition(340, 240);
	spunto.setPosition(101, 466);
	senemigo.setPosition(532, 466);
	samigo.setPosition(532, 510);
	sdisparo.setPosition(200, 215);

	App.setMouseCursorVisible(false);//oculto el raton
	
	while (App.isOpen()) {//inicio la aplicacion
		Persona *personaje= new Persona(elijePersonaje());
		personaje->mostrar();//muestro el personaje e inicio el tiempo de visibilidad
		tiempo.restart();//inicializo el tiempo de visibilidad del personaje
		control = 0;

		while (personaje->verOculto()) {			
			App.draw(sfondo);//cargo el fondo primero
			App.draw(personaje->ver_sprite());
			App.draw(scursor);//dibujo
			App.draw(spunto);
			App.draw(samigo);
			App.draw(senemigo);
			if (tiempo.getElapsedTime().asSeconds() > 2) { personaje->setOculto(); }
			if (tiempo.getElapsedTime().asSeconds() > 1) {//solucion poco elegante al problema de querer comprar un timer con un int
				if (personaje->esMalo()) {
					App.draw(sdisparo);
					control++;
				}
			}
			if (control==1) {//dado el problema del contador, utilizo este metodo para sumar solo 1 punto
				vidas++;
			}
			App.display();//muestro

			while (App.pollEvent(evt)) {//capturo evento	
				switch (evt.type) {//Proceso los eventos
				case sf::Event::Closed://Controlo si se cerró la ventana
					App.close();//cierro aplicacion
					break;
				case sf::Event::MouseMoved:
					scursor.setPosition(evt.mouseMove.x, evt.mouseMove.y);//seteo nueva posicion
				case sf::Event::MouseButtonPressed:
					if (evt.mouseButton.button == sf::Mouse::Left) {
						sf::Vector2i windowPosition = sf::Mouse::getPosition(App);
						simpacto.setPosition(windowPosition.x, windowPosition.y);//tomo el lugar del impacto
						App.draw(simpacto);//dibujo el impacto
						App.display();//muestro
						if (personaje->impactoDisparo(windowPosition.x, windowPosition.y)) {//controlo si el impacto fue sobre el personaje
							if (personaje->esMalo()) {//valido que tipo de personaje recibio el disparo
								puntos++;//incremento si acerto a un enemigo
								enemigos++;
							}
							else {
								if (puntos > 0) { puntos--; }//resto puntaje si disparo a un inocente
								vidas++;
							}
						personaje->ocultar();//no elimino el objeto aun porque lo necesito, por eso lo oculto
						App.draw(personaje->ver_sprite());
						personaje->setOculto();
						}
					}
				}
			}
			
			/*coloco los respectivos numeros en el score del juego*/
			punto.loadFromFile("Imagenes/" + std::to_string(puntos) + ".png");
			enemigo.loadFromFile("Imagenes/" + std::to_string(enemigos) + ".png");
			amigo.loadFromFile("Imagenes/" + std::to_string(vidas) + ".png");
		}
		App.draw(spunto);
		App.draw(samigo);
		App.draw(senemigo);
		
		delete personaje;//elimino el objeto personaje

		App.display();//muestro
		if (calculaScore(vidas, enemigos) != 0) {//calculo el tanteador e ingreso para ver que tipo de resultado se obtuvo
			if (calculaScore(vidas, enemigos) == 1) {
				cartel.loadFromFile("Imagenes/perdedor.png");//Mensaje de perdedor
			}
			else if (calculaScore(vidas, enemigos) == 2) {
				cartel.loadFromFile("Imagenes/ganador.png");//Mensaje de ganador
			}
			scartel.setTexture(cartel);
			scartel.setPosition(198, 10);
			App.draw(scartel);
			App.display();
			while (App.isOpen()) {//repito el while para poder quedar a la espera del cierre del juego
				while (App.pollEvent(evt)) {//capturo evento	
					switch (evt.type) {//Proceso los eventos
					case sf::Event::Closed://Controlo si se cerró la ventana
						App.close();//cierro aplicacion
						break;
					}
				}
			}
		}
	}
};
void Juego::play() {
	cargarFondo();
};
int Juego::elijePersonaje() {
/*Metodo para seleccionar un personaje al azar*/
	int pers;

	do {
		srand(time(NULL));
		pers = rand() % 11;
	} while (pers==anterior);//pregunto si el personaje es el anterior
	anterior = pers;//asigno el valor para que sea historico
	return pers;
};
int Juego::calculaScore(int z1, int z2) {
/*calculo si mate mas de 3 inocentes o si llegue a 10 puntos*/

	if (z1 == 3) {//controlo las vidas restantes
		return 1;
	}
	if (z2 == 10) {//controlo 10 aciertos en los disparos
		return 2;
	}
	return 0;
};

void Juego::presentacion() {
/*Pantalla de presentacion y reglas del juego*/
	Texture fondo;
	Sprite sfondo;

	fondo.loadFromFile("Imagenes/presentacion.png");//fondo de presentacion
	sfondo.setTexture(fondo);

	sf::RenderWindow App(sf::VideoMode(600, 562, 32), "El Pistolero de TDVJ");
	App.clear();//limpio
	App.draw(sfondo);
	App.display();
	while (App.isOpen()) {//inicio la aplicacion
		while (App.pollEvent(evt)) {//capturo evento	
			switch (evt.type) {//Proceso los eventos
			case sf::Event::Closed://Controlo si se cerró la ventana
				App.close();//cierro aplicacion
				break;
			case sf::Event::MouseButtonPressed:
				if (evt.mouseButton.button == sf::Mouse::Left) {
					App.close();//cierro aplicacion
					break;
				}
			}
		}
	}
};
