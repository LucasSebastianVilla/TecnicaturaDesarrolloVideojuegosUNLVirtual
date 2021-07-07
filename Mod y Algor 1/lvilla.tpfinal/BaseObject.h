#pragma once
#include <SFML\Graphics.hpp>
#include <string>

using namespace std;
using namespace sf;

class BaseObject
{
public:
	BaseObject(string filename);
	BaseObject();
	~BaseObject();


	void Draw(RenderWindow *pVentana);
	void Update(float dt);

	void SetScale(float uniformScale){_pSpr->setScale(uniformScale, uniformScale);}
	
	void SetPosition(Vector2f pos){ _position = pos; }
	void SetAcceleration(Vector2f acc){ _acceleration = acc; }
	void SetVelocity(Vector2f vel){ _velocity = vel; }

	void SetTipo(char tipo);//configuro el tipo de personaje
	char GetTipo();//obtengo el tipo de personaje

	Vector2f GetPosition(){ return _position; }
	Vector2f GetAcceleration(){ return _acceleration; }
	Vector2f GetVelocity(){ return _velocity; }

	void SetOculto(bool hide) { _oculto = hide; }
	bool GetOculto() { return _oculto; }

	void AddAcceleration(Vector2f acc){
		_acceleration.x += acc.x;
		_acceleration.y += acc.y;
	}

	void AddVelocity(Vector2f vel) {
		_velocity.x += vel.x;
		_velocity.y += vel.y;
	}
private:	
	Sprite *_pSpr;
	Texture *_pTex;
	Vector2f _position;
	Vector2f _acceleration;
	Vector2f _velocity;
	char _tipo;//tipo personaje E - Enemigo, A - Amigo
	bool _oculto = false;

};

