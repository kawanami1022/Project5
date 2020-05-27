#pragma once
#include <common\Vector2.h>
#include <vector>
#include <_debug\_DebugConOut.h>
#include <Object\Obj.h>

#define PI 3.14159265

enum class MOVE_TYPE
{
	WAIT,
	SIGMOID,
	SPIRAL,
	PITIN,
	LR,
	EXPEND,
	ATTACK
};

using MoveState = std::vector<std::pair<MOVE_TYPE,Vector2Dbl>>; // moveの種別　目標地点

class EnemyMove
{
public:
	EnemyMove(Vector2Dbl& pos, double& rad);
	~EnemyMove();
	void Update(sharedObj plObj, bool attFlag);
	void Update(bool state);
	bool SetMoveState(MoveState& state, bool newFlag);	// 6か所からの敵の出現は外で行い、enemyはこれに従う
	const MOVE_TYPE state(void) const;

	static int _enemyCnt;
private:
	void SetMovePrg(void);

	void (EnemyMove::*_move)(void);

	void MoveSigmoid(void);		// 目標地点までの移動　返り値と引数は同じにしないといけない
	void MoveSpiral(void);		// 回転
	void PitIn(void);			// 空きｽﾍﾟｰｽに挿入
	void Wait(void);			// 待ち時間
	void MoveLR(void);			// 左右移動
	void Expend(void);
	void MoveAttack(void);

	unsigned int _count;					// 次のｼｰﾝまでのｶｳﾝﾄ
	

	MoveState _aim;
	int _aimCnt;				// vectorの位置

	Vector2Dbl _startPos;		// 各移動関数のｽﾀｰﾄ地点
	Vector2Dbl _endPos;			// 各移動関数のｴﾝﾄﾞ地点 

	Vector2Dbl& _pos;
	Vector2Dbl _posOld;
	double& _rad;

	Vector2Dbl _length;
	Vector2Dbl _speed;
	double _middleSig;
	double _sizeSig;
	int _direction;
	double _radius;
	double _angle;
	double _rotaSpeed;
	int _limit;
	
	bool _dead;

	Vector2Dbl _center;
	Vector2Dbl _expendLength;

	Vector2Dbl plPos;
	Vector2Dbl _savePos;	// endPosを保存する

	bool _attackFlag;
};

