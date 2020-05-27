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

using MoveState = std::vector<std::pair<MOVE_TYPE,Vector2Dbl>>; // move�̎�ʁ@�ڕW�n�_

class EnemyMove
{
public:
	EnemyMove(Vector2Dbl& pos, double& rad);
	~EnemyMove();
	void Update(sharedObj plObj, bool attFlag);
	void Update(bool state);
	bool SetMoveState(MoveState& state, bool newFlag);	// 6��������̓G�̏o���͊O�ōs���Aenemy�͂���ɏ]��
	const MOVE_TYPE state(void) const;

	static int _enemyCnt;
private:
	void SetMovePrg(void);

	void (EnemyMove::*_move)(void);

	void MoveSigmoid(void);		// �ڕW�n�_�܂ł̈ړ��@�Ԃ�l�ƈ����͓����ɂ��Ȃ��Ƃ����Ȃ�
	void MoveSpiral(void);		// ��]
	void PitIn(void);			// �󂫽�߰��ɑ}��
	void Wait(void);			// �҂�����
	void MoveLR(void);			// ���E�ړ�
	void Expend(void);
	void MoveAttack(void);

	unsigned int _count;					// ���̼�݂܂ł̶���
	

	MoveState _aim;
	int _aimCnt;				// vector�̈ʒu

	Vector2Dbl _startPos;		// �e�ړ��֐��̽��Ēn�_
	Vector2Dbl _endPos;			// �e�ړ��֐��̴��ޒn�_ 

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
	Vector2Dbl _savePos;	// endPos��ۑ�����

	bool _attackFlag;
};

