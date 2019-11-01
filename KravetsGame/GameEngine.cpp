
#include "stdafx.h"
#include "GameEngine.h"



void GameEngine::Init(std::map<std::string, std::string>& param)
{
	std::string value = param["-window"];
	if (!value.empty())
	{
		_configMap.at("windowWidth") = std::stoi(value.substr(0, value.find_first_of("x")));
		_configMap.at("windowHeight") = std::stoi(value.substr(value.find_first_of("x") + 1, value.size() - value.find_first_of("x") - 1));
	}

	value = param["-map"];
	if (!value.empty()) {
		_configMap.at("mapWidth") = std::stoi(value.substr(0, value.find_first_of("x")));
		_configMap.at("mapHeight") = std::stoi(value.substr(value.find_first_of("x") + 1, value.size() - value.find_first_of("x") - 1));
	}
	
	value = param["-num_enemies"];
	if (!value.empty()) {
		
		_configMap.at("enemyCount") = std::stoi(value);
	}
	
	value = param["-num_ammo"];
	if (!value.empty()) {
		_configMap.at("bulletCount") = std::stoi(value);
	}

	_field.Init(_configMap);


	_cursor.x = 0;
	_cursor.y = 0;

	_shot = false;

	_timer = getTickCount();
}

void GameEngine::Update()
{
	// field scroll direction
	IPoint dir = IPoint(0, 0);
	for (auto &sptr : _frkeysPtr)
	{
		if ((*sptr) == FRKey::RIGHT)
			dir.x = -1;
		if ((*sptr) == FRKey::LEFT)
			dir.x = 1;
		if ((*sptr) == FRKey::DOWN)
			dir.y = -1;
		if ((*sptr) == FRKey::UP)
			dir.y = 1;
	}
	
	while ((getTickCount() - _timer) > 17) //60 updates per second
	{
		_timer = getTickCount();
		_field.Update(dir, _cursor, _shot);
		if (_shot)
			_shot = false;
	}
	
	Draw();
}

void GameEngine::Draw()
{
	//field draw
	_field.Draw();
}


void GameEngine::MouseMove(const IPoint& cursor)
{
	_cursor = cursor;
}


void GameEngine::MouseButton(const FRMouseButton& button, bool& isReleased)
{
	if (button == FRMouseButton::LEFT)
		_shot = true;
	if (isReleased)
		_shot = false;
}


void GameEngine::KeyPressed(FRKey key)
{
	bool isPressed = false;

	for (auto &sptr : _frkeysPtr)
	{
		if ((*sptr) == key)
		{
			isPressed = true;
			break;
		}
	}

	if (!isPressed)
	{
		_frkeysPtr.push_back(std::make_shared<FRKey>(key));
	}
}


void GameEngine::KeyReleased(FRKey key)
{
	_frkeysPtr.remove_if([key](const std::shared_ptr<FRKey> &releasedKey) {return (*releasedKey) == key; });
}
