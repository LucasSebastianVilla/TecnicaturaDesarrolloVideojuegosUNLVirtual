#pragma once
#include "Ragdoll.h"
#include "Box2DHelper.h"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "SFMLRenderer.h"
#include "Box2D\Box2D.h"
#include "Actor.h"


class Colisionador : public b2ContactListener {
public:
	void BeginContact(b2Contact * contacto);// evento que se llama cuando comienza el contacto
	void EndContact(b2Contact* contact); // evento que se llama cuando termina el contacto
	void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);// evento que se llama luego de la detecci�n de colisiones pero antes //de la resoluci�n de las mismas
	void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse); // evento que se llama luego de la resoluci�n de colision y da //informaci�n del impulso resultante de la
};