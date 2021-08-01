#pragma once
#include <SFML\Graphics.hpp>

using namespace sf;

class Tortuguita {
private:
	int tortuga;//Indica el color de la tortuga
	
	Texture *TortuguitaTex;
	Sprite *TortuguitaSpr[4];
	
public:
	Tortuguita(Vector2f ubicacion, int color){
		tortuga = color;
		for (int i=0;i<4;i++){//cargo los sprites en un arreglo
			TortuguitaTex = new Texture;
			TortuguitaSpr[i] = new Sprite;
			TortuguitaTex->loadFromFile("Recursos/shell_"+ std::to_string(tortuga) +".png");
			TortuguitaSpr[i]->setTexture(*TortuguitaTex);
		}
		TortuguitaSpr[tortuga]->setPosition(ubicacion.x,ubicacion.y);
	};
	void dibujar(RenderWindow *Ventana){Ventana->draw(*TortuguitaSpr[tortuga]);};
	void movimiento(int dir){
		switch (dir) {
		case 1://izquierda
			if (TortuguitaSpr[tortuga]->getPosition().x < 630) {
				TortuguitaSpr[tortuga]->setPosition(TortuguitaSpr[tortuga]->getPosition().x - 6, TortuguitaSpr[tortuga]->getPosition().y);
			}
			if (TortuguitaSpr[tortuga]->getPosition().x < -20 ) {
				TortuguitaSpr[tortuga]->setPosition(740, TortuguitaSpr[tortuga]->getPosition().y);
			}
			break;
		case 2://izquierda
			if (TortuguitaSpr[tortuga]->getPosition().x <= 800 && TortuguitaSpr[tortuga]->getPosition().x >= 3) {
				TortuguitaSpr[tortuga]->setPosition(TortuguitaSpr[tortuga]->getPosition().x - 6, TortuguitaSpr[tortuga]->getPosition().y);
			}
			break;
		case 3://derecha
			if (TortuguitaSpr[tortuga]->getPosition().x <= 740 && TortuguitaSpr[tortuga]->getPosition().x >= 3) {
				TortuguitaSpr[tortuga]->setPosition(TortuguitaSpr[tortuga]->getPosition().x + 6, TortuguitaSpr[tortuga]->getPosition().y);
				if (TortuguitaSpr[tortuga]->getPosition().x >= 740) {
					TortuguitaSpr[tortuga]->setPosition(740, TortuguitaSpr[tortuga]->getPosition().y);
				}
			}
			break;
		}
	};
	
	FloatRect GetGlobalBounds() { return TortuguitaSpr[tortuga]->getGlobalBounds(); };
	
	int Tope() { return TortuguitaSpr[tortuga]->getPosition().x; };//obtengo la posicion en X del sprite
	bool getSaleLista() { return sacarLista; };
	bool sacarLista;
	
	void desplazTortug() { //Muevo la tortuga constantemente solo para un sentido
		TortuguitaSpr[tortuga]->setPosition(TortuguitaSpr[tortuga]->getPosition().x - 32, TortuguitaSpr[tortuga]->getPosition().y); 
	};

	bool DesplazamientoTort(Tortuguita *tor,int sentido){//desplazo las tortugas segun el indice que indica para que lado
		switch (sentido) {
		case 1:
			if (TortuguitaSpr[tortuga]->getGlobalBounds().intersects(tor->GetGlobalBounds())) {
				TortuguitaSpr[tortuga]->setPosition(tor->Tope() + 38, TortuguitaSpr[tortuga]->getPosition().y);
				return true;
			}
			break;
		case 2:
			if (TortuguitaSpr[tortuga]->getGlobalBounds().intersects(tor->GetGlobalBounds())) {
				TortuguitaSpr[tortuga]->setPosition(tor->Tope() - 38, TortuguitaSpr[tortuga]->getPosition().y);
				return true;
			}
			break;
		}
	};
};
