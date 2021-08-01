#include "Juego.h"
#include <iostream>

using namespace std;

Juego::Juego() {
	//creo la ventana donde se desarrolla el Juego
	Ventana = new RenderWindow(VideoMode(800, 600), "Crazy Train");
	Ventana->setFramerateLimit(30);

	//cargo los recursos del fondo del Juego
	txFondo = new Texture;
	spFondo = new Sprite;
	txFondo->loadFromFile("Recursos/fondo.jpg");
	spFondo->setTexture(*txFondo);
	spFondo->setScale((float)Ventana->getSize().x / txFondo->getSize().x,(float)Ventana->getSize().y / txFondo->getSize().y);

	//Posiciono cada vagon aleatoriamente en un nivel, los dos primeros estan fijos a la locomotora
	vagon[0] = new Vagon({10, 70});
	vagon[1] = new Vagon({10, 185});
	vagon[2] = new Vagon({rand() % 300 + 300, 70 });//acoto el valor para que no toque la salida
	vagon[3] = new Vagon({rand() % 650 + 100, 187});
	vagon[4] = new Vagon({rand() % 650 + 100, 296});
	vagon[5] = new Vagon({rand() % 650 + 100, 415});
	vagon[6] = new Vagon({rand() % 400 + 50, 534});//acoto el valor para que no toque la meta
	spTren = new Tren;

	//Uno los dos primeros vagones en el tren
	PonerVagon(ListaTren, vagon[0],rand() % 27 + 1);//genero aleatoriamente los numeros. El tope es 27 porque es el max a lograr
	PonerVagon(ListaTren, vagon[1],rand() % 27 + 1);
	
	//cargo los recursos de la ventana emergente
	txPopUp = new Texture;
	spPopUp = new Sprite;
	txPopUp->loadFromFile("Recursos/PopUp.png");
	spPopUp->setTexture(*txPopUp);
	spPopUp->setOrigin(txPopUp->getSize().x / 2, txPopUp->getSize().y / 2);//lo centro en la pantalla
	spPopUp->setPosition(Ventana->getSize().x / 2, Ventana->getSize().y / 2);
	spPopUp->setScale(0.5f, 0.5f);//escalo la imagen
	fuente.loadFromFile("Recursos/nighttrain315.ttf");
	
	//fuente del texto a ingresar
	textIngresar.setFont(fuente);
	textIngresar.setPosition(spPopUp->getPosition().x+50,spPopUp->getPosition().y);//posiciono el texto a ingresar dentro del pop up
	textIngresar.setCharacterSize(25);
	textIngresar.setFillColor(Color::Cyan);
	
	//fuente de los Operadores de la suma
	textNumeros.setFont(fuente);
	textNumeros.setPosition(spPopUp->getPosition().x - 60, spPopUp->getPosition().y);
	textNumeros.setCharacterSize(25);
	textNumeros.setFillColor(Color::Blue);
	
	//fuente del mensaje de fin de Juego
	Final.setFont(fuente);
	Final.setColor(sf::Color::Cyan);
	Final.setPosition(320,10);
	Final.setCharacterSize(50);
	
	//Cronometro de tiempo de Juego **/
	Cronometro.setFont(fuente);
	Cronometro.setFillColor(Color::Green);
	Cronometro.setPosition(610, 10);
	Cronometro.setCharacterSize(25);

	//Cargo el sonido de fondo del Juego
	if (!sbFondoOzzy.loadFromFile("Recursos/ozzyosbournecrazytrain.wav")) { cout << "No se pudo cargar el sonido" << endl; }
	sFondoOzzy.setBuffer(sbFondoOzzy);
	sFondoOzzy.setVolume(10.0f);//al ser el sonido ambiente su volumen es bajo
	sFondoOzzy.setLoop(true);//a pesar de que el juego dura menos igual configuro que se repita
	sFondoOzzy.play();//inicio la melodia \m/ Ozzy \m/
	
	Bucle();
}

void Juego::dibujar() {
/** Funcion que dibuja los actores del Juego y mensajes **/
	Ventana->clear();
	Ventana->draw(*spFondo);
	spTren->dibujar(Ventana);
	for (int i = 0; i < 7; i++) {//cargo todos los vagones
		vagon[i]->dibujar(Ventana);
	}
	if (IngresarText) {//Presento el popup donde va la ecuacion
		Ventana->draw(*spPopUp);//sprite del popup
		Ventana->draw(textNumeros);//texto de la ecuacion
		Ventana->draw(textIngresar);//texto del resultado
		Ventana->draw(Cronometro);//tiempo para responder
	}
	FinJuego();
	Ventana->display();
}

void Juego::FinJuego() {
/** Funcion de fin de Juego **/
	string condicion;
	if (pierde) {
		switch (estado) {
		case 0:
			condicion="¡¡Perdedor!!";
			break;
		case 1:
			condicion="¡¡Ganador!!";
			break;
		}
		Final.setString(condicion);
		Ventana->draw(Final);
	}
}

void Juego::Bucle() {
/** Bucle principal del Juego **/
	while (Ventana->isOpen()){//mientras la ventana esta abierta el Juego sigue
		ProcesarEventos();//Proceso los eventos de teclado
		dibujar();//dibujo todo lo referente al juego
		if (!pierde) {//si aun no termino el juego sigo con el bucle
			if (spTren->moverTren()) {//Muevo el tren
				spTren->Desplazamiento();//llamo a la funcion que dibuja el camino del tren y su posicion
				instalarVagon = spTren->obtenerPosicion().x;//obtengo la posicion del tren, luego la uso para los vagones
			}
			if (spTren->obtenerLlegada()) {//Pregunto si llego a la meta y termino
				estado = 1;//Marco que gano
				pierde = true;//cambio la variable para que no ingrese mas 
			}
			for (int i = 2; i < 7; i++) {//Controlo si el tren colisiona con algun vagon parado
				if (vagon[i]->Colisiona(spTren->GetGlobalBounds())) {
					spTren->posicionarTren(false);//dejo de MueveTren el tren
					calculo(vagon[i]);//Llamo al cronometro y al popup para realizar la suma
				}
			}
			for (int i = 0; i < 7; i++) {//de acuerdo a la posicion del tren muevo los vagones por detras
				vagon[i]->MueveTren({instalarVagon,spTren->obtenerPosicion().y });
			}
		}
	}
}

void Juego::ProcesarEventos() {
/** Proceso los eventos del teclado **/
	if (Ventana->pollEvent(evt)) {//Controlo los eventos de teclado
		switch (evt.type) {
		case Event::Closed:
			Ventana->close();//Cierro la ventana
		case Event::KeyPressed:
			if (evt.key.code == Keyboard::Escape) { Ventana->close(); };//Si presiono escape se cierra el juego
			if (evt.key.code == Keyboard::R) { Ventana->close(); new Juego(); };//Si presiono R se reinicia
		case Event::TextEntered://Preparo para ingresar por teclado
			if (IngresarText) {//Valido que este activa la opcion de ingreso
				if (evt.text.unicode >= 48 && evt.text.unicode <= 57) {//Solo tomo el rango numerico
					strIngresarText += evt.text.unicode;//ingreso el texto
					textIngresar.setString(strIngresarText);//lo cargo en la variable para mostrar
					string num = strIngresarText;
					SumaUsuario = stoi(num);//convierto el string a numero y lo guardo en la variable del usuario
				}
			}
		}
	}
}

void Juego::PonerVagon(nodo *&ListaVagones, Vagon *spVagon,int VagValor) {
/** Funcion que inserta un vagon dentro de la Lista segun su orden **/
	nodo *NuevoVagon = new nodo();//creo un nuevo nodo vagon
	spVagon->NroEnVagon = VagValor;//asigno el valor de la suma al nuevo vagon
	NuevoVagon->vagon = spVagon;
	nodo *aux = ListaVagones;//creo un duplicado auxiliar de la lista
	nodo *aux2= NULL;//creo un puntero nulo
	int Temporal;//variable para el intercambio de valores entre los vagones
	while ((aux != NULL)) {//verifico que la lista no este vacia
		if (aux->vagon->NroEnVagon > NuevoVagon->vagon->NroEnVagon) {//Pregunto si el nuevo vagon es mas chico que el consultado
			int Temporal = aux->vagon->NroEnVagon;//guardo el valor del vagon
			aux->vagon->NroEnVagon = NuevoVagon->vagon->NroEnVagon;//asigno el valor del nuevo vagon al anterior
			NuevoVagon->vagon->NroEnVagon = Temporal;//asigno el valor del anterior al nuevo vagon
		}
		aux2 = aux;//duplico la lista actual
		aux = aux->siguiente;//muevo al siguiente nodo
	}
	if (ListaVagones == aux) {ListaVagones = NuevoVagon;}//comparo las listas para agregar el nuevo vagon
	else {aux2->siguiente = NuevoVagon;}
	PosicionVagon++;//controlo cuantos vagones quedan asi puedo dibujarlos
	NuevoVagon->vagon->asignaPosicionVagon(PosicionVagon);//le doy un lugar en la lista
	NuevoVagon->vagon->obtenerVagonUnido(true);//Uno el vagon al resto
	NuevoVagon->siguiente = aux;//tomo el puntero de la lista auxiliar
}

void Juego::SacarVagon(nodo *&ListaVagones, Vagon *spVagon) {
/** Funcion para sacar un vagon del tren, si es el ultimo informa final de Juego **/
	if (ListaVagones != NULL) {//Verifico que la lista no este vacia
		nodo *VagonEliminado;//creo dos nodos para poder borrar el vagon
		nodo *VagonPrevio=NULL;
		VagonEliminado = ListaVagones;//asigno el puntero al vagon a eliminar
		while (VagonEliminado->siguiente != NULL) {//desplazo hasta ubicar el ultimo vagon
			VagonPrevio = VagonEliminado;//posiciono el puntero en el vagon anterior
			VagonEliminado = VagonEliminado->siguiente;//desplazo al vagon siguiente
		}
		if (VagonEliminado == NULL) {//Consulto si no hay mas vagones
			estado = 0;//Marco que perdio
			pierde = true;//Configuro la variable para que no repita el bucle de juego
		}else if (VagonEliminado->siguiente == NULL) {
			if (VagonPrevio != NULL) {//Pregunto si el vagon anterior es nulo
				PosicionVagon--;//resto una posicion vagon
				VagonPrevio->vagon->asignaPosicionVagon(PosicionVagon);//configuro la nueva cantidad
				VagonPrevio->siguiente = VagonEliminado->siguiente;
				VagonEliminado->vagon->asignaPosicionVagon(PosicionVagon);
				VagonEliminado->vagon->obtenerVagonUnido(false);//Suelto el ultimo vagon
				VagonEliminado->vagon->asignaVagonEliminado(true);//oculto el vagon
			}else if (VagonPrevio == NULL) {//Quito el ultimo vagon
				VagonEliminado->vagon->obtenerVagonUnido(false);//Suelto el ultimo vagon
				VagonEliminado->vagon->asignaVagonEliminado(true);//oculto el vagon
				estado = 0;//Marco que perdio
				pierde = true;//Configuro la variable para que no repita el bucle de juego
			}
		}
	}
}

void Juego::MuestraPopUp() {
/** Muestro la ventana emergente con la ecuacion a resolver **/
	if (!IngresarText) {//Pregunto si esta activado para ingresar texto
		for (int i = 0; i < 3; i++) {//cargo los operadores en el arreglo
			Operadores[i] = rand() % 10;//los genero aleatoriamente
			Suma += Operadores[i];//Voy sumando para saber el resultado correcto
		}
		strNumeros = to_string(Operadores[0]) + "+" //preparo el string para mostrar la operacion
			+ to_string(Operadores[1]) + "+" 
			+ to_string(Operadores[2]) + "= ";
		textIngresar.setString("");//Limpio el ingreso de numeros
		textNumeros.setString(strNumeros);//Muestro la operacion a realizar
		IngresarText = true;//Habilito para que escriban
		tiempo.restart();//reinicio el tiempo
	}
}

void Juego::calculo(Vagon *spVagon) {
/** Controlo los vagones y la cuenta regresiva **/
	int Reloj = tiempo.getElapsedTime().asSeconds();//llevo la cuenta regresiva
	Cronometro.setString("Tiempo restante: " + to_string(5 - Reloj));
	MuestraPopUp();//llamo a la funcion PopUp para que procese la operacion aritmetica
	if (tiempo.getElapsedTime().asSeconds() < 5) {
		if (evt.key.code == Keyboard::Return) {//Presionar Enter para realizar la validacion
			if (IngresarText && strIngresarText != "") {//Verifico que se ingrese un resultado
				if (Suma == SumaUsuario) {//si esta bien se agrega el vagon en su lugar
					PonerVagon(ListaTren, spVagon, Suma);//inserto el nuevo vagon en la lista
				}
				else {//si esta mal retiro el ultimo vagon
					SacarVagon(ListaTren, spVagon);
					spVagon->asignaVagonEliminado(true);//configuro para que el vagon no se dibuje mas
				}
				spTren->posicionarTren(true);//vuelvo a permitir que el tren se mueva
				Suma=0;//Vuelvo a cero la suma
				IngresarText = false;//libero para que siga el tren
				strIngresarText = "";//limpio el campo de ingreso de texto
			}
		}
	}
	else {//si llega a cero penalizo y reinicio la cuenta si es que tengo mas vagones
		SacarVagon(ListaTren, spVagon);
		spVagon->asignaVagonEliminado(true);//configuro para que el vagon no se dibuje mas
		spTren->posicionarTren(true);//vuelvo a permitir que el tren se mueva
		IngresarText = false;//libero para que siga el tren
		strIngresarText = "";//limpio el campo de ingreso de texto
		Suma=0;//Vuelvo a cero la suma
	}
	
}




