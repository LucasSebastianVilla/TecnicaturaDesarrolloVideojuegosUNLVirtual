#include "Afichmation.h"
#include <SFML/Graphics.hpp>
#include "ImageManager.h"
#include "Personaje.h"
#include "Bloque.h"
#include "Juego.h"

using namespace std;


Juego::Juego(){
	cronometro.restart();//inicio el tiempo de juego
	window->setFramerateLimit(60);//Limito los frames por segundo
	img_mgr.addResourceDirectory("Recursos/");//acá pongo la ruta donde se alojan los recursos gráficos	
	
	/** preparo las texturas del personaje y el bloque **/
	Texture *pers = new Texture(img_mgr.getImage("spritesheet.png"));
	personaje = new Personaje(*pers);
	Texture *bloqueTex = new Texture(img_mgr.getImage("bloque_pared.png"));
	
	/**Preparo la fuente con todos sus atributos**/
	if (!fuente.loadFromFile("recursos/MarioBros2.ttf")) { cout << "No se pudo cargar la fuente" << endl; }
	digitos.setFont(fuente);//Configuro la fuente a usar
	digitos.setCharacterSize(15);//Tamaño del caracter en pixeles
	mensaje.setFont(fuente);//Configuro la fuente a usar
	mensaje.setCharacterSize(45);//Tamaño del caracter en pixeles
	
	/** preparo los sonidos del juego **/
	if (!sbgameover.loadFromFile("Recursos/mbgameover.wav")) { cout << "No se pudo cargar el sonido" << endl; }
	gameover.setBuffer(sbgameover);
	if (!sbfondometal.loadFromFile("Recursos/mbfondometal.ogg")) { cout << "No se pudo cargar el sonido" << endl; }
	fondometal.setBuffer(sbfondometal);
	if (!sbgolpea.loadFromFile("Recursos/mbbump.wav")) { cout << "No se pudo cargar el sonido" << endl; }
	golpea.setBuffer(sbgolpea);
	
	for (int i = 0; i < 10; i++){//realizo la carga de todos los bloques con sus atributos particulares
		bloque[i] = new Bloque(bloqueTex, posBloque, Aleatorio(999));
		posBloque += 60;
		arregloBloqNros[i]=bloque[i]->GetNumero();//cargo en el arreglo los nros de los bloques como aparecen
		int variab=bloque[i]->GetNumero();
		arregloBloqNrosOrd[i]=variab;//hago lo mismo pero este arreglo sera ordenado para comparar
	}
	sfondo.setTexture(img_mgr.getImage("mundo_fondo.jpg"));//imagen de fondo
	sfondo.setPosition(0, 0);
	ArregloNros();
	OrdenaArreglo(arregloBloqNrosOrd,0,10);
	
	/** comienzo los sonidos **/
	fondometal.setVolume(3.0f);//al ser el sonido ambiente su volumen es bajo
	fondometal.setLoop(true);//a pesar de que el juego dura menos igual configuro que se repita
	fondometal.play();//inicio la melodia
	
	fuente.loadFromFile("Recursos/MarioBros2.ttf");
	reloj.setFont(fuente);//Configuro la fuente a usar
	reloj.setCharacterSize(20);//Tamaño del caracter en pixeles
	reloj.setColor(sf::Color::Black);//Configuro en color correcto
	reloj.setPosition(550,50);
	reloj.setString("60");
}

void Juego::Bucle(){
	while (window->isOpen()) {
		ArregloNros();
		CuentaRegresiva();//temporizador en reversa del tiempo de juego
		if(!bloquesTotales && !Fin){//consulto si llegue a 10 bloques o a 0 segundos para bloquear los eventos
			personaje->ControlarDesplazamiento();
			ProcesarEventos();
		}
		if(personaje->saltoEnProceso){
			ProcesarColision();
		}
		ActualizaJuego();
	}
}

void Juego::ArregloNros(){//metodo que coloca los nros sobre los boques
	posBloque=101;
	int control=0;
	
	for (int i = 0; i < 10; i++){//recorro los nros de los bloques 
		if (bloque[i]->obtieneColor()){//Configuro en color correcto
			digitos.setColor(sf::Color::Green);
			control++;
			if (control==10){//si llega a 10 bloques verdes el juego debe terminar
				bloquesTotales=true;				
			};
		}else {digitos.setColor(sf::Color::Blue);};//uso color AZUL para una mejor visualizacion
		digitos.setPosition(posBloque,264);
		digitos.setString(std::to_string(bloque[i]->GetNumero()));
		window->draw(digitos);
		posBloque += 60;
	}
}

void Juego::CuentaRegresiva(){//control del tiempo restante de juego
	tiempo= cronometro.getElapsedTime().asSeconds();//obtiene el tiempo que llevamos jugando
	reloj.setString("Tiempo: " + std::to_string(tiempototal - tiempo));//se resta el tiempo de juego al tiempo total
	if ((tiempototal-tiempo) > 0){Fin=false;}//verifico el tiempo restante y condiciono la continuidad del juego
	else {Fin=true;}
}

void Juego::ProcesarEventos(){
	while (window->pollEvent(*event)) {
		if (event->type == sf::Event::Closed) {window->close();}
		personaje->ControlarSalto(event);
	}
}	

void Juego::ActualizaJuego(){	
	personaje->Actualizar();
	window->clear();//Limpia lo dibujado en ciclo anterior
	dibujar();
	window->display();//Se muestra en ventana,lo nuevo que se ha dibujado con window.draw
}
	
void Juego::dibujar(){//dibujo los actores del juego asi como el fondo
	window->draw(sfondo);
	personaje->Dibujar(window);
	for (int i = 0; i < 10; i++){
		bloque[i]->Dibujar(window);
	}
	if (bloquesTotales || Fin){
		fondometal.stop();
		FinJuego();
	}else{
		window->draw(reloj);
		ArregloNros();
	}
}
	
int Juego::Aleatorio(int _NroLimite) {
	/*Metodo para conseguir un nro aleatorio entre un rango predeterminado.*/
	int aleat;
	for (int j = 1; j == 100; j++) { srand(time(NULL)); };//genero un delay para que el srand pueda generar diferentes nros
	aleat = (rand() % _NroLimite );	
	return aleat;
}
		
void Juego::ProcesarColision(){//
	FloatRect persBox = personaje->GetGlobalBounds();//tomo el rectangulo del personaje
	for (int i = 0; i < 10; i++){
		sf::FloatRect bloqBox = bloque[i]->GetGlobalBounds();//tomo el rectangulo del bloque
		if (persBox.intersects(bloqBox)){//veo si hay interseccion entre los cuadros
			if(personaje->EstadoControlColision()){
				personaje->ControlaColision(false);
				for (int h=0;h<10;h++){//comparo el bloque golpeado con el arreglo ordenado para ver si esta bien
					if(arregloBloqNrosOrd[h]==bloque[i]->GetNumero()){//controlo que el nro colisinado sea el del arreglo
						for (int g=0;g<10;g++){
							if ((arregloBloqNrosOrd[h-1]==bloque[g]->GetNumero())&& bloque[g]->obtieneColor()){//pregunto si el anterior ordenado ya fue golpeado
								personaje->setColision(true);
								bloque[i]->asignaColor();//le doy el color verde 
								break;//salgo para no seguir repitiendo y calcule mal en el tiempo restante
							}else if(arregloBloqNrosOrd[0] == bloque[i]->GetNumero()){//pregunto si es el primero del arreglo ordenado
								personaje->setColision(true);
								bloque[i]->asignaColor();//le doy el color verde 
								break;//salgo para no seguir repitiendo y calcule mal en el tiempo restante
							}else {
								personaje->setColision(false);
								golpea.play();
								if (g==9){//controlo que sea el ultimo bloque y resto
									tiempototal=tiempototal-10;
								}
							}
						}
					}
				}
			}
		}
	}
}

void Juego::FinJuego(){
	posBloque=101;
	String condicion;
	gameover.setLoop(true);
	gameover.play();
	/**Preparo la fuente con todos sus atributos**/
	mensaje.setColor(sf::Color::Yellow);//Configuro en color correcto
	mensaje.setPosition(140,160);
	if (bloquesTotales){//si ordeno los bloque gano
		condicion="¡¡Ganador!!";
	}else if(Fin){//si acaba el tiempo pierdo
		condicion="¡¡Perdedor!!";
	}else if(bloquesTotales && Fin){//si acaba el tiempo pero ordene los bloques gano
		condicion="¡¡Ganador!!";
	}
	mensaje.setString(condicion);
	window->draw(mensaje);
	
	for (int i = 0; i < 10; i++){//recorro los nros de los bloques 
		digitos.setColor(sf::Color::Cyan);//Configuro en color correcto
		digitos.setPosition(posBloque,264);
		digitos.setString(std::to_string(arregloBloqNrosOrd[i]));
		window->draw(digitos);
		posBloque += 60;
	}
}



void Juego::OrdenaArreglo(int arreglo[10], int izquierda, int derecha){
	/** Metodo Quicksort **/
	int i = izquierda;//almaceno el límite de la izquierda
	int j = derecha;//y aqui en este caso el de la derecha
	int temporal;//esta variable la vamos a usar luego
	//Así determinamos y recordamos cual es el pivot para dividir la tarea de ordenamiento
	int pivot = arreglo[(izquierda + derecha) / 2];
	
	while (i <= j) {//mientras el valor actual de límite izquierdo sea menor o igual al límite derecho
		while (arreglo[i] < pivot) {//mientras el valor en la posición i sea menor que el pivot que encontramos antes
			i++;//incrementamos el límite izquierdo
		}
		while (arreglo[j] > pivot) {//mientras el valor en la posición j sea mayor que el pivot que encontramos antes
			j--;//decrementamos el valor de j
		}
		//si se sigue cumpliendo la condición inicial
		if (i <= j) {//se realizan los intercambios
			temporal = arreglo[i];
			arreglo[i] = arreglo[j];
			arreglo[j] = temporal;
			
			i++;//nuevamente se acercan los límites
			j--;
		}
	}
	if (izquierda < j) {//Cuando ya i y j han excedido sus límites realizamos comparaciones
		OrdenaArreglo(arreglo, izquierda, j);
	}
	if (i < derecha) {//Análogamente al IF anterior pero mirando el otro lado del arreglo
		OrdenaArreglo(arreglo, i, derecha);
	}
}

