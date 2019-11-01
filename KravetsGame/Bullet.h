#pragma once
#include "MovingObj.h"

class Bullet : public MovingObj
{
private:
	IPoint _direction;

	static size_t _bulletCounter;
	bool once = false;

public:
	Bullet(Sprite* tex);
	~Bullet();


	void Update(const IPoint&) override;
	void SetDirection(const IPoint&, const IPoint&, const int&);

	static size_t Count() { return _bulletCounter; }
	static void ResetCounter() { _bulletCounter = 0; };
};

