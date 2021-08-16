#include "ActorSprite.h"

ActorSprite::ActorSprite(b2Body* cuerpo, Sprite* figura) : bdy_actor(cuerpo), fig_actor(figura) {
	posicion = bdy_actor->GetPosition();

	dimensiones.upperBound = b2Vec2(-FLT_MAX, -FLT_MAX);
	dimensiones.lowerBound = b2Vec2(FLT_MAX, FLT_MAX);

	for (b2Fixture* f = bdy_actor->GetFixtureList(); f; f = f->GetNext()) {
		dimensiones = f->GetAABB(0);
	}
	fig_actor->setOrigin(fig_actor->getTexture()->getSize().x / 2.f, fig_actor->getTexture()->getSize().y / 2.f);
	fig_actor->setPosition(posicion.x, posicion.y);
	fig_actor->setRotation(rad2deg(bdy_actor->GetAngle()));

	fig_actor->setScale(dimensiones.GetExtents().x * 2.f/ fig_actor->getTexture()->getSize().x, dimensiones.GetExtents().y * 2.f/ fig_actor->getTexture()->getSize().y);
}

float ActorSprite::rad2deg(float radianes) {
	return radianes * 180 / 3.14f;
}

void ActorSprite::dibujar(RenderWindow& ventana) {
	posicion = bdy_actor->GetPosition();
	fig_actor->setPosition(posicion.x, posicion.y);
	fig_actor->setRotation(rad2deg(bdy_actor->GetAngle()));
	ventana.draw(*fig_actor);
}