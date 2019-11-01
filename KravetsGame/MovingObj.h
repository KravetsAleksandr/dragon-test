#include "Framework.h"
#include "Utils.h"
#include "iostream"
#include <list>

#ifndef MOVINGOBJ
#define MOVINGOBJ


class MovingObj
{
protected:
	IPoint _pos;

	int _radius;

	const int _penetrationDepth = 5;

	bool _isActive;

	Sprite* _tex;
	IPoint _spriteSize;

public:
	explicit MovingObj(Sprite* tex);
	virtual ~MovingObj(){};

	virtual void Update(const IPoint&) = 0;
	
	void Draw();
	void Init(const IPoint&);
	void SetDisabled();
	bool IsActive();
	IPoint GetPosition();
	int GetRadius();
	bool CheckObjectCollision(const IPoint&, const int&);
};

#endif