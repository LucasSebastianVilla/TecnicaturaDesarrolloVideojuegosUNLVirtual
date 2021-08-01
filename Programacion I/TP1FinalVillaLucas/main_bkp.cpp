#include <SFML/Graphics.hpp>
#include "ImageManager.h"
#include "Personaje.h"
#include "Bloque.h"

using namespace std;
using namespace sf;

//Se crea una instancia del objeto imageManager
ImageManager img_mgr;

Personaje *personaje;
Bloque *bloque[10];
sf::Sprite background;
sf::Event* event = new Event();
int posBloque=200; //posicion del bloque inicial

//Se crea la ventana principal del juego
sf::RenderWindow *window = new RenderWindow(sf::VideoMode(800, 600), "Juego");

//DECLARACION DE METODOS
void actualizarSalto();//Verifica y controla las condiciones de salto
void dibujar();//Declaramos metodo que realiza el dibujo en pantalla de todo lo necesario
int Aleatorio(int _NroLimite);//funcion que obtiene un nro entero aleatorio 
void ProcesarColision();//Controlo si el personaje golpea el bloque
void ActualizaJuego();
void ProcesarEventos();

//PUNTO DE ENTRADA
int main(){
	//Se asigna la carpeta relativa donde se alojan nuestras imágenes
	img_mgr.addResourceDirectory("Recursos/imagenes/");

	Texture pers = img_mgr.getImage("spritesheet.png");
	personaje = new Personaje(pers);
	
	Texture bloq = img_mgr.getImage("bloque_pared.png");
	//bloque = new Bloque(bloq, posBloque, Aleatorio(50));
	for (int i = 0; i < 10; i++){//realizo la carga de todos los bloques 
		bloque[i] = new Bloque(bloq, posBloque, Aleatorio(50));
		posBloque += 45;
	}
	
	background.setTexture(img_mgr.getImage("mundo_fondo.jpg"));
	background.setPosition(0, 0);
	
	//Detección de eventos de SFML
    while (window->isOpen()) {
		while (window->pollEvent(*event)) {
			if (event->type == sf::Event::Closed) {
				window->close();
			}
			//La detección de tecla presionada por Evento, solo se justifica para acciones que se ejecutan no tan a menudo.
			//Por ejemplo escuchar una vez, cuando la tecla de salto se presiona, sin importar que quede presionada.
			personaje->ControlarSalto(event);
		}
		//Esta es la mejor manera de detectar teclas presionadas para cuando se necesita un movimiento constante y fluido.
		//Como por ejemplo el movimiento de un personaje.
		personaje->ControlarDesplazamiento();
		ProcesarColision();
		ActualizaJuego();
    }	
    return 0;
}

void dibujar(){
	window->draw(background);
	personaje->Dibujar(window);
	for (int i = 0; i < 10; i++){
		bloque[i]->Dibujar(window);
	}
}
int Aleatorio(int _NroLimite) {
	/*Metodo para conseguir un nro aleatorio entre un rango predeterminado.*/
	int aleat;
	
	for (int j = 1; j == 100; j++) { srand(time(NULL)); };//genero un delay para que el srand pueda generar diferentes nros
	aleat = (rand() % _NroLimite );
	
	return aleat;
}

void ProcesarColision(){
//	FloatRect persBox = personaje->getGlobalBounds();
//	for (int i = 0; i < 10; i++){
//		sf::FloatRect bloqBox = bloque[i]->getGlobalBounds();
//	
//		if persBox.intersects(bloqBox){
//			
//		}
//	}
}

void ProcesarEventos(){

}

void ActualizaJuego(){
	personaje->Actualizar();
	window->clear();//Limpia lo dibujado en ciclo anterior
	dibujar();
	window->display();//Se muestra en ventana,lo nuevo que se ha dibujado con window.draw
}
