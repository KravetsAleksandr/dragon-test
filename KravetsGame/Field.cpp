#include "stdafx.h"
#include "Field.h"


Field::FieldTile::FieldTile(Sprite* tex, const IPoint& pos, const IPoint& texSize)
{
	_position = pos;
	_tex = tex;
	_texSize = texSize;
}


void Field::FieldTile::Draw(const IPoint& pos)
{
	if (!_tex)
	{
		return;
	}

	drawSprite(_tex, pos.x + _position.x - _texSize.x/2, pos.y + _position.y - _texSize.y/2);
}


Field::Field()
{
}


Field::~Field()
{
	destroySprite(_texType1);
	destroySprite(_texType2);
	destroySprite(_texType3);
	destroySprite(_texBullet);
	destroySprite(_texCursor);
	destroySprite(_texAvatar);
	destroySprite(_texEnemy);
	destroySprite(_texBoom);
}


void Field::Init(const std::map<std::string, int>& configs)
{
	_fieldSize = IPoint(configs.at("mapWidth"), configs.at("mapHeight"));
	_screenSize = IPoint(configs.at("windowWidth"), configs.at("windowHeight"));;

	_enemyCount = configs.at("enemyCount");
	_bulletCount = configs.at("bulletCount");
	_speed = configs.at("speed");
	_enemySpeed = configs.at("enemySpeed");
	_bulletSpeed = configs.at("bulletSpeed");


	//textures
	_texType1 = createSprite("data/background.png");
	_texType2 = createSprite("data/background1.png");
	_texType3 = createSprite("data/background2.png");
	_texBullet = createSprite("data/bomb.png");
	_texCursor = createSprite("data/cur_normal.png");
	_texAvatar = createSprite("data/star.png");
	_texEnemy = createSprite("data/beetle.png");
	_texBoom = createSprite("data/boom.png");
	getSpriteSize(_texBoom, _texBoomSize.x, _texBoomSize.y);


	//Cursor
	getSpriteSize(_texCursor, _texCurSize.x, _texCurSize.y);


	//field initialization
	int columns = _fieldSize.x / _tileSize + (_fieldSize.x % _tileSize > 0 ? 1 : 0);
	int rows = _fieldSize.x / _tileSize + (_fieldSize.x % _tileSize > 0 ? 1 : 0);

	srand(time(NULL));

	for (int i = 0; i < columns; i++) {
		for (int j = 0; j < rows; j++)
		{
			_fieldGround.push_back(std::make_shared<FieldTile>(GetSpriteByTileType(rand() % 3), IPoint(j * _tileSize, i * _tileSize), IPoint(_tileSize, _tileSize)));
		}
	}

	//screen rect
	_screenRect = IRect(-_tileSize, -_tileSize, _screenSize.x + _tileSize*2, _screenSize.y + _tileSize*2);


	//avatar initialization
	_avatarPtr = std::make_shared<Avatar>(_texAvatar);


	//enemy initialization
	for (size_t i = 0; i < _enemyCount; ++i)
	{
		std::shared_ptr<Enemy> sptr = std::make_shared<Enemy>(_texEnemy);
		_enemyPtr.push_back(sptr);
	}


	Restart();
}


void Field::Restart()
{
	//clear containers
	_enemyPtr.clear();
	_bulletPtr.clear();
	Bullet::ResetCounter();
	

	//set up the field position
	_scrollPos = IPoint((_screenSize.x - _fieldSize.x) / 2, (_screenSize.y - _fieldSize.y) / 2);


	//avatar re-init
	_avatarPtr->Init(IPoint(_fieldSize.x / 2 + _scrollPos.x, _fieldSize.y / 2 + _scrollPos.y));


	//enemies re-init
	for (enemyListIt it = _enemyPtr.cbegin(); it != _enemyPtr.cend(); ++it)
	{
		bool initSuccess = true;
		do
		{
			initSuccess = true;
			(*it)->Init(IPoint(_scrollPos.x + rand() % _fieldSize.x, _scrollPos.y + rand() % _fieldSize.y));

			//not too close to the avatar
			if ((*it)->GetPosition().GetDistanceTo(_avatarPtr->GetPosition()) < _screenSize.x / 2)
			{
				initSuccess = false;
				continue;
			}

			// not cover other enemies
			for (enemyListIt it_check = _enemyPtr.cbegin(); it_check != it; ++it_check)
			{
				if ((*it)->CheckObjectCollision((*it_check)->GetPosition(), (*it_check)->GetRadius()))
				{
					initSuccess = false;
					break;
				}
			}

		} while (!initSuccess);
	}

}


void Field::Draw()
{
	//Field draw
	for (FieldGround::const_iterator it = _fieldGround.begin(); it != _fieldGround.end(); ++it)
	{
		FieldTilePtr tile = *it;
		if (tile && _screenRect.Contains(tile->_position + _scrollPos))
		{ 
			tile->Draw(_scrollPos);
		}
	}


	//Enemy draw
	for (auto& sptr : _enemyPtr)
	{
		if ( _screenRect.Contains((*sptr).GetPosition()) )
			(*sptr).Draw();
	}


	//Avatar draw

	if (_isPaused)
		drawSprite(_texBoom, _avatarPtr->GetPosition().x - static_cast<int>(_texBoomSize.x / 2), _avatarPtr->GetPosition().y - static_cast<int>(_texBoomSize.x / 2));
	else
		_avatarPtr->Draw();



	//Bullet draw
	for (auto& sptr : _bulletPtr)
	{
		(*sptr).Draw();
	}


	//Cursor draw
	if (_texCursor)
	{
		drawSprite(_texCursor, _cursorPos.x - static_cast<int>(_texCurSize.x / 2), _cursorPos.y - static_cast<int>(_texCurSize.y / 2));
	}

}


 Sprite* Field::GetSpriteByTileType(int type)
{
	if (type == 0) {
		return _texType1;
	}
	if (type == 1) {
		return _texType2;
	}
	if (type == 2) {
		return _texType3;
	}
	return nullptr;
}


void Field::Update(const IPoint& dir, const IPoint& cursor, bool& shot)
{
	if (_isPaused)
	{
		GameOver();
		return;
	}
	
	IPoint shift(dir.x * _speed, dir.y * _speed);
	
	
	//cursor position update
	_cursorPos = cursor;

	
	//bullet update
	if (shot) 
	{
	//bullet generation
		if (Bullet::Count() >= _bulletCount)
		{
			(*_bulletPtr.cbegin())->SetDisabled();
		}
		std::shared_ptr<Bullet> sptr = std::make_shared<Bullet>(_texBullet);
		_bulletPtr.push_back(sptr);
		sptr->Init(IPoint(_avatarPtr->GetPosition().x, _avatarPtr->GetPosition().y));
		sptr->SetDirection(_avatarPtr->GetPosition(), _cursorPos, _bulletSpeed);
	}


	for (auto& sptr : _bulletPtr)
	{
		(*sptr).Update(shift);
	}


	// field update and check border collision
	IPoint texSize;
	getSpriteSize(_texAvatar, texSize.x, texSize.y);
	if ((_scrollPos.x + shift.x) > _screenSize.x/2) 
	{
		shift.x = 0;
		_scrollPos.x = _screenSize.x/2;
	}
	else if ((_scrollPos.x + _fieldSize.x + shift.x) <= _screenSize.x/2 + texSize.x)
		{
			shift.x = 0;
			_scrollPos.x = _screenSize.x/2 - _fieldSize.x + texSize.x;
		}
	else
		_scrollPos.x += shift.x;

	if ((_scrollPos.y + shift.y) > _screenSize.y/2) 
	{
		shift.y = 0;
		_scrollPos.y = _screenSize.y/2;
	}
	else if ((_scrollPos.y + _fieldSize.y + shift.y) < _screenSize.y/2 + texSize.y)
		{
			shift.y = 0;
			_scrollPos.y = _screenSize.y/2 - _fieldSize.y + texSize.y;
		}
	else
		_scrollPos.y += shift.y;


	//enemy update
	bool isCollision = false;

	for (enemyListIt it = _enemyPtr.cbegin(); it != _enemyPtr.cend(); ++it)
	{
		IPoint direction(_avatarPtr->GetPosition() - (*it)->GetPosition());
		direction.Normalized(_enemySpeed);
		IPoint enemyShift(shift + direction);

		if ( (*it)->IsPaused() )
			(*it)->Update(shift);
		else 
			(*it)->Update(enemyShift);

		//enemy-avatar collision
		if ((*it)->CheckObjectCollision(_avatarPtr->GetPosition(), _avatarPtr->GetRadius()))
		{
			isCollision = true;
			break;
		}

		//enemy-enemy collisions
		for (enemyListIt it_check = _enemyPtr.cbegin(); it_check != _enemyPtr.cend(); ++it_check)
		{
			if ( it_check == it || (*it_check)->IsPaused() ) continue;
			if ( (*it)->CheckObjectCollision((*it_check)->GetPosition(), (*it_check)->GetRadius()) )
			{
				(*it_check)->SetPaused();

				IPoint penetrationDirection = (*it_check)->GetPosition() - (*it)->GetPosition();

				IPoint relativeVelocity = (*it_check)->GetShift() - enemyShift;
				if (penetrationDirection * relativeVelocity > 0)
					continue;

				int exchangeVelocity = (-1) * (penetrationDirection * relativeVelocity);

				IPoint impulse(penetrationDirection.x * exchangeVelocity, penetrationDirection.y * exchangeVelocity);
				impulse.Normalized(1);
				(*it)->Update(impulse * (-2));
			}
		}
	}


	//game restart if enemy-avatar collision
	if (isCollision)
	{
		_isPaused = true;
		_freezeSecTimer = time(NULL);
		//Restart();
	}


	//bullet-enemy collision
	for (auto &sptr_bullet : _bulletPtr)
	{
		bool isHit = false;
		for (auto &sptr_enemy : _enemyPtr)
		{
			if ( (*sptr_bullet).IsActive() && (*sptr_bullet).CheckObjectCollision((*sptr_enemy).GetPosition(), (*sptr_enemy).GetRadius()) )
			{
				isHit = true;
				(*sptr_enemy).SetDisabled();
			}
		}
		if (isHit)
		{
			(*sptr_bullet).SetDisabled();
		}
		
		//check if goes beyond field boundaries
		IPoint texSize;
		getSpriteSize(_texBullet, texSize.x, texSize.y);
		IRect fieldRect(_scrollPos, _fieldSize.x - texSize.x, _fieldSize.y - texSize.y);
		if ( !fieldRect.Contains((*sptr_bullet).GetPosition()) )
		{
			(*sptr_bullet).SetDisabled();
		}


		// check if goes beyond screen boundaries
		if (!_screenRect.Contains((*sptr_bullet).GetPosition()))
		{
			(*sptr_bullet).SetDisabled();
		}
	}


	//delete non-acive objects
	_bulletPtr.remove_if([](const std::shared_ptr<Bullet> &bullet) {return !bullet->IsActive(); });
	_enemyPtr.remove_if([](const std::shared_ptr<Enemy> &enemy) {return !enemy->IsActive(); });
	

	// enemy regeneration
	if (_enemyPtr.size() < _enemyCount)
	{
		size_t born = _enemyCount -_enemyPtr.size();

		//enemy re-initialization
		for (size_t i = 0; i < born; ++i)
		{
			std::shared_ptr<Enemy> sptr = std::make_shared<Enemy>(_texEnemy);
			_enemyPtr.push_back(sptr);

			bool initSuccess = true;
			do
			{
				initSuccess = true;
				sptr->Init(IPoint(_scrollPos.x + rand() % _fieldSize.x, _scrollPos.y + rand() % _fieldSize.y));

				//not too close to the avatar
				if (sptr->GetPosition().GetDistanceTo(_avatarPtr->GetPosition()) < _screenSize.x/2)
				{
					initSuccess = false;
				}

			} while (!initSuccess);
		}

	}


}

void Field::GameOver()
{
	if (_isPaused && time(NULL) >= (_freezeSecTimer + _delaySec))
	{
		_isPaused = false;
		Restart();
	}

}
