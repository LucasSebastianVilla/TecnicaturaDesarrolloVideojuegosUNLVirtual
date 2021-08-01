#include "Afichmation.h"
#include <SFML/Graphics.hpp>
#include "ImageManager.h"
#include "Personaje.h"
#include "Bloque.h"
#include <SFML/Audio.hpp>


using namespace std;
using namespace sf;

class Juego {
private:
	ImageManager img_mgr;//Se crea una instancia del objeto imageManager
	Personaje *personaje;//Personaje del juego
	Bloque *bloque[10];//Objetivo del personaje
	Sprite sfondo;//Fondo de pantalla
	Texture fondo;
	Event* event = new Event();//eventos de teclado
	int posBloque=100; //posicion del bloque inicial
	Text digitos, reloj, mensaje;
	Font fuente;
public:
	Juego();
	RenderWindow *window = new RenderWindow(sf::VideoMode(800, 600), "Marito TDVJ");
	void dibujar();//Declaramos metodo que realiza el dibujo en pantalla de todo lo necesario
	int Aleatorio(int _NroLimite);//funcion que obtiene un nro entero aleatorio 
	void actualizarSalto();//Verifica y controla las condiciones de saltoatorio 
	void ProcesarColision();//Controlo si el personaje golpea el bloque y lo cambio de color 
	void ActualizaJuego();
	void ProcesarEventos();//Proceso los eventes de las teclas
	void Bucle();//Repeticion del juego, mantiene abierta la ventana del mismo
	void ArregloNros();//metodo que coloca los nros sobre los boques
	int arregloBloqNros[10];//arreglo con los nros de los bloques
	int arregloBloqNrosOrd[10];//arreglo con los nros de los bloques ordenados
	void OrdenaArreglo(int arreglo[10], int izquierda, int derecha);//Ordenamiento del arreglo inicial para poder comparar 
	bool restar=false;
	int ObtieneTiempo();//controla el tiempo de juego
	void AsignaTiempo();//asigna el tiempo con sus penalidades
	bool bloquesTotales=false; //variable para obtener la cantidad de bloques restantes
	void FinJuego();//Metodo que se encagra de presentar la finalizacion del juego
	Sound golpea, gameover, fondometal;//sonidos para demarcar cada evento en el juego
	sf::SoundBuffer sbgolpea, sbgameover, sbfondometal;//buffer para los sonidos
	int tiempo;//variable del tiempo que se muestra
	Clock cronometro;//variable que cuenta el tiempo transcurrido
	void CuentaRegresiva();//contador en reversa para terminar el juego
	bool Fin=false;//bandera que indica si el tiempo llega a cero
	int tiempototal=60;
};
