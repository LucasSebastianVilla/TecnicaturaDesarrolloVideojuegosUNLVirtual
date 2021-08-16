#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "SFMLRenderer.h"
#include <list>
#include "Actor.h"

using namespace sf;
class Ragdoll
{
public:
	Ragdoll(Vector2f posicion, b2World  & mundo1);
	void Dibujar(RenderWindow& ventana);
	void aplicar_fuerza(Vector2f posicion_m);
	float rad2deg(float radianes);
	void ColoreaRectangleShape(RectangleShape * figRag, int color);

private:
	RectangleShape* fig_rag[6];
	Actor* act_rag[6];
	int color_rs;//variable para el color del rectangleshape

	b2Body* bdy_rag[6];
	b2BodyDef bdydef_rag[6];
	b2Fixture* fix_rag[6];
	b2FixtureDef fixdef_rag[6];

	//Resortes
	b2DistanceJoint* jnt_rag[5];
	b2DistanceJointDef jntdef_rag[5];

};

