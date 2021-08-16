#include "Juego.h"
#include "Ragdoll.h"
#include <iostream>


Juego::Juego(Vector2i resolucion, std::string titulo){
	//configuro los fps del juego
	fps = 60;
	tiempoFrame = 1.0f / fps;

	ventana1 = new RenderWindow(VideoMode(resolucion.x, resolucion.y), titulo);
	ventana1->setFramerateLimit(fps);

	fuente.loadFromFile("../Src/EarlyGameBoy.ttf"); //Cargo la fuente del juego

	for (int i = 0; i < 3; i++) {
		txFondo[i] = new Texture;
		spFondo[i] = new Sprite;
		txFondo[i]->loadFromFile("../Src/fondo" + std::to_string(i) + ".png");
		spFondo[i]->setTexture(*txFondo[i]);
		spFondo[i]->setScale(0.0165f, 0.025f);
	}
	spFondo[0]->setPosition(45.4f, 89.f);
	spFondo[1]->setPosition(45.4f, 89.f);
	spFondo[2]->setPosition(100.f, 89.f);
	
	ventana1->setVisible(true);
	
	evento1 = new Event;
	reloj1 = new Clock;
	tiempo1 = new Time;
	
	set_camera({ 19.f,19.f }, { 13.f, 13.f }, { 33.f, 77.5f });//camara sobre juego

	InicioJuego();
}

void Juego::InicioJuego() {
	//Funcion que me sirve para poder reiniciar los parametros del juego cuando paso de nivel
	iniciar_fisica();
	iniciar_img();
	iniciar_textos();

	//elementos del mundo
	act_suelo = new ActorSprite(bdy_suelo, spSuelo);
	act_arma = new ActorSprite(bdy_arma, spArma[0]);
	act_base = new ActorSprite(bdy_base, spArma[1]);
	for (int i = 0; i < 3; i++) {
		act_caja[i] = new ActorSprite(bdy_caja[i], spCajaF[i]);
		act_caja_m[i] = new ActorSprite(bdy_caja_m[i], spCajaM[i]);
	}
	gameLoop();
}

void Juego::set_camera(Vector2f vista1, Vector2f vista2, Vector2f mover1) {
	//vista de la ventana de juego
	camara1 = new View({ vista1 }, { vista2 });
	camara1->move({ mover1 });
	ventana1->setView(*camara1);
}

void Juego::iniciar_fisica(){
	// Inicializamos el mundo con la gravedad por defecto
	mundo1 = new b2World(b2Vec2(0.0f, 9.81f));

	procesar_c = new Colisionador();

	mundo1->SetContactListener(procesar_c);

	//creo e inicializo el suelo
	bdydef_suelo.type = b2_staticBody;
	bdydef_suelo.position = b2Vec2(52.f + desplazamientoX, 102.4f);
	
	bdy_suelo = mundo1->CreateBody(&bdydef_suelo);
	
	b2PolygonShape shp_suelo;
	shp_suelo.SetAsBox(10.f, .7f);
	
	fixdef_suelo.shape = &shp_suelo;
	fixdef_suelo.restitution = 0.f;
	fixdef_suelo.density = 1.f;
	fix_suelo = bdy_suelo->CreateFixture(&fixdef_suelo);

	//creo e inicializo el cañon
	bdydef_arma.type = b2_staticBody;
	bdydef_arma.position = b2Vec2(48.f + desplazamientoX, 100.2f);

	bdy_arma = mundo1->CreateBody(&bdydef_arma);

	b2PolygonShape shp_arma;
	shp_arma.SetAsBox(0.8f, 0.4f);

	fixdef_arma.shape = &shp_arma;
	fixdef_arma.density = 1.f;
	fixdef_arma.restitution = 0.1f;
	fixdef_arma.friction = 0.3f;

	fix_arma = bdy_arma->CreateFixture(&fixdef_arma);

	//creo e inicializo la base del cañon
	bdydef_base.type = b2_staticBody;
	bdydef_base.position = b2Vec2(48.f + desplazamientoX, 100.8f);

	bdy_base = mundo1->CreateBody(&bdydef_base);

	b2PolygonShape shp_base;
	shp_base.SetAsBox(0.3f, 0.9f);

	fixdef_base.shape = &shp_base;
	fixdef_base.density = 1.f;
	fixdef_base.restitution = 0.1f;
	fixdef_base.friction = 0.3f;

	fix_base = bdy_base->CreateFixture(&fixdef_base);

	for (int i = 0; i < 3; i++) {
		//creo e inicializo el obstaculo fijo
		bdydef_caja[i].type = b2_staticBody;
		bdydef_caja[i].position = b2Vec2(52.f + incX + desplazamientoX, 98.f + incY - desplazamientoY);

		bdy_caja[i] = mundo1->CreateBody(&bdydef_caja[i]);

		b2PolygonShape shp_caja;
		shp_caja.SetAsBox(0.4f, 0.4f);

		fixdef_caja[i].shape = &shp_caja;
		fixdef_caja[i].density = 1.f;
		fixdef_caja[i].restitution = 0.1f;
		fixdef_caja[i].friction = 0.3f;

		fix_caja[i] = bdy_caja[i]->CreateFixture(&fixdef_caja[i]);

		//creo e inicializo el obstaculo mobil
		bdydef_caja_m[i].type = b2_dynamicBody;
		bdydef_caja_m[i].position = b2Vec2(51.7f + incX + desplazamientoX, 96.5f + incY - desplazamientoY);

		bdy_caja_m[i] = mundo1->CreateBody(&bdydef_caja_m[i]);

		b2PolygonShape shp_caja_m;
		shp_caja.SetAsBox(0.4f, 0.4f);

		fixdef_caja_m[i].shape = &shp_caja;
		fixdef_caja_m[i].density = 1.f;
		fixdef_caja_m[i].restitution = 0.1f;
		fixdef_caja_m[i].friction = 0.3f;

		fix_caja_m[i] = bdy_caja_m[i]->CreateFixture(&fixdef_caja_m[i]);
		incX += 1.95f;
		if (nivel1 <= 2) {incY -= 1.25f;}
		else {incY -= 1.25f;}
	}
	//Uno con resortes dos cajas
	jntdef_caja.Initialize(bdy_caja[2], bdy_caja_m[2], b2Vec2(bdy_caja[2]->GetPosition().x, bdy_caja_m[0]->GetPosition().y + 1.3f),
		b2Vec2(bdy_caja[2]->GetPosition().x, bdy_caja_m[2]->GetPosition().y + 1.f));

	jntdef_caja.damping = 0.1f;
	jntdef_caja.stiffness = 0.1f;
	jntdef_caja.length = 1.5f;
	jntdef_caja.collideConnected = true;
	jnt_caja = (b2DistanceJoint*)mundo1->CreateJoint(&jntdef_caja);
}

void Juego::iniciar_img() {
	//inicializo las figuras de los actores
	//cargo las sprites del arma
	for (int i = 0; i < 2; i++) {
		txArma[i] = new Texture;
		spArma[i] = new Sprite;
		txArma[i]->loadFromFile("../Src/arma" + std::to_string(i) + ".png");
		spArma[i]->setTexture(*txArma[i]);
	}
	//cargo las sprites de las cajas
	for (int i = 0; i < 3; i++) {
		txCajaF[i] = new Texture;
		spCajaF[i] = new Sprite;
		txCajaF[i]->loadFromFile("../Src/caja_f.png");
		spCajaF[i]->setTexture(*txCajaF[i]);
		txCajaM[i] = new Texture;
		spCajaM[i] = new Sprite;
		txCajaM[i]->loadFromFile("../Src/caja_m.png");
		spCajaM[i]->setTexture(*txCajaM[i]);
	}
	//cargo las sprites del suelo
	txSuelo = new Texture;
	spSuelo = new Sprite;
	txSuelo->loadFromFile("../Src/suelo.png");
	spSuelo->setTexture(*txSuelo);

	fig_suelo = new RectangleShape;//suelo
	fig_arma = new RectangleShape;//cañon
	fig_base = new RectangleShape;//base cañon
	for (int i = 0; i < 3; i++) {
		fig_caja[i] = new RectangleShape;//obstaculo fijo
		fig_caja_m[i] = new RectangleShape;//obstaculo mobil
	}
}

float Juego::deg2rad(float grados){
	//formula para convertir grados a radianes
	return grados * 3.14f / 100;
}

void Juego::gameLoop(){
	//bucle del juego
	while (ventana1->isOpen()){
		*tiempo1 = reloj1->getElapsedTime();
		if (tiempo2 + tiempoFrame < tiempo1->asSeconds()) {
			tiempo2 = tiempo1->asSeconds();
			ventana1->clear();
			procesar_eventos();
			actualizar_fisica();
			CondicionFinal();
			Reloj = contador.getElapsedTime().asSeconds();//genero un pequeño retraso para que se muestre bien el cambio de nivel
			if(Reloj > 4){ CambiaNivel(condicionGameOver); }
			mostrar_textos();
			Dibujar();
			ventana1->display();
		}		
	}
}

void Juego::CambiaNivel(int condicion){
	//Cambio las variables para poder visualizar el segundo nivel -- El desplazamientoX de la camara es de 54.6f
	if (condicionGameOver>=3 && nivel1<2) {
		set_camera({ 19.f,19.f }, { 13.f, 13.f }, { 87.6f, 77.5f });
		condicionGameOver = 0;
		nivel1++;
		desplazamientoX =+ 54.6f;
		desplazamientoY =+ 1.6f;
		incX = 0;
		incY = 0;
		cant_rag = 0;
	
		InicioJuego();
	}
}

void Juego::actualizar_fisica(){
	//creo la fisica del juego
	mundo1->Step(tiempoFrame, 8, 8);
	mundo1->ClearForces();
	mundo1->DebugDraw();
}

void Juego::CondicionFinal() {
	for (int i = 0; i < 3; i++) {
		//Controlo que las cajas hayan caido para ganar, en caso contrario, se pierde
		if (spCajaM[i]->getGlobalBounds().intersects(spCajaF[i]->getGlobalBounds())) {condicionGameOver = 0; }
		else if (spCajaM[i]->getGlobalBounds().intersects(spSuelo->getGlobalBounds())) {condicionGameOver++;}
		else if (!spCajaM[i]->getGlobalBounds().intersects(spCajaF[i]->getGlobalBounds())) { condicionGameOver++; }
	}
	if (condicionGameOver == 3) {
		Reloj = 0;
		contador.restart();//reinicio el tiempo
	}
}

void Juego::Dibujar(){
	//dibujo los actores
	ventana1->clear();
	switch (nivel1)	{
	case 0:
		ventana1->draw(*spFondo[0]);
		for (int i = 0; i < 3; i++) {ventana1->draw(textosJuego[i]);}
		break;
	default:
		if (nivel1 == 1) { ventana1->draw(*spFondo[1]); }
		else if (nivel1 == 2) { ventana1->draw(*spFondo[2]); }
		act_suelo->dibujar(*ventana1);
		ventana1->draw(textCantRag);
		ventana1->draw(textCondicion);
		act_arma->dibujar(*ventana1);
		act_base->dibujar(*ventana1);
		for (int i = 0; i < 3; i++) {
			act_caja[i]->dibujar(*ventana1);
			act_caja_m[i]->dibujar(*ventana1);
		}
		for (int i = 0; i < cant_rag; i++) {
			if (!rag_3[i] || dibujalo) {rag_3[i]->Dibujar(*ventana1);}
		}
		break;
	}
}

void Juego::procesar_eventos() {
	Vector2i posicion_m;//posicion del raton
	Vector2f posicion_mc;//posicion del raton en coordenadas

	while (ventana1->pollEvent(*evento1)) {
		switch (evento1->type) {
		case Event::Closed:
			ventana1->close();
			break;
		case sf::Event::MouseMoved:
			posicion_m = Mouse::getPosition(*ventana1);
			posicion_mc = ventana1->mapPixelToCoords(posicion_m);
			bdy_arma->SetTransform(bdy_arma->GetPosition(), atan2f(posicion_mc.y - bdy_arma->GetPosition().y, posicion_mc.x - bdy_arma->GetPosition().x));//obtengo las coodrdenadas del raton
			break;
		case Event::MouseButtonPressed:
			
			if (cant_rag < 5 && nivel1 >0) {//si disparo 5 ragdolls termina el juego y solo se puede hacer si no esta en el nivel 0
				//obtengo la ubicacion del del raton y la convierto de pixel a coordenadas
				dibujalo = true;//
				posicion_m = Mouse::getPosition(*ventana1);
				posicion_mc = ventana1->mapPixelToCoords(posicion_m);

				if (condicionGameOver < 3){//si tire todas las cajas no permito disparar mas
					rag_3[cant_rag] = new Ragdoll({ bdy_arma->GetPosition().x + 1.f,bdy_arma->GetPosition().y - 2.f }, *mundo1);//configuro la posicion del raton para saber donde disparar
					rag_3[cant_rag]->aplicar_fuerza({ posicion_mc.x - bdy_arma->GetPosition().x,posicion_mc.y - bdy_arma->GetPosition().y });//aplico la fuerza al disparo
					cant_rag++;//incremento para poder condicionar el juego
				}
			}
			break;
		case Event::KeyPressed:
			if (Keyboard::isKeyPressed(Keyboard::Left)) {//bajo la mira (obsoleto, solo se deja para probar)
				bdy_arma->SetTransform(bdy_arma->GetPosition(), bdy_arma->GetAngle() + deg2rad(2));
			}
			else if (Keyboard::isKeyPressed(Keyboard::Right)) {//subo la mira (obsoleto, solo se deja para probar)
				bdy_arma->SetTransform(bdy_arma->GetPosition(), bdy_arma->GetAngle() - deg2rad(2));
			}
			else if (Keyboard::isKeyPressed(Keyboard::Escape)) {//con escape cierro la ventana
				ventana1->close();
				break;
			}
			else if (Keyboard::isKeyPressed(Keyboard::R)) {
				ventana1->close();
				new Juego({ 800,600 }, "TP Final MAVII - Villa Lucas"); //Si presiono R se reinicia
			}
			else if (Keyboard::isKeyPressed(Keyboard::Up)) {//coloreo la opcion correcta
				//opcion que entra al juego
				textosJuego[1].setColor(Color::Yellow);
				textosJuego[2].setColor(Color::White);
				opcion1 = true;
				break;
			}
			else if (Keyboard::isKeyPressed(Keyboard::Down)) {//coloreo la opcion correcta
				//opcion que sale del juego
				textosJuego[1].setColor(Color::White);
				textosJuego[2].setColor(Color::Yellow);
				opcion1 = false;
				break;
			}
			else if (Keyboard::isKeyPressed(Keyboard::Enter) ){
				//opcion que dispara el evento de las teclas anteriores
				if (opcion1) {nivel1 = 1;}
				else {ventana1->close();}
				break;
			}
			break;
		}
	}
}

void Juego::ColoreaRectangleShape(RectangleShape* figRag, int color) {
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

void Juego::mostrar_textos(){
	//funcion para mostrar los diferentes textos del juego 
	Reloj = contador.getElapsedTime().asSeconds();
	textCantRag.setString("Ragdolls Restantes: " + to_string(5 - cant_rag));
	textosJuego[0].setString("CiberRags MAVII");
	textosJuego[1].setString("Jugar");
	textosJuego[2].setString("Salir");
	if (condicionGameOver >=3) {
		if (nivel1 > 1 ) { textCondicion.setString("GANADOR - R menu"); }
	}
	else if (condicionGameOver < 3 && cant_rag == 5){
		textCondicion.setString("PERDEDOR - R menu"); 
	}else{ textCondicion.setString(" "); }
	
}

void Juego::iniciar_textos(){
	//Funcion para preparar las fuentes y poder mostrar mensajes
	textCantRag.setFont(fuente);
	textCantRag.setPosition(46.f + desplazamientoX, 90.5f);
	textCantRag.setScale(0.02f, 0.02f);
	textCantRag.setCharacterSize(20);
	textCantRag.setFillColor(Color::Cyan);

	textCondicion.setFont(fuente);
	textCondicion.setPosition(46.f + desplazamientoX, 91.f);
	textCondicion.setScale(0.02f, 0.02f);
	textCondicion.setCharacterSize(30);
	textCondicion.setFillColor(Color::Yellow);

	textInciar.setFont(fuente);
	textInciar.setPosition(46.f + desplazamientoX, 90.5f);
	textInciar.setScale(0.02f, 0.02f);
	textInciar.setCharacterSize(20);
	textInciar.setFillColor(Color::Cyan);

	textSalir.setFont(fuente);
	textSalir.setPosition(46.f + desplazamientoX, 91.f);
	textSalir.setScale(0.02f, 0.02f);
	textSalir.setCharacterSize(30);
	textSalir.setFillColor(Color::Yellow);

	//0: presentacion - 1: Jugar - 2: Salir - 3: Mensaje de sistema
	for (int i=0; i < 4; i++) {
		textosJuego[i].setFont(fuente);
		textosJuego[i].setScale(0.025f, 0.025f);
		textSalir.setCharacterSize(30);
		textSalir.setFillColor(Color::White);
	}
	textosJuego[0].setPosition(46.7f + desplazamientoX, 92.f);
	textosJuego[1].setPosition(50.f + desplazamientoX, 95.f);
	textosJuego[2].setPosition(50.f + desplazamientoX, 96.f);
	textosJuego[3].setPosition(46.f + desplazamientoX, 92.f);

}
