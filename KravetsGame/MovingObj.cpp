#include "stdafx.h"
#include "MovingObj.h"


MovingObj::MovingObj(Sprite* tex)
: _tex(tex)
{
	getSpriteSize(_tex, _spriteSize.x, _spriteSize.y);
	_spriteSize.x < _spriteSize.y ? _radius = _spriteSize.x/2 : _radius = _spriteSize.y/2;
	_radius -= _penetrationDepth;
}


void MovingObj::Init(const IPoint& pos)
{
	_pos = pos;
	_isActive = true;
}


void MovingObj::Draw()
{
	if (!_tex)
	{
		return;
	}
	drawSprite(_tex, _pos.x - static_cast<int>(_spriteSize.x/2), _pos.y - static_cast<int>(_spriteSize.y/2));
}


IPoint MovingObj::GetPosition()
{
	return _pos;
}


int MovingObj::GetRadius()
{
	return _radius;
}


bool MovingObj::CheckObjectCollision(const IPoint& point, const int& radius)
{
	return ( _pos.GetDistanceTo(point) <= (_radius + radius) );
}


void MovingObj::SetDisabled()
{
	_isActive = false;
}


bool MovingObj::IsActive()
{
	return _isActive;
}
