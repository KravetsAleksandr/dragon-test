// KravetsGame.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "Framework.h"
#include "GameEngine.h"
#include <string>
#include <map>


GameEngine _game;
std::map<std::string, std::string> params;

/* Test Framework realization */
class MyFramework : public Framework
{
public:

	virtual void PreInit(int& width, int& height, bool& fullscreen)
	{
		width = 800;
		height = 600;
		std::string value = params["-window"];
		if (!value.empty())
		{
			width = std::stoi(value.substr(0, value.find_first_of("x")));
			height = std::stoi(value.substr(value.find_first_of("x") + 1, value.size() - value.find_first_of("x") - 1));
		}
		fullscreen = false;
	}

	virtual bool Init()
	{
		_game.Init(params);
		showCursor(false);
		return true;
	}

	virtual void Close()
	{
	}

	virtual bool Tick()
	{
		_game.Update();
		return false;
	}

	virtual void onMouseMove(int x, int y, int xrelative, int yrelative)
	{
		_game.MouseMove(IPoint(x, y));
	}

	virtual void onMouseButtonClick(FRMouseButton button, bool isReleased)
	{
		_game.MouseButton(button, isReleased);
	}

	virtual void onKeyPressed(FRKey k)
	{	
		_game.KeyPressed(k);
	}

	virtual void onKeyReleased(FRKey k)
	{
		_game.KeyReleased(k);
	}
};


int main(int argc, char *argv[])
{
	for (int c = 1; c <= (argc - 1) / 2; c++)
	{
		params.insert(std::pair<std::string, std::string >(argv[2*c - 1], argv[2*c]));
	}	
	return run(new MyFramework);
}

