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

using EnemyState = std::tuple<ENEMY_TYPE,Vector2Dbl,Vector2Dbl,MoveState&>; // タイプ　サイズ　座標　目標座標

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
	EnemyMove _moveCtl{ _pos, _rad };	// 初期化リストを使って初期化する
	void Init(void);
	bool _attackFlag;
};

