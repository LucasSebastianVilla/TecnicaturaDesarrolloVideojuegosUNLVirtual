#pragma once
#include "Personaje.h"
#include"Tortuguita.h"
#include "nodo.h"
#include <SFML\Graphics.hpp>
#include <iostream>
#include "ImageManager.h"

using namespace sf;

class Juego {
private:
	/**ya corregidas**/
	RenderWindow * Ventana;
	Event* event = new Event();//eventos de teclado
	ImageManager img_mgr;//Se crea una instancia del objeto imageManager
	Clock cronometro;//variable que cuenta el tiempo transcurrido
	Text reloj;//texto para visualizar el reloj y los mensajes
	
	Personaje *personaje;//Personaje del juego
	Font fuente;//fuente usada en todo el juego
	int tiempo;//variable del tiempo que se muestra
	int tiempototal=30;
	bool Fin=false;//bandera que indica si el tiempo llega a cero
	Sound golpea, gameover, fondometal;//sonidos para demarcar cada evento en el juego
	sf::SoundBuffer sbgolpea, sbgameover, sbfondometal;//buffer para los sonidos
	Clock *controlCola, *controlPila, *controlDesplazamiento; //cronometros para controlar el tiempo de desplazamiento
	
	int colacontador = 0;
	int pilacontador1 = 0, pilacontador2 = 3;

	bool pila_ladoizquierdo;
	bool pierde=false;
	
	int sentido;

	Texture *fondotex;
	Sprite  *fondo;

	Texture *puerta_tex;
	Sprite *puerta;

	nodo *primeroCola1, *ultimoCola1;//punteros al primero y ultimo de la cola
	nodo *primeroCola3, *ultimoCola3;//punteros al primero y ultimo de la cola
	nodo *primeroCola5, *ultimoCola5;//punteros al primero y ultimo de la cola
	nodo *pila1, *pila2, *pila3, *pila4, *pila5, *pila6;//punteros a los topes de las pilas
	Tortuguita *ListaTort1[4], *ListaTort2[4], *ListaTort3[4], *ListaTort4[4], *ListaTort5[4], *ListaTort6[4];//arreglo de sprites

public:
	/**Metodos para procesar el Juego**/
	Juego();
	void Bucle();//Repeticion del juego, mantiene abierta la ventana del mismo
	void dibujar();//dibujar en pantalla
	void ProcesarEventos();
	void ProcesarColision();
	void CuentaRegresiva();//contador en reversa para terminar el juego
	void FinJuego();//procesa el fin del juego
	/**Metodos para procesar COLAS**/
	void meteCola(nodo *&primeroCola, nodo *&ultimoCola, Tortuguita *tortuguin,int auxiliar);
	void sacaCola(nodo *&primeroCola, nodo *&ultimoCola, Tortuguita *tortuguita,int auxiliar);
	bool colaVacia(nodo *frente,int auxiliar);//si la ProcesarCola está vacia
	void mueveTortuguitas(int auxiliar);//da movimiento a la ProcesarCola
	void ProcesarCola();//se encarga de eliminar e insertar Tortuguitas en ProcesarCola	
	/**Metodos para procesar PILAS**/
	void metePila(nodo *&pila2, Tortuguita *tortuguita,int sentido);
	void sacaPila(nodo *&pila2, Tortuguita *tortuguia,int sentido);
	void ordenarPila();//ordenar fila de la ProcesarPila
	void ProcesarPila();
};
