#include "Bullet.h"
#include <common\imageMng.h>
#include <Scene/SceneMng.h>


Bullet::Bullet()
{
}

Bullet::Bullet(UNIT_ID unitID, Vector2Dbl pos, double speed)
{
	_unitID = unitID;
	_pos = pos;
	_size = { 8,3 };
	_rad = PI/2;
	_zOrder = 0;
	_speed = speed;
	init();
}


Bullet::~Bullet()
{
}

void Bullet::Update(sharedObj obj)
{
	if (DestroyPrpc())
	{
		return;
	}

	_pos.y += _speed;

	lpSceneMng.AddActQue({ ACT_QUE::CHECK_HIT, *this });
}

bool Bullet::DestroyPrpc(void)
{
	if ((_pos.y <= -_size.y / 2.0) ||
		(_pos.y >= lpSceneMng.GameScreenSize.y + _size.y / 2.0)
		)
	{
		SetAlive(false);
	}
	if (_alive)
	{
		return false;
	}
	if (isAnimEnd())
	{
		_dead = true;
		return true;
	}
}

bool Bullet::init(void)
{
	AnimVector data;

	data.reserve(1);
	data.emplace_back(IMAGE_ID("’e")[0], 30);
	SetAnim(STATE::NORMAL, data);

	data.reserve(1);
	data.emplace_back(-1, 40);
	SetAnim(STATE::DETH, data);

	state(STATE::NORMAL);

	return true;
}
