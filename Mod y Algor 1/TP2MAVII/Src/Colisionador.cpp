#include "Colisionador.h"

void Colisionador::BeginContact(b2Contact * contacto){
	b2Fixture* aux1 = contacto->GetFixtureA();
	b2Fixture* aux2 = contacto->GetFixtureB();


	//if (aux1->GetBody()->GetUserData() == "Target")// Se puede comprobar de otras maneras tambien
	//{
	//	// TODO
	//}

	//if (aux2->GetBody()->GetUserData() == "Target")// Se puede comprobar de otras maneras tambien
	//{
	//	// TODO
	//}
}

void Colisionador::EndContact(b2Contact* contact)
{
}

void Colisionador::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
}

void Colisionador::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
}
