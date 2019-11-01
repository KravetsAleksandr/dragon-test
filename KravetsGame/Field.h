#pragma once

#include "Utils.h"
#include "Framework.h"
#include <vector>
#include <map>
#include "iostream"
#include <stdlib.h>  
#include <ctime>
#include "Avatar.h"
#include "Enemy.h"
#include "Bullet.h"

class Field
{
public:
	Field();
	~Field();

	void Init(const std::map<std::string, int>& configs);
	void Draw();
	void Update(const IPoint& dir, const IPoint& cursor, bool& shot);


private:

	struct FieldTile
	{
		FieldTile(Sprite* tex, const IPoint& pos, const IPoint& texSize);
		IPoint _position;
		Sprite* _tex;
		IPoint _texSize;
		void Draw(const IPoint& pos);
	};
	typedef std::shared_ptr<FieldTile> FieldTilePtr;
	typedef std::vector<FieldTilePtr> FieldGround;
	

	std::shared_ptr<Avatar> _avatarPtr;
	std::list< std::shared_ptr<Enemy> > _enemyPtr;
	typedef std::list< std::shared_ptr<Enemy> >::const_iterator enemyListIt;
	
	std::list< std::shared_ptr<Bullet> > _bulletPtr;
	typedef std::list< std::shared_ptr<Bullet> >::const_iterator bulletListIt;

	Sprite* GetSpriteByTileType(int type);
	const int _tileSize = 50;

	IRect _screenRect;
	IPoint _fieldSize;
	IPoint _screenSize;
	IPoint _scrollPos;
	IPoint _cursorPos;

	int _speed;
	int _enemySpeed;
	int _bulletSpeed;
	int _enemyCount;
	int _bulletCount;
	
	Sprite* _texType1;
	Sprite* _texType2;
	Sprite* _texType3;
	Sprite* _texCursor;
	Sprite* _texAvatar;
	Sprite* _texEnemy;
	Sprite* _texBullet;
	Sprite* _texBoom;

	IPoint _texCurSize;
	IPoint _texBoomSize;

	FieldGround _fieldGround;

	void Restart();
	void GameOver();

	bool _isPaused = false;
	const time_t _delaySec = 3;
	time_t _freezeSecTimer;

};

