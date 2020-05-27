#include <common\imageMng.h>
#include "Enemy.h"
#include <Scene\GameScene.h>
#include <Scene/SceneMng.h>

 int Enemy::_enemyNum = 0;

Enemy::Enemy()
{
	_type = ENEMY_TYPE::A;
	Init();
}

Enemy::Enemy(EnemyState & state)
{
	_type = std::get<static_cast<int>(ENEMY_STATE::TYPE)>(state);
	_pos = std::get<static_cast<int>(ENEMY_STATE::VECTOR)>(state);
	_size = std::get<static_cast<int>(ENEMY_STATE::SIZE)>(state);
	_moveCtl.SetMoveState(std::get<static_cast<int>(ENEMY_STATE::AIM)>(state),true);
	_unitID = UNIT_ID::ENEMY;
	_attackFlag = false;
	Init();
}

void Enemy::Update(sharedObj obj)
{
	if (DestroyPrpc())
	{
		if (isDead())
		{
			_enemyNum--;
			_moveCtl.Update(isDead());
			
			TRACE("Enemy Alive: %d\n", Enemy::_enemyNum);
		}
		return;
	}
	
	if (rand() % 1000 == 0)
	{
		_attackFlag = true;
	}

	_moveCtl.Update(obj, _attackFlag);


	_attackFlag = false;
	if (rand() % 1000 == 0)
	{
		/*SetAlive(false);*/
		lpSceneMng.AddActQue({ ACT_QUE::SHOT, *this });
	}

	lpSceneMng.AddActQue({ ACT_QUE::CHECK_HIT, *this });
}


Enemy::~Enemy()
{
}



bool Enemy::isAttack(void)
{
	return _moveCtl.state() == MOVE_TYPE::EXPEND;
}

bool Enemy::SetAlive(bool alive)
{
	if (!alive)
	{
		// ‰æ–Ê—h‚ç‚ê‚µ‚ÌQue‚ð“Š‚°‚é
		lpSceneMng.AddActQue({ ACT_QUE::SHAKE,*this });
		lpSceneMng.AddActQue({ ACT_QUE::BLAST, *this });
	}
	
	return Obj::SetAlive(alive);
}

void Enemy::Init(void)
{
	AnimVector data;
	data.emplace_back(IMAGE_ID("·¬×")[10 + 2 * static_cast<int>(_type)], 30);
	data.emplace_back(IMAGE_ID("·¬×")[11 + 2 * static_cast<int>(_type)], 60);
	SetAnim(STATE::NORMAL, data);

	data.emplace_back(IMAGE_ID("“G”š”­")[0], 10);
	data.emplace_back(IMAGE_ID("“G”š”­")[1], 15);
	data.emplace_back(IMAGE_ID("“G”š”­")[2], 20);
	data.emplace_back(IMAGE_ID("“G”š”­")[3], 25);
	data.emplace_back(IMAGE_ID("“G”š”­")[4], 30);
	data.emplace_back(-1, 35);
	SetAnim(STATE::DETH, data);

	state(STATE::NORMAL);
}
