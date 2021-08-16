#pragma once
#include <SFML/Graphics.hpp>
#include "box2D/box2D.h"

using namespace sf;	

enum Identificador { piso, canion, caja_m, caja_f };

class Actor {
public:
	Actor(b2Body* cuerpo, RectangleShape* figura);
	~Actor(void);
	float rad2deg(float radianes);
	void dibujar(RenderWindow &ventana);
	void setIdentificador(Identificador id);
	Identificador get_act();
private:
	b2Body* bdy_actor;
	RectangleShape * fig_actor;
	b2AABB dimensiones;
	b2Vec2 posicion;
	Identificador type_act;
};