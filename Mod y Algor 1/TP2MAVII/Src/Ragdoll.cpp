#include "Ragdoll.h"
#include "Box2DHelper.h"

Ragdoll::Ragdoll(Vector2f posicion, b2World& mundo1) {
	//Constructor del objeto ragdoll
	//posicion de los poligonos - posicion inicial es 50 en X y 94.1 en Y
	bdydef_rag[0].position = b2Vec2(posicion.x, posicion.y);//cabeza
	bdydef_rag[1].position = b2Vec2(posicion.x, posicion.y + 0.55f);//torso
	bdydef_rag[2].position = b2Vec2(posicion.x + 0.32f, posicion.y + 0.4f);//brazo der
	bdydef_rag[3].position = b2Vec2(posicion.x - 0.32f, posicion.y + 0.4f);//brazo izq
	bdydef_rag[4].position = b2Vec2(posicion.x + 0.09f, posicion.y + 1.4f);//pierna der
	bdydef_rag[5].position = b2Vec2(posicion.x - 0.09f, posicion.y + 1.4f);//pierna izq

	//creo las partes del cuerpo
	for (int i = 0; i < 6; i++) {
		bdydef_rag[i].type = b2_dynamicBody;
		bdy_rag[i] = mundo1.CreateBody(&bdydef_rag[i]);
	}
	//Genero el contenedor del shape de las partes del ragdoll
	b2PolygonShape shp_rag[6];

	//dimension partes del ragdoll
	shp_rag[0].SetAsBox(0.1f, 0.1f);//cabeza
	shp_rag[1].SetAsBox(0.2f, 0.4f);//torso
	shp_rag[2].SetAsBox(0.07f, 0.3f);//brazo der
	shp_rag[3].SetAsBox(0.07f, 0.3f);//brazo izq
	shp_rag[4].SetAsBox(0.07f, 0.4f);//pierna der
	shp_rag[5].SetAsBox(0.07f, 0.4f);//pierna izq

	//configuro el fixture de cada parte del ragdoll
	for (int i = 0; i < 6; i++) {
		fixdef_rag[i].shape = &shp_rag[i];
		fixdef_rag[i].restitution = 0.3f;
		fixdef_rag[i].density = 1.f;
		fixdef_rag[i].friction = 0.3f;
		fix_rag[i] = bdy_rag[i]->CreateFixture(&fixdef_rag[i]);
	}

	//Uno con resortes las diferentes parte del cuerpo
	jntdef_rag[0].Initialize(bdy_rag[0], bdy_rag[1], b2Vec2(bdy_rag[0]->GetPosition().x, bdy_rag[0]->GetPosition().y + 0.08f), //cabeza - torso
		b2Vec2(bdy_rag[1]->GetPosition().x, bdy_rag[1]->GetPosition().y - 0.38f));

	jntdef_rag[1].Initialize(bdy_rag[1], bdy_rag[2], b2Vec2(bdy_rag[1]->GetPosition().x + 0.09f, bdy_rag[1]->GetPosition().y - 0.38f), //brazoder - torso
		b2Vec2(bdy_rag[2]->GetPosition().x, bdy_rag[2]->GetPosition().y - 0.3f));

	jntdef_rag[2].Initialize(bdy_rag[1], bdy_rag[3], b2Vec2(bdy_rag[1]->GetPosition().x - 0.09f, bdy_rag[1]->GetPosition().y - 0.38f), //brazoizq - torso
		b2Vec2(bdy_rag[3]->GetPosition().x, bdy_rag[3]->GetPosition().y - 0.3f));

	jntdef_rag[3].Initialize(bdy_rag[1], bdy_rag[4], b2Vec2(bdy_rag[1]->GetPosition().x + 0.09f, bdy_rag[1]->GetPosition().y + 0.38f), //piernader - torso
		b2Vec2(bdy_rag[4]->GetPosition().x, bdy_rag[4]->GetPosition().y - 0.38f));

	jntdef_rag[4].Initialize(bdy_rag[1], bdy_rag[5], b2Vec2(bdy_rag[1]->GetPosition().x - 0.09f, bdy_rag[1]->GetPosition().y + 0.38f), //piernaizq - torso
		b2Vec2(bdy_rag[5]->GetPosition().x, bdy_rag[5]->GetPosition().y - 0.38f));

	//configuro cada parte del cuerpo del ragdoll
	for (int i = 0; i < 5; i++) {
		jntdef_rag[i].damping = 0.3f;
		jntdef_rag[i].stiffness = 0.9f;
		jntdef_rag[i].length = 0.035f;
		jntdef_rag[i].collideConnected = true;
		jnt_rag[i] = (b2DistanceJoint*)mundo1.CreateJoint(&jntdef_rag[i]);
	}

	//inicializamos imagen
	for (int i = 0; i < 6; i++) {
		fig_rag[i] = new RectangleShape;
	}
	
	//para hacer visualmente mas agradable cambio los colores de las partes del cuerpo
	color_rs = rand() % 7 + 1;
	ColoreaRectangleShape(fig_rag[0], color_rs);
	color_rs = rand() % 7 + 1;
	ColoreaRectangleShape(fig_rag[1], color_rs);
	ColoreaRectangleShape(fig_rag[2], color_rs);
	ColoreaRectangleShape(fig_rag[3], color_rs);
	color_rs = rand() % 7 + 1;
	ColoreaRectangleShape(fig_rag[4], color_rs);
	ColoreaRectangleShape(fig_rag[5], color_rs);

	//inicializo el actor
	for (int i = 0; i < 6; i++) {
		act_rag[i] = new Actor(bdy_rag[i], fig_rag[i]);
	}
}

void Ragdoll::Dibujar(RenderWindow & ventana){
	//dibujo todas las partes del cuerpo del ragdoll
	for (int i = 0; i < 6; i++) {
		act_rag[i]->dibujar(ventana);
	}
}

void Ragdoll::aplicar_fuerza(Vector2f posicion_m){
	//aplico fuerza sobre el ragdoll de acuerdo a la posicion del raton
	bdy_rag[1]->ApplyForceToCenter(b2Vec2(posicion_m.x * 80, posicion_m.y * 80), false);
}
float Ragdoll::rad2deg(float radianes) {
	//convierto radianes a grados
	return radianes * 180 / 3.14f;
}

void Ragdoll::ColoreaRectangleShape(RectangleShape * figRag, int color){
	//funcion que toma una parte del cuerpo y la colorea de acuerdo al nro que se pasa por parametro
	switch (color) {
	case 1:
		figRag->setFillColor(Color::Blue);
		break;
	case 2:
		figRag->setFillColor(Color::Cyan);
		break;
	case 3:
		figRag->setFillColor(Color::Green);
		break;
	case 4:
		figRag->setFillColor(Color::Magenta);
		break;
	case 5:
		figRag->setFillColor(Color::Red);
		break;
	case 6:
		figRag->setFillColor(Color::White);
		break;
	case 7:
		figRag->setFillColor(Color::Yellow);
		break;
	}
}
