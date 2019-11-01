#include "stdafx.h"
#include "Enemy.h"




Enemy::Enemy(Sprite* tex) : MovingObj::MovingObj(tex)
{
	_isPaused = false;
}



void Enemy::Update(const IPoint& shift)
{
	_shift = shift;
	_pos = _pos + _shift;

	if ( _isPaused && time(NULL) >= (_freezeSecTimer + _delaySec) )
	{
			_isPaused = false;
	}
}


void Enemy::SetPaused()
{
	_isPaused = true;
	_freezeSecTimer = time(NULL);
}


bool Enemy::IsPaused()
{
	return _isPaused;
}


IPoint Enemy::GetShift()
{
	return _shift;
}

