#pragma once
#include "MovingObj.h"

class Avatar : public MovingObj
{
public:
	using MovingObj::MovingObj;
	~Avatar() {};

	void Update(const IPoint&) override;
};

