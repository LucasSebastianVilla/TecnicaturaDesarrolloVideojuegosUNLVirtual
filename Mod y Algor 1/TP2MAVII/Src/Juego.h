#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Box2D\Box2D.h"
#include <list>
#include "Ragdoll.h"
#include "Colisionador.h"
#include "ActorSprite.h"

using namespace sf;
using namespace std;

class Juego
{
private:
	RenderWindow* ventana1;
	View *camara1;
	Event *evento1;
	Time* tiempo1;
	Font fuente;
	
	Clock
		* reloj1,
		contador;

	Texture
		* txFondo[3],
		* txArma[2],
		* txSuelo,
		* txCajaF[3],
		* txCajaM[3];	

	Sprite
		* spFondo[3],
		* spArma[2],
		* spSuelo,
		* spCajaF[3],
		* spCajaM[3];
	
	Text 
		textosJuego[4], 
		textCantRag, 
		textCondicion, 
		textInciar, 
		textSalir;

	float 
		incX = 0,
		incY = 0,
		fps, 
		tiempoFrame, 
		desplazamientoX=0,//controlo el desplazamiento de la camara para los 2 niveles -- 54.6f
		desplazamientoY = 0,
		tiempo2;// Tiempo de frame

	int 
		alto, 
		ancho,
		cant_rag = 0,//total de ragdolls que permito disparar
		color_rs,//variable para el color del rectangleshape
		nivel1 = 0,//0:menu - 1:nivel 1 - 2:nivel 2
		Reloj = 0, //variable para contar el tiempo de paso de nivel
		condicionGameOver = 0;

	bool
		dibujalo = false, //variable para identificar si dubuja el ragdoll
		opcion1;//bandera para ver si entra o sale del juego

	ActorSprite
		* act_suelo,//suelo
		* act_arma,//cañon
		* act_base,//base del cañon
		* act_caja[3],//obstaculo fijo
		* act_caja_m[3];//obstaculo mobil

	Ragdoll* rag_3[5];

	Colisionador *procesar_c;

	RectangleShape 
		*fig_suelo,
		*fig_arma,
		*fig_base,
		*fig_caja[3],
		*fig_caja_m[3];

	b2World* mundo1;

	//Suelo
	b2Body* bdy_suelo;
	b2BodyDef bdydef_suelo;
	b2Fixture* fix_suelo;
	b2FixtureDef fixdef_suelo;

	//cañon
	b2Body* bdy_arma;
	b2BodyDef bdydef_arma;
	b2Fixture* fix_arma;
	b2FixtureDef fixdef_arma;

	//base del cañon
	b2Body* bdy_base;
	b2BodyDef bdydef_base;
	b2Fixture* fix_base;
	b2FixtureDef fixdef_base;
	
	//obstaculo fijo
	b2Body* bdy_caja[3];
	b2BodyDef bdydef_caja[3];
	b2Fixture* fix_caja[3];
	b2FixtureDef fixdef_caja[3];

	//obstaculo mobil
	b2Body* bdy_caja_m[3];
	b2BodyDef bdydef_caja_m[3];
	b2Fixture* fix_caja_m[3];
	b2FixtureDef fixdef_caja_m[3];

	//Resortes
	b2DistanceJoint* jnt_caja;
	b2DistanceJointDef jntdef_caja;

public:
	Juego(Vector2i resolucion, string titulo);
	void InicioJuego();
	void set_camera(Vector2f vista1, Vector2f vista2, Vector2f mover1);
	void iniciar_fisica();
	void iniciar_img();
	void iniciar_textos();
	
	void CambiaNivel(int nivel);
	void mostrar_textos();
	float deg2rad(float grados);
	void ColoreaRectangleShape(RectangleShape* figRag, int color);
	void CondicionFinal();

	void gameLoop();
	void Dibujar();
	void actualizar_fisica();
	void procesar_eventos();

};