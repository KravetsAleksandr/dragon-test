#pragma once
#include "MovingObj.h"
#include <ctime>


class Enemy : public MovingObj
{
protected:
	bool _isPaused = false;
	
	const time_t _delaySec = 0.5;
	time_t _freezeSecTimer;

	IPoint _shift;

public:
	Enemy(Sprite* tex);
	~Enemy() {};

	void Update(const IPoint&) override; 
	void SetPaused();
	IPoint GetShift();
	bool IsPaused();
};

