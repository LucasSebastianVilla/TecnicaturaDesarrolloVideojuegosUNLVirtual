#pragma once
#include <SFML\Graphics.hpp>

using namespace sf;

class Vagon {
private:
	Texture *txVagon;
	Sprite *spVagon;

	Font fuente;
	Text nroVagon;
	bool VagonUnido = false;
	bool NroVisible = false;
	bool VagonEliminado = false;
public:
	Vagon(Vector2i pos){
	/** Constructor **/
		txVagon = new Texture;
		spVagon = new Sprite;
		
		txVagon->loadFromFile("Recursos/vagon.png");
		spVagon->setTexture(*txVagon);
		spVagon->setScale(0.7f, 0.5f);
		spVagon->setPosition(pos.x,pos.y);
		
		fuente.loadFromFile("Recursos/nighttrain315.ttf");
		nroVagon.setFont(fuente);
		nroVagon.setCharacterSize(35);
		nroVagon.setFillColor(Color::Cyan);
	};
	void dibujar(RenderWindow *Ventana){
	/** Funcion que controla si esta eliminado o no para dibujarlo **/
		asignaNroVagon();//asigno el nro al vagon
		if (!VagonEliminado) {//pregunto si esta eliminado y no lo dibujo
			Ventana->draw(*spVagon);
			if (NroVisible) {//si ya tiene nro lo voy dibujando con el vagon
				nroVagon.setPosition(spVagon->getPosition().x + 40, spVagon->getPosition().y - 5);
				Ventana->draw(nroVagon);
			}
		}else{
			spVagon->setPosition(2000,2000);//saco el vagon del area de juego
		}
	};
	void MueveTren(Vector2f pos){
	/** Muevo el tren con todos sus vagones **/
		if (VagonUnido) {
			spVagon->setPosition(pos.x - (90*PosicionVagon), pos.y);//marco la posicion de cada vagon
		}
	};
	void asignaNroVagon(){
	/** Metodo que asigna un nro al vagon **/
		if (NroEnVagon==0){//pregunto si el valor es cero para mostrar una X
			nroVagon.setString("X");
		}else{//En caso de que tenga valor diferente a cero lo coloco
			nroVagon.setString(std::to_string(NroEnVagon));	
		}
		NroVisible = true;//configuro para que se pueda mostrar en pantalla
	};

	void asignaVagonEliminado(bool Quitado) { VagonEliminado = Quitado; };//Lo configuro para que se vea o no
	bool Colisiona(FloatRect tencito){
	/** Controla la colision con el tren **/
		if (spVagon->getGlobalBounds().intersects(tencito)) {return true;}
		else {return false;}
	};
	void obtenerVagonUnido(bool Unido) { VagonUnido = Unido; };//verifico si esta enlazado con los otros vagones
	FloatRect dev_bound() { return spVagon->getGlobalBounds(); };//detecto la colision
	void asignaPosicionVagon(int Posicion) { PosicionVagon = Posicion; };//posiciono al vagon
	int NroEnVagon = 0;//numero inicial de los vagones parados
	int PosicionVagon;
};
