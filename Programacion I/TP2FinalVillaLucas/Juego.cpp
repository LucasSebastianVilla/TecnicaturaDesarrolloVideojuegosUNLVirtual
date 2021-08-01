#include "Afichmation.h"
#include <SFML/Graphics.hpp>
#include "ImageManager.h"
#include "Personaje.h"
#include "Tortuguita.h"
#include "Juego.h"

Juego::Juego() {//constructor del juego
	/**Confiruracion de la ventana de juego y directorio de recursos**/
	Ventana = new RenderWindow(VideoMode(800, 600), "Marito 2 TDVJ");
	Ventana->setFramerateLimit(60);
	img_mgr.addResourceDirectory("Recursos/");//acá pongo la ruta donde se alojan los recursos gráficos	
	
	/**Preparo el contador de tiempo**/
	cronometro.restart();//inicio el tiempo de juego
	fuente.loadFromFile("Recursos/MarioBros2.ttf");
	reloj.setFont(fuente);//Configuro la fuente a usar
	reloj.setCharacterSize(20);//Tamaño del caracter en pixeles
	reloj.setColor(sf::Color::Blue);//Configuro en color del reloj
	reloj.setPosition(570,550);//posiciono el reloj en la parte inferior para que interfiera a los graficos

	
	/** preparo los sonidos del juego **/
	if (!sbgameover.loadFromFile("Recursos/mbgameover.wav")) { cout << "No se pudo cargar el sonido" << endl; }
	gameover.setBuffer(sbgameover);
	if (!sbfondometal.loadFromFile("Recursos/supermariobros.ogg")) { cout << "No se pudo cargar el sonido" << endl; }
	fondometal.setBuffer(sbfondometal);
	if (!sbgolpea.loadFromFile("Recursos/mbbump.wav")) { cout << "No se pudo cargar el sonido" << endl; }
	golpea.setBuffer(sbgolpea);
	
	controlCola = new Clock;
	
	controlPila = new Clock; 

	controlDesplazamiento = new Clock;

	Fin = false;

	pila_ladoizquierdo = false;
	sentido = 1;

	fondotex = new Texture;
	fondo = new Sprite;

	puerta_tex = new Texture;
	puerta = new Sprite;

	puerta_tex->loadFromFile("Recursos/salida.png");
	puerta->setTexture(*puerta_tex);
	puerta->setPosition(385, 22);
	puerta->setScale(1.1, 1.1);

	fondotex->loadFromFile("Recursos/fondo_plataformas.png");
	fondo->setTexture(*fondotex);

	/**preparo las texturas del personaje**/
	Texture *pers = new Texture(img_mgr.getImage("Personaje.png"));
	personaje = new Personaje(*pers);

	/**Inicializo los pisos con las listas de cada tortuga**/
	int pisoX=620;//variable para configurar la ubicacion de los sprites
	for(int i=0; i<4;i++){
		ListaTort1[i] = new Tortuguita({ pisoX+30, 420 }, i);
		ListaTort2[i] = new Tortuguita({ pisoX+30, 345 }, i);
		ListaTort3[i] = new Tortuguita({ pisoX+30, 270 }, i);
		ListaTort4[i] = new Tortuguita({ pisoX+30, 195 }, i);
		ListaTort5[i] = new Tortuguita({ pisoX+30, 120 }, i);
		ListaTort6[i] = new Tortuguita({ pisoX+30, 45 }, i);
		pisoX+=30;//desplazo la ubicacion de los sprites
	}

	primeroCola1 = NULL;
	ultimoCola1 = NULL;
	primeroCola3 = NULL;
	ultimoCola3 = NULL;
	primeroCola5 = NULL;
	ultimoCola5 = NULL;

	pila1 = NULL;
	pila2 = NULL;
	pila3 = NULL;
	pila4 = NULL;
	pila5 = NULL;
	pila6 = NULL;


	for(int i = 0; i < 4; i++) {//insertar Tortuguitas en listas
		meteCola(primeroCola1,ultimoCola1,ListaTort1[i],1);
		meteCola(primeroCola3, ultimoCola3, ListaTort3[i],3);
		meteCola(primeroCola5, ultimoCola5, ListaTort5[i],5);
		metePila(pila2, ListaTort2[i],2);
		metePila(pila4, ListaTort4[i],2);
		metePila(pila6, ListaTort6[i],2);
	}
	
	/** comienzo los sonidos **/
	fondometal.setVolume(15.0f);//al ser el sonido ambiente su volumen es bajo
	fondometal.setLoop(true);//a pesar de que el juego dura menos igual configuro que se repita
	fondometal.play();//inicio la melodia
	
	Bucle();//llamo al bucle del juego
}

void Juego::Bucle() {
	while (Ventana->isOpen()) {//mientras la ventana esta abierta se puede jugar
		dibujar();
		ProcesarColision();//Proceso la colision con las tortugas
		ProcesarEventos();//Proceso los eventos del personaje
		if (!Fin) {//si no es el fin sigo jugando y moviendo los actores
			personaje->Actualizar();
			ProcesarCola();//proceso los cambios en la cola de tortugas
			ProcesarPila();//proceso los cambios en la pila de tortugas
			ordenarPila();	
			CuentaRegresiva();//temporizador en reversa del tiempo de juego
			personaje->ControlarDesplazamiento();
			for (int j = 0; j < 4; j++) {//movimiento de las colas
				if (ListaTort1[j]->getSaleLista()) {ListaTort1[j]->movimiento(1);}
				if (ListaTort3[j]->getSaleLista()) {ListaTort3[j]->movimiento(1);}
				if (ListaTort5[j]->getSaleLista()) {ListaTort5[j]->movimiento(1);}
				if (sentido == 1) {//pilas para la izquierda
					if (ListaTort2[j]->getSaleLista()) {ListaTort2[j]->movimiento(2);}
					if (ListaTort4[j]->getSaleLista()) {ListaTort4[j]->movimiento(2);}
					if (ListaTort6[j]->getSaleLista()) {ListaTort6[j]->movimiento(2);}
				}
				if (sentido == 2) {//pilas para la derecha
					if (ListaTort2[j]->getSaleLista()) {ListaTort2[j]->movimiento(3);}
					if (ListaTort4[j]->getSaleLista()) {ListaTort4[j]->movimiento(3);}
					if (ListaTort6[j]->getSaleLista()) {ListaTort6[j]->movimiento(3);}
				}
			}
			
		}
	}

}

void Juego::dibujar() {
	Ventana->clear();//Limpio la pantala
	Ventana->draw(*fondo);//dibujo el fondo de la ventana
	Ventana->draw(*puerta);//dibujo la puerta de salida
	personaje->Dibujar(Ventana);
	for (int j = 0; j < 4; j++) {//dibujo cada lista de cada piso
		ListaTort1[j]->dibujar(Ventana);
		ListaTort2[j]->dibujar(Ventana);
		ListaTort3[j]->dibujar(Ventana);
		ListaTort4[j]->dibujar(Ventana);
		ListaTort5[j]->dibujar(Ventana);
		ListaTort6[j]->dibujar(Ventana);
	}
	if(Fin){FinJuego();	}//condicion de fin de juego, si entra procesa el tipo de final
	Ventana->draw(reloj);//dibuja el contador de tiempo
	Ventana->display();
}

void Juego::ProcesarEventos() {
	//Proceso los eventos de teclado y raton 
	while (Ventana->pollEvent(*event)) {
		if (event->key.code == Keyboard::R) { Ventana->close(); new Juego; };//Reinicio el Juego
		if (event->type == sf::Event::Closed || event->key.code == Keyboard::Escape) {Ventana->close();}
		personaje->ControlarSalto(event);
	}
}

void Juego::CuentaRegresiva() {
	//Funcion que controla el cambio del reloj que controla el tiempo de juego
	tiempo= cronometro.getElapsedTime().asSeconds();//obtiene el tiempo que llevamos jugando
	reloj.setString("Tiempo: " + std::to_string(tiempototal - tiempo));//se resta el tiempo de juego al tiempo total
	if ((tiempototal-tiempo) >= 0){Fin=false;}//verifico el tiempo restante y condiciono la continuidad del juego
	else {Fin=true;pierde=true;}//se termina el juego
}

void Juego::ProcesarColision() {
	//Proceso la colision con la puerta o con las tortugas
	if (puerta->getGlobalBounds().intersects(personaje->GetGlobalBounds())) {//si jugador colisiona con puerta, gana
		Fin = true;//se termina el juego
	}
	for (int i = 0; i < 4; i++) {
		if (personaje->GetGlobalBounds().intersects(ListaTort1[i]->GetGlobalBounds())|| 
			personaje->GetGlobalBounds().intersects(ListaTort2[i]->GetGlobalBounds())|| 
			personaje->GetGlobalBounds().intersects(ListaTort3[i]->GetGlobalBounds())|| 
			personaje->GetGlobalBounds().intersects(ListaTort4[i]->GetGlobalBounds())|| 
			personaje->GetGlobalBounds().intersects(ListaTort5[i]->GetGlobalBounds())||
			personaje->GetGlobalBounds().intersects(ListaTort6[i]->GetGlobalBounds())) {
			
			personaje->setColision();
			personaje->setPosicion(400,495);
		}
	}
}

void Juego::FinJuego(){
	//Metodo de fin de juego, controlo el tipo de mensaje a mostrar
	String condicion;
	fondometal.stop();
	gameover.setVolume(15.0f);
	gameover.play();
	/**Preparo la fuente con todos sus atributos**/
	reloj.setColor(sf::Color::Yellow);//Configuro en color correcto
	reloj.setPosition(60,90);
	reloj.setCharacterSize(50);
	if(!pierde){condicion="¡¡Ganador!!";}
	else{condicion="¡¡Perdedor!!";}
	reloj.setString(condicion);
	Ventana->draw(reloj);
}

void Juego::meteCola(nodo *&primeroCola, nodo *&ultimoCola, Tortuguita *tortuguin,int auxiliar) {
	//Creo un nuevo nodo dentro de la cola
	nodo *nuevonodo = new nodo;
	nuevonodo->TortuguitaSpr = tortuguin;
	nuevonodo->siguiente = NULL;
	if (colaVacia(primeroCola,auxiliar)) {primeroCola = nuevonodo;}
	else {ultimoCola->siguiente = nuevonodo;}
	ultimoCola = nuevonodo;
}

bool Juego::colaVacia(nodo *frente, int auxiliar) {
	//Funcion para consultar si una cola esta vacia
	switch (auxiliar) {
	case 1:
		if (primeroCola1 == NULL) { return true; }
		else { return false; };
		break;
	case 3:
		if (primeroCola3 == NULL) { return true; }
		else { return false; };
		break;
	case 5:
		if (primeroCola5 == NULL) { return true; }
		else { return false; };
		break;
	}
}

void Juego::sacaCola(nodo *&primeroCola, nodo *&ultimoCola, Tortuguita *tortuguita,int auxiliar) {
	//extraigo el nodo de la cola que quiero desplazar y despues lo paso al metodo que lo mueve
	tortuguita=primeroCola->TortuguitaSpr;
	nodo *temp = primeroCola;
	if (primeroCola == ultimoCola) {//si aun no tengo nodos en la cola la marco como vacia
		primeroCola = NULL;
		ultimoCola = NULL;
	}
	else {
		primeroCola = primeroCola->siguiente;//si tengo nodo lo enlazo
	}
	mueveTortuguitas(auxiliar);//muevo la tortuguita
	tortuguita->sacarLista = true;//saco la tortuga de la cola
	delete temp;
}
void Juego::mueveTortuguitas(int auxiliar) {//esta funcion va moviendo una posicion los Tortuguitas en ProcesarCola
	//Metodo para mover las tortugas de la cola
	switch (auxiliar) {
	case 1:
		for (int k = 0; k < 4; k++) {ListaTort1[k]->desplazTortug();}
		break;
	case 3:
		for (int k = 0; k < 4; k++) {ListaTort3[k]->desplazTortug();}
		break;
	case 5:
		for (int k = 0; k < 4; k++) {ListaTort5[k]->desplazTortug();}
		break;
	}
}
void Juego::ProcesarCola() {
	//Metodo para procesar la cola, donde hace el bucle de sacar de un lado y meter en el otro
	if (colacontador == 3) {//si llego al final de la cola, vuelvo a cero para volver a sacar
		colacontador = 0;
	}
	if (controlCola->getElapsedTime().asSeconds() > 1.0f) {//despues de 2 segundos, muevo los sprites
		sacaCola(primeroCola1, ultimoCola1, ListaTort1[colacontador],1);
		sacaCola(primeroCola3, ultimoCola3, ListaTort3[colacontador],3);
		sacaCola(primeroCola5, ultimoCola5, ListaTort5[colacontador],5);

		meteCola(primeroCola1, ultimoCola1, ListaTort1[colacontador],1);
		meteCola(primeroCola3, ultimoCola3, ListaTort3[colacontador],3);
		meteCola(primeroCola5, ultimoCola5, ListaTort5[colacontador],5);

		colacontador++;
		controlCola->restart();//reinicio el contador de tiempo
	}
 }

void Juego::metePila(nodo *&pila2, Tortuguita *tortuguita,int sentido) {
	//Metodo push, meto el nodo en la pila
	nodo *nuevo_nodo = new nodo;
	nuevo_nodo->TortuguitaSpr = tortuguita;
	nuevo_nodo->siguiente = pila2;
	tortuguita->sacarLista = false;
	pila2 = nuevo_nodo;
}

void Juego::sacaPila(nodo *&pila2, Tortuguita *tortuguita,int sentido) {
	//Metodo pop, saco el nodo de la pila y lo elimino
	nodo *aux = pila2;
	aux->TortuguitaSpr = tortuguita;
	pila2 = aux->siguiente;
	tortuguita->sacarLista = true;
	delete aux;
}

void Juego::ProcesarPila() {//controlo el desplazamiento entre pilas
	//si el desplazamiento supero los 10 segundos pregunto si las pilas estan vacias
	if (controlDesplazamiento->getElapsedTime().asSeconds() > 6) {
		if (pila2 == NULL){
			sentido = 2;//con la pila vacia cambio de sentido para volver a repetir el proceso
			controlDesplazamiento->restart();//reinicio el tiempo de desplazamiento
		}
		if (pila1 == NULL){
			sentido = 1;//con la pila vacia cambio de sentido para volver a repetir el proceso
			controlDesplazamiento->restart();//reinicio el tiempo de desplazamiento
		}
	}

	if (sentido==1) {
		pilacontador2 = 3;
		if (controlPila->getElapsedTime().asSeconds() > 1.0f) {
			if (pila2 != NULL) {//para los controles solo uso una pila ya que todas se comportan iguales
				sacaPila(pila2, ListaTort2[pilacontador1], 1);//saco los nodos de encima de cada pila
				sacaPila(pila4, ListaTort4[pilacontador1], 1);
				sacaPila(pila6, ListaTort6[pilacontador1], 1);
				pilacontador1++;
				controlPila->restart();
			}
		}
	}
	if (sentido==2) {
		pilacontador1 = 0;
		if (pila1 != NULL) {//para los controles solo uso una pila ya que todas se comportan iguales
			if (controlPila->getElapsedTime().asSeconds() > 1.0f) {
				sacaPila(pila1, ListaTort2[pilacontador2], 1);
				sacaPila(pila3, ListaTort4[pilacontador2], 1);
				sacaPila(pila5, ListaTort6[pilacontador2], 1);
				pilacontador2--;
				controlPila->restart();
			}
		}
	}
}

void Juego::ordenarPila() {
	for (int j = 0; j < 4; j++) {
		if (sentido == 1) {
			//controlo que la tortuga este en el lado correcto y le doy el sentido de desplazamiento
			if (ListaTort2[0]->Tope() <= 8 && pila1 == NULL) {//detengo en limite izquierdo
				metePila(pila1, ListaTort2[0], 1);
				metePila(pila3, ListaTort4[0], 1);
				metePila(pila5, ListaTort6[0], 1);
			}
			if (ListaTort2[j]->Tope() < 400) {
				if (ListaTort2[j]->DesplazamientoTort(ListaTort2[j - 1], 1)) {
					metePila(pila1, ListaTort2[j], 1);
					metePila(pila3, ListaTort4[j], 1);
					metePila(pila5, ListaTort6[j], 1);
				}
			}
		}
		if (sentido == 2) {
			//controlo que la tortuga este en el lado correcto y le doy el sentido de desplazamiento
			if (ListaTort2[3]->Tope() >= 740 && pila2 == NULL) {
				metePila(pila2, ListaTort2[3], 2);
				metePila(pila4, ListaTort4[3], 2);
				metePila(pila6, ListaTort6[3], 2);
			}
			if (ListaTort2[j]->Tope() > 400) {
				if (ListaTort2[j]->DesplazamientoTort(ListaTort2[j + 1], 2)) {
					metePila(pila2, ListaTort2[j], 2);
					metePila(pila4, ListaTort4[j], 2);
					metePila(pila6, ListaTort6[j], 2);
				}
			}
		}
	}
}
	



