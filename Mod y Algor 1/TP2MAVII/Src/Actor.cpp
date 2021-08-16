#include "Actor.h"

Actor::Actor(b2Body* cuerpo, RectangleShape* figura) : bdy_actor(cuerpo), fig_actor(figura) {
	posicion = bdy_actor->GetPosition();

	//bdy_actor->SetUserData((void*)this);
	
	dimensiones.upperBound = b2Vec2(-FLT_MAX, -FLT_MAX);
	dimensiones.lowerBound = b2Vec2(FLT_MAX, FLT_MAX);

	for (b2Fixture* f = bdy_actor->GetFixtureList(); f; f = f->GetNext()) {
		dimensiones = f->GetAABB(0);
	}
	fig_actor->setSize({ dimensiones.GetExtents().x * 2, dimensiones.GetExtents().y * 2 });
	fig_actor->setOrigin(fig_actor->getSize().x / 2.f, fig_actor->	getSize().y / 2.f);
	fig_actor->setPosition(posicion.x, posicion.y);
	fig_actor->setRotation(rad2deg(bdy_actor->GetAngle()));
}

Actor::~Actor(void){
	//delete &bdy_actor;
}

float Actor::rad2deg(float radianes){
	return radianes * 180 / 3.14f;
}

void Actor::dibujar(RenderWindow &ventana){
	posicion = bdy_actor->GetPosition();
	fig_actor->setPosition(posicion.x, posicion.y);
	fig_actor->setRotation(rad2deg(bdy_actor->GetAngle()));
	ventana.draw(*fig_actor);
}

void Actor::setIdentificador(Identificador id){
	type_act = id;
}

Identificador Actor::get_act(){
	return type_act;
}
