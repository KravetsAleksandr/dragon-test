
#include "Framework.h"
#include "Field.h"
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <algorithm>

#ifndef GAMEENGINE
#define GAMEENGINE

class GameEngine
{
public:
	GameEngine(){}
	~GameEngine(){}
	
	void Update();
	void Draw();
	void Init(std::map<std::string, std::string>& params);

	void MouseMove(const IPoint& cursor);
	void MouseButton(const FRMouseButton& button, bool& isReleased);

	void KeyPressed(FRKey key);
	void KeyReleased(FRKey key);

private:
	
	Field _field;

	IPoint _cursor;

	bool _shot = false;

	unsigned int _timer = 0;
	std::list< std::shared_ptr<FRKey> > _frkeysPtr;


	std::map<std::string, int> _configMap = 
	{
		{"windowWidth", 800},
		{"windowHeight", 600},
		{"mapWidth", 1500},
		{"mapHeight", 1500},
		{"enemyCount", 7},
		{"bulletCount", 3},
		{"speed", 5},
		{"enemySpeed", 2},
		{"bulletSpeed", 8}
	};
	
};

#endif