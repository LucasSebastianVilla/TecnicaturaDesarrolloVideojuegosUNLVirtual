#pragma once
#include <SFML/Graphics.hpp>
#include "box2D/box2D.h"

using namespace sf;

class ActorSprite
{
public:
	ActorSprite(b2Body* cuerpo, Sprite* figura);
	float rad2deg(float radianes);
	void dibujar(RenderWindow& ventana);
private:
	b2Body* bdy_actor;
	Sprite* fig_actor;
	b2AABB dimensiones;
	b2Vec2 posicion;
};

