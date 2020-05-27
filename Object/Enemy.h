#pragma once
#include "Obj.h"
#include <tuple>
#include "EnemyMove.h"

enum class ENEMY_TYPE
{
	A,
	B,
	C,
	D,
	E,
	F,
	MAX
};

enum class ENEMY_STATE
{
	TYPE,
	VECTOR,
	SIZE,
	AIM,
	MAX
};

using EnemyState = std::tuple<ENEMY_TYPE,Vector2Dbl,Vector2Dbl,MoveState&>; // �^�C�v�@�T�C�Y�@���W�@�ڕW���W

class Enemy :
	public Obj
{
public:
	Enemy();
	Enemy(EnemyState& state);
	void Update(sharedObj obj) override;
	~Enemy();
	ENEMY_TYPE _type;
	static int _enemyNum;
	bool isAttack(void);
private:
	bool SetAlive(bool alive);
	EnemyMove _moveCtl{ _pos, _rad };	// ���������X�g���g���ď���������
	void Init(void);
	bool _attackFlag;
};

