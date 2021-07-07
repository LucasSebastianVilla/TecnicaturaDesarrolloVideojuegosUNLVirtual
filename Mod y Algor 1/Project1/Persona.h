#pragma once
#include "SFML/Graphics.hpp"
#include <SFML/Window.hpp>
#include <iostream>

using namespace sf;

class Persona
{
private:
	float x, y;//posicion en el juego
	float x1, y1, x2, y2;//variables para definir el area de impacto del personaje
	Sprite spers;
	Texture pers;
	char tipo; //M - malo; I - Inocente
	sf::Clock tiempo;
	bool oculto=true;//estado de visibilidad
public:
	Persona();
	~Persona();
	Sprite ver_sprite();
	bool impactoDisparo(float a, float b);//verifico si el impacto fue sobre el personaje
	Persona(int personaje);//constructor de personajes con parametro
	void mostrar();//muestro el personaje en un lugar de la pantalla
	void ocultar();//oculto el personaje
	bool verOculto();//determino si es visible o no
	void setOculto();//determino si es visible o no
	bool esMalo();//identifica el tipo de personaje
	
}; 

