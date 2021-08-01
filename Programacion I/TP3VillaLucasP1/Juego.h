#pragma once
#include "Tren.h"
#include "Vagon.h"
#include "nodo.h"
#include <SFML\Graphics.hpp>
#include <iostream>
#include <string>
#include <SFML/Audio.hpp>

using namespace sf;
using namespace std;

class Juego {
private:
	RenderWindow * Ventana;
	Event evt;
	nodo *ListaTren = NULL;

	int Operadores[3];//numeros aleatorios 
	int SumaUsuario = 0;//resultado ingresado por el jugador
	int PosicionVagon = 0;//cuenta los vagones que tiene el tren
	int estado;
	float instalarVagon;
	bool IngresarText = false;//se muestra el textbox y permite al usuario el input del teclado
	bool pierde = false;//valida si el Juego termino y el usuario perdio
	Clock tiempo; //control de tiempo para las ventanas emergentes
	int Suma = 0;//resultado correcto de los tres Operadores sumados
	
	Sound sFondoOzzy;//sonidos para demarcar cada evento en el juego
	sf::SoundBuffer sbFondoOzzy;//buffer para los sonidos
	
	Tren *spTren;//genero un objeto de tipo tren
	Vagon *vagon[6];//genero seis objetos de tipo vagon
	Texture *txFondo, *txPopUp;
	Sprite *spFondo, *spPopUp;
	Font fuente;
	String strIngresarText, strNumeros;
	Text textIngresar, textNumeros, Cronometro, Final;

public:
	Juego();
	void dibujar();
	void Bucle();
	void ProcesarEventos();
	void PonerVagon(nodo *&list,Vagon *vag,int VagValor);
	void SacarVagon(nodo *&list, Vagon *vag);
	void MuestraPopUp();
	void FinJuego();
	void calculo(Vagon *vag);
};
