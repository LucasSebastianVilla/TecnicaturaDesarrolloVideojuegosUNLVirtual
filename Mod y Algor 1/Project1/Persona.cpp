#include "Persona.h"
Persona::~Persona() {};

Persona::Persona(int personaje) {
/*Al crear el objeto ya defino que tipo de personaje es y todas sus particularidades*/

	switch (personaje) {
	case 1:
		pers.loadFromFile("Imagenes/malo01.png");
		spers.setTexture(pers);
		x = 55;
		y = 56;
		x1 = 84;
		y1 = 74;
		x2 = 178;
		y2 = 168;
		tipo = 'M';
		
		break;
	case 2:
		pers.loadFromFile("Imagenes/malo01.png");
		spers.setTexture(pers);
		x = 392;
		y = 56;
		x1 = 420;
		y1 = 74;
		x2 = 514;
		y2 = 168;
		tipo = 'M';
		break;
	case 3:
		pers.loadFromFile("Imagenes/malo02.png");
		spers.setTexture(pers);
		x = 30;
		y = 242;
		x1 = 58;
		y1 = 262;	
		x2 = 152;
		y2 = 375;
		tipo = 'M';
		break;
	case 4:
		pers.loadFromFile("Imagenes/malo03.png");
		spers.setTexture(pers);
		x = 253;
		y = 248;
		x1 = 253;
		y1 = 244;
		x2 = 347;
		y2 = 412;
		tipo = 'M';
		break;
	case 5:
		pers.loadFromFile("Imagenes/malo02.png");
		spers.setTexture(pers);
		x = 411;
		y = 242;
		x1 = 439;
		y1 = 261;
		x2 = 534;
		y2 = 375;
		tipo = 'M';
		break;
	case 6:
		pers.loadFromFile("Imagenes/bueno02.png");
		spers.setTexture(pers);
		x = 55;
		y = 56;
		x1 = 84;
		y1 = 74;
		x2 = 178;
		y2 = 168;
		tipo = 'I';
		break;
	case 7:
		pers.loadFromFile("Imagenes/bueno02.png");
		spers.setTexture(pers);
		x = 392;
		y = 56;
		x1 = 420;
		y1 = 74;
		x2 = 514;
		y2 = 168;
		tipo = 'I';
		break;
	case 8:
		pers.loadFromFile("Imagenes/bueno03.png");
		spers.setTexture(pers);
		x = 30;
		y = 242;
		x1 = 58;
		y1 = 262;
		x2 = 152;
		y2 = 375;
		tipo = 'I';
		break;
	case 9:
		pers.loadFromFile("Imagenes/bueno01.png");
		spers.setTexture(pers);
		x = 253;
		y = 248;
		x1 = 253;
		y1 = 244;
		x2 = 347;
		y2 = 412;
		tipo = 'I';
		break;
	case 10:
		pers.loadFromFile("Imagenes/bueno03.png");
		spers.setTexture(pers);
		x = 411;
		y = 242;
		x1 = 439;
		y1 = 262;
		x2 = 534;
		y2 = 375;
		tipo = 'I';
		break;
	}
};
Persona::Persona() {};

void Persona::mostrar() {
	spers.setPosition(x, y);
	tiempo.restart();
};
void Persona::ocultar() {
	spers.setPosition(1000, 1000);
	//Persona::~Persona();
};
Sprite Persona::ver_sprite() {
	return spers;
};
bool Persona::impactoDisparo(float a, float b) {
/*Controlo si el jugador presiono el boton en el area del personaje*/
	if ((a > x1) && (b > y1)) {
		if ((a < x2) && (b < y2)) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
};
bool Persona::verOculto() {//determino si es visible o no
	return oculto;
};
void Persona::setOculto() {//determino si es visible o no
	oculto = !oculto;
};

bool Persona::esMalo() {
	if (tipo == 'M') { return true; }
	else { return false; }
};