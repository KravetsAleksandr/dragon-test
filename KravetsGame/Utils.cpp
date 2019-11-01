#include "stdafx.h"
#include "Utils.h"


IPoint::IPoint(int xInit, int yInit)
{
	x = xInit;
	y = yInit;
}


int IPoint::GetDistanceTo(const IPoint& point)
{
	IPoint direction(x - point.x, y - point.y);
	
	int vectorlength = static_cast<int>(sqrtf(direction.x * direction.x + direction.y * direction.y) );

	return vectorlength;
}


void IPoint::Normalized(const int& speed)
{
	float vectorlength = sqrtf(x * x + y * y);
	float _invLength = 1 / vectorlength;
	float xShift = x * _invLength;
	float yShift = y * _invLength;
	x = static_cast<int>(round(xShift * speed));
	y = static_cast<int>(round(yShift * speed));
}


IRect::IRect(int xInit, int yInit, int widthInit, int heightInit)
{
	x = xInit;
	y = yInit;
	width = widthInit;
	height = heightInit;
}


IRect::IRect(const IPoint& point, int widthInit, int heightInit)
{
	x = point.x;
	y = point.y;
	width = widthInit;
	height = heightInit;
}


bool IRect::Contains(const IPoint& point) const
{
	if (point.x >= x && point.x <= (x + width) && point.y >= y && point.y <= (y + height))
		return true;
	
	return false;
}