#include "stdafx.h"
#include "Bullet.h"


size_t Bullet::_bulletCounter = 0;


Bullet::Bullet(Sprite* tex) : MovingObj::MovingObj(tex)
{
	++_bulletCounter;
}


Bullet::~Bullet()
{
	--_bulletCounter;
}


void Bullet::SetDirection(const IPoint& sPos, const IPoint& ePos, const int& speed)
{
	_direction = ePos - sPos; 
	_direction.Normalized(speed);
}


void Bullet::Update(const IPoint& shift)
{
	_pos = _pos + shift + _direction;
}
