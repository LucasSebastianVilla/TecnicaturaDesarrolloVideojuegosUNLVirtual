#include "Game.h"
#include "SFML/Graphics.hpp"
#include <SFML/Window.hpp>
#include <iostream>
#include <SFML/Audio.hpp>
#include <Windows.h>


using namespace sf;
/*agregar a las naves un indice para que salgan o no, asi las hago pasar de un lado para otro cuando 
llegan al final de la pantalla. acelerando cada vez mas */


Game::Game()
{
	Presentacion();//presento el juego y sus reglas

	_pVentana = new RenderWindow(VideoMode(1000, 700), "Space TDVJ");
	float dv1;
	/*cargo los personajes y los posiciono aleatoriamente*/
	for (int i = 1; i < 10; i++) {
		_pPersonaje[i] = new BaseObject("imagenes/personaje" + std::to_string(i) + ".png");
		
		if (i < 4) {_pPersonaje[i]->SetTipo('A');}//de 1 a 3 los personajes son Amigos, de 4 a 9 son Enemigos
		else {_pPersonaje[i]->SetTipo('E');}
		
		//posicion
		int j = 0;
		do {
			j++;
			if(i!=1 || i!=j){
				dv = Aleatorio(400, 0);
				dv1 = Aleatorio(900, 0);
				_pPersonaje[i]->SetPosition(Vector2f(dv1, dv));
			}
			else {
				break;
			}
		} while ((Colision(_pPersonaje[i], _pPersonaje[j])));

		//aceleracion
		dv = Aleatorio(100, 50);
		_pPersonaje[i]->SetAcceleration(Vector2f(dv, 0));

		//velocidad
		dv = Aleatorio(200, 100);
		_pPersonaje[i]->SetVelocity(Vector2f(dv, 0));
	}
	/*preparo el fondo del juego*/
	if (!fondo.loadFromFile("imagenes/fondo.png")) { cout << "No se pudo cargar la imagen" << endl;}
	sfondo.setTexture(fondo);
	
	/*preparo la reticula*/
	if (!reticula.loadFromFile("imagenes/crosshair.png")) { cout << "No se pudo cargar la imagen" << endl; }
	sreticula.setTexture(reticula);
	_pVentana->setMouseCursorVisible(false);//oculto el raton

	/*preparo los contadores y su fuente*/
	if (!fuente.loadFromFile("recursos/space age.ttf")) { cout << "No se pudo cargar la fuente" << endl; }
	contador.setFont(fuente);
	contador.setPosition(65, 570);
	contador.setColor(sf::Color(100, 255, 100));
	contador.setCharacterSize(30);
	contadorA.setFont(fuente);
	contadorA.setPosition(65, 600);
	contadorA.setColor(sf::Color(100, 255, 100));
	contadorA.setCharacterSize(30);
	mensaje.setFont(fuente);
	mensaje.setPosition(200, 250);
	mensaje1.setColor(sf::Color(200, 100, 70));
	mensaje1.setFont(fuente);
	mensaje1.setPosition(285, 300);
	mensaje1.setCharacterSize(70);
	contadorM.setFont(fuente);
	contadorM.setPosition(65, 630);
	contadorM.setColor(sf::Color(100, 255, 100));
	contadorM.setCharacterSize(30);
	contador.setString("NAVES DESTRUIDAS: 0");
	contadorA.setString("ALIADOS MUERTOS: 0");
	contadorM.setString("DISPAROS RESTANTES: 7");

	/*preparo los sonidos del juego*/
	if (!sbdisparo.loadFromFile("sonidos/disparo.wav")) { cout << "No se pudo cargar el sonido" << endl; }
	disparo.setBuffer(sbdisparo);
	if (!sbexplosion.loadFromFile("sonidos/explosion.wav")) { cout << "No se pudo cargar el sonido" << endl; }
	explosion.setBuffer(sbexplosion);
	if (!sbmfondo.loadFromFile("sonidos/plumbum-rain.ogg")) { cout << "No se pudo cargar el sonido" << endl; }
	mfondo.setBuffer(sbmfondo);
	if (!sbnaves.loadFromFile("sonidos/naves.wav")) { cout << "No se pudo cargar el sonido" << endl; }
	naves.setBuffer(sbnaves);
	if (!sbinicio.loadFromFile("sonidos/inicio.wav")) { cout << "No se pudo cargar el sonido" << endl; }
	inicio.setBuffer(sbinicio);
	if (!sbloser.loadFromFile("sonidos/loser.wav")) { cout << "No se pudo cargar el sonido" << endl; }
	loser.setBuffer(sbloser);
	if (!sbwinner.loadFromFile("sonidos/winner.wav")) { cout << "No se pudo cargar el sonido" << endl; }
	winner.setBuffer(sbwinner);
	
	inicio.play();
	naves.setVolume(45.0f);
	naves.setLoop(true);
	naves.play();
	mfondo.play();
	}

int Game::Aleatorio(int _NroLimite, int _NroNegativo) {
	/*Metodo para conseguir un nro aleatorio entre un rango predeterminado
	se adiciona la posibilidad de obtener nros negativos.*/
	int aleat;
	
	for (int j = 1; j == 100; j++) { srand(time(NULL)); };//genero un delay para que el srand pueda generar diferentes nros
	aleat = (rand() % _NroLimite ) - _NroNegativo;

	return aleat;
}

Game::~Game(){
	/*for (int i = 1; i < 10; i++) {
		delete _pPersonaje[i];
	};*/
	delete _pVentana;
}

void Game::Loop(){
	Clock clock;
	clock.restart();
	float dt;
	while (_pVentana->isOpen()){
		float dt = clock.restart().asSeconds();
		_ProcessEvents();
		_Update(dt);
		_Draw();
		if (1 == _calculaCondicion()) {
			_Draw();
			_pVentana->setMouseCursorVisible(true);//muestro el raton
			if (MessageBoxA(0, "¿Desea jugar nuevamente?", "Final del juego", MB_YESNO | MB_ICONINFORMATION) == IDNO){
				setOtravez();
				break;
			}
			else {
				break;
			}
		}
	}
}

void Game::_Draw(){
	_pVentana->clear();
	_pVentana->draw(sfondo);
	for (int i = 1; i < 10; i++) { _pPersonaje[i]->Draw(_pVentana); }
	_pVentana->draw(sreticula);
	_pVentana->draw(contador);
	_pVentana->draw(contadorA);
	_pVentana->draw(contadorM);
	_pVentana->draw(mensaje);
	_pVentana->draw(mensaje1);
	_pVentana->display();
}

void Game::_Update(float dt){
	float dv1;
	for (int i = 1; i < 10; i++) {
		dv = Aleatorio(50, 25);;
		_pPersonaje[i]->AddAcceleration(Vector2f(dv, 0));
	}
	/*Chequeamos si llegamos a los limites de la pantalla de cualquiera de los personajes*/
	for (int i = 1; i < 10; i++) {
		if (!_pPersonaje[i]->GetOculto()) {//controlo si la nave ya fue destruida para no mostrarla
			dv = Aleatorio(200, 100);;
			_pPersonaje[i]->AddAcceleration(Vector2f(dv, 0));
			if (_pPersonaje[i]->GetPosition().x > 866.0f) {
				_pPersonaje[i]->AddAcceleration(Vector2f(30.0f, 0));
				_pPersonaje[i]->AddVelocity(Vector2f(50.0f, 0));
				dv = Aleatorio(400, 0);
				_pPersonaje[i]->SetPosition(Vector2f(0.0f, dv));
			}
			else if (_pPersonaje[i]->GetPosition().x < 0.0f) {
				_pPersonaje[i]->AddAcceleration(Vector2f(-30.0f, 0));
				_pPersonaje[i]->AddVelocity(Vector2f(-50.0f, 0));
				dv = Aleatorio(400, 0);
				_pPersonaje[i]->SetPosition(Vector2f(860.0f, dv));
			}
		}
	}
	for (int i = 1; i < 10; i++) { _pPersonaje[i]->Update(dt); }
}

void Game::_ProcessEvents() {
	Event evt;
	while (_pVentana->pollEvent(evt)) {
		switch (evt.type) {//Proceso los eventos
		case sf::Event::Closed://Controlo si se cerró la ventana
			_pVentana->close();
			break;
		case sf::Event::MouseMoved:
			sreticula.setPosition(evt.mouseMove.x, evt.mouseMove.y);//configuro la nueva posicion
		case sf::Event::MouseButtonPressed:
			if (evt.mouseButton.button == sf::Mouse::Left) {
				disparo.play();
				sf::Vector2i windowPosition = sf::Mouse::getPosition(*_pVentana);
				municion--;
				contadorM.setString("DISPAROS RESTANTES: " + std::to_string(municion	));			
				//dibujo el disparo
				for (int i = 1; i < 10; i++) {//chequeo que colisione con algun personaje
					if ((windowPosition.x >= _pPersonaje[i]->GetPosition().x) && (windowPosition.y >= _pPersonaje[i]->GetPosition().y) && //calculo si el click del raton cae en el area del enemigo
						(windowPosition.x <= _pPersonaje[i]->GetPosition().x + 134) && (windowPosition.y <= _pPersonaje[i]->GetPosition().y + 134)) {
						//desaparezco el personaje si hay colision
						if (_pPersonaje[i]->GetTipo() == 'E') {
							explosion.play();
							puntaje++;
							contador.setString("NAVES DESTRUIDAS: " + std::to_string(puntaje));
							_pPersonaje[i]->SetOculto(true);
							_pPersonaje[i]->SetPosition(Vector2f(1500, 1500));//muevo la nave de lugar para que no se vea
						}
						else if (_pPersonaje[i]->GetTipo() == 'A') {
							puntajeA++;
							contadorA.setString("ALIADOS MUERTOS: " + std::to_string(puntajeA));
							_pPersonaje[i]->SetOculto(true);
							_pPersonaje[i]->SetPosition(Vector2f(1500, 1500));//muevo al et a un lugar para que no se vea
						}
					}
				}
				//emito el sonido

			}
		}
	}
}
bool Game::Colision(BaseObject *pers1, BaseObject *pers2) {
	/*Metodo para controlar si los personajes chocan entre si*/
	if ((pers1->GetPosition().x > pers2->GetPosition().x - 134) && (pers1->GetPosition().x < pers2->GetPosition().x + 134) &&
		(pers1->GetPosition().y > pers2->GetPosition().y - 134) && (pers1->GetPosition().y < pers2->GetPosition().y + 134)) {
		return true;
	}
	else {
		return false;
	}
}
void Game::Presentacion() {
	/*Pantalla de presentacion y reglas del juego*/
	Texture present;
	Sprite spresent;
	Event evt;

	if (!present.loadFromFile("imagenes/presentacion.png")) { cout << "No se pudo cargar la imagen" << endl; }
	spresent.setTexture(present);

	sf::RenderWindow _pVentana(sf::VideoMode(1000, 700, 32), "Space TDVJ");
	_pVentana.clear();//limpio
	_pVentana.draw(spresent);
	_pVentana.display();
	while (_pVentana.isOpen()) {//inicio la aplicacion
		while (_pVentana.pollEvent(evt)) {//capturo evento	
			switch (evt.type) {//Proceso los eventos
			case sf::Event::Closed://Controlo si se cerró la ventana
				_pVentana.close();//cierro aplicacion
				break;
			case sf::Event::MouseButtonPressed:
				if (evt.mouseButton.button == sf::Mouse::Left) {
					_pVentana.close();//cierro aplicacion
					break;
				}
			}
		}
	}
}
void Game::setOtravez() {
	/*metodo para controlar si se juega otra vez*/
	_otravez = !_otravez;
}
bool Game::getOtravez() {
	/*metodo para controlar si se juega otra vez*/
	return _otravez;
}

int Game::_calculaCondicion() {
	/*metodo que determina la condicion del juego, ganador, perdedor o seguir jugando*/
	mensaje.setColor(sf::Color(255, 50, 10));
	mensaje.setCharacterSize(50);
	if (puntaje == 6 && (municion == 0)) {
		mensaje.setString("JUEGO TERMINADO");
		mensaje1.setString("GANADOR");
		naves.stop();
		mfondo.stop();
		winner.play();
		return 1;
	}
	else if ((puntajeA == 3) || (municion == 0)) {
		mensaje.setString("JUEGO TERMINADO");
		mensaje1.setString("PERDEDOR");
		naves.stop();
		mfondo.stop();
		loser.play();
	return 1;
	}
	return 0;
}