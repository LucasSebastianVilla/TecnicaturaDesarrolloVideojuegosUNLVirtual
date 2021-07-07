#include "BaseObject.h"
#include <iostream>

BaseObject::BaseObject() {};

BaseObject::BaseObject(string filename)
{
	_pTex = new Texture();
	if (!_pTex->loadFromFile(filename)){
		cout << "No se pudo cargar la imagen" << endl;
	}

	_pSpr = new Sprite();
	_pSpr->setTexture(*_pTex);
	
	_position.x = 0.0f;
	_position.y = 0.0f;
	_acceleration.x = 0.0f;
	_acceleration.y = 0.0f;
	_velocity.x = 0.0f;
	_velocity.y = 0.0f;
}

BaseObject::~BaseObject(){
	//delete _pSpr;
	//delete _pTex;
}

void BaseObject::Draw(RenderWindow *pVentana){
	pVentana->draw(*_pSpr);
}

void BaseObject::Update(float dt){
	_velocity.x += _acceleration.x * dt;
	_velocity.y += _acceleration.y * dt;

	_position.x += _velocity.x * dt;
	_position.y += _velocity.y * dt;
	
	_pSpr->setPosition(_position);

	_acceleration.x = 0.0f;
	_acceleration.y = 0.0f;
}
void BaseObject::SetTipo(char tipo) {
	_tipo = tipo;
}
char BaseObject::GetTipo() {
	return _tipo;
}