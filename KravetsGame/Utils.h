#pragma once
#include <iostream>

class IPoint
{
public:
	IPoint() {};
	IPoint(int xInit, int yInit);
	
	int x;
	int y;
	~IPoint() {};

	int GetDistanceTo(const IPoint&);
	void Normalized(const int& speed);

	IPoint operator * (int i) const
	{
		return IPoint(x * i, y * i);
	}

};



inline int operator * (const IPoint& a, const IPoint& b)
{
	return int(a.x * b.x + a.y * b.y);
}


inline IPoint operator + (const IPoint& a, const IPoint& b)
{
	return IPoint(a.x + b.x, a.y + b.y);
}


inline IPoint operator - (const IPoint& a, const IPoint& b)
{
	return IPoint(a.x - b.x, a.y - b.y);
}


inline bool operator == (const IPoint& a, const IPoint& b)
{
	return a.x == b.x && a.y == b.y;
}


inline bool operator != (const IPoint&a, const IPoint& b)
{
	return a.x != b.x || a.y != b.y;
}


inline std::ostream& operator<<(std::ostream& os, const IPoint& p)
{
	os << p.x << " " << p.y;
	return os;
}


class IRect
{
public:
	IRect() {};
	~IRect() {};
	IRect(int xStart, int yStart, int width, int height);
	IRect(const IPoint& point, int width, int height);


	int x;
	int y;
	int width;
	int height;


	bool Contains(const IPoint& point) const;
};