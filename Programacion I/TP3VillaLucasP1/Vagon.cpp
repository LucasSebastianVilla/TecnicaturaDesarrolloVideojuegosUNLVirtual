#include "Vagon.h"

Vagon::Vagon(Vector2i pos) {
/** Constructor **/
	NroEnVagon = 0;
	VagonUnido = false;
	NroVisible = false;
	VagonEliminado = false;
	
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
}
void Vagon::dibujar(RenderWindow *Ventana) {
/** Funcion que controla si esta eliminado o no para dibujarlo **/
	if (!VagonEliminado) {
		Ventana->draw(*spVagon);
		if (NroVisible) {//si ya tiene nro lo voy dibujando con el vagon
			nroVagon.setPosition(spVagon->getPosition().x + 40, spVagon->getPosition().y - 5);
			Ventana->draw(nroVagon);
		}
	}
}

void Vagon::MueveTren(Vector2f pos) {
/** Muevo el tren con todos sus vagones **/
	if (VagonUnido) {
		spVagon->setPosition(pos.x - (90*PosicionVagon), pos.y);
	}
}
bool Vagon::Colisiona(FloatRect tencito) {
/** Controla la colision con el tren **/
	if (spVagon->getGlobalBounds().intersects(tencito)) {return true;}
	else {return false;}
}

void Vagon::asignaNroVagon() {
/** Metodo que asigna un nro al vagon **/
	if (NroEnVagon==0){//pregunto si el valor es cero para mostrar una X
		nroVagon.setString("X");
	}else{//En caso de que tenga valor diferente a cero lo coloco
		nroVagon.setString(std::to_string(NroEnVagon));	
	}
	NroVisible = true;//configuro para que se pueda mostrar en pantalla
}
