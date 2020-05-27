#include "EnemyMove.h"
#include <math.h>
#include <_debug\_DebugDispOut.h>
#include <_debug\_DebugConOut.h>
#include <Scene/SceneMng.h>
#include <Object/Enemy.h>

int EnemyMove::_enemyCnt = 0;

EnemyMove::EnemyMove(Vector2Dbl& pos, double& rad) : _pos(pos), _rad(rad)	// ‰Šú‰»Žq‚ð‘‚­Žž : ‚ð‘‚­
{
	_move = nullptr;
	_aimCnt = -1;
	_savePos = { 0,0 };
	_dead = false;
}


EnemyMove::~EnemyMove()
{
}

void EnemyMove::Update(sharedObj plObj, bool attFlag)
{
	plPos = (*plObj).pos();
	_attackFlag = attFlag;

	if (_move != nullptr)
	{

		(this->*_move)();
	}

	//_pos.x++;
}

void EnemyMove::Update(bool state)
{
	_dead = state;
	if (_move != nullptr)
	{

		(this->*_move)();
	}
}


bool EnemyMove::SetMoveState(MoveState & state, bool newFlag)
{
	if (newFlag)
	{
		_aim.clear();
	}
	_aim = std::move(state); // std::move(data) 

	if (newFlag)
	{
		SetMovePrg();
	}

	return true;
}

const MOVE_TYPE EnemyMove::state(void) const
{
	return _aim[_aimCnt].first;
}

void EnemyMove::SetMovePrg(void)
{
	_aimCnt++;
	if (_aimCnt >= _aim.size()) // _aim.size()‚Í—v‘f”
	{
		return;
	}
	_startPos = _pos;				// ‰ŠúˆÊ’uÝ’è
	_endPos = _aim[_aimCnt].second;	// –Ú•WˆÊ’uÝ’è

	switch (_aim[_aimCnt].first)
	{
	case MOVE_TYPE::WAIT:
		_move = &EnemyMove::Wait;
		_count = 0;
		break;
	case MOVE_TYPE::SIGMOID:
		_move = &EnemyMove::MoveSigmoid;
		
		_radius = 100;

		_endPos.y = _endPos.y > _startPos.y ? _endPos.y : _endPos.y - _radius*2;
		
		// _endPos.x が　ｹﾞｰﾑｼｰﾝ画面を中心に、どちらの側を判断して、処理される
		if (_endPos.x > lpSceneMng.GameScreenSize.x / 2)
		{
			_endPos.x = _startPos.x < lpSceneMng.GameScreenSize.x / 2 ? _endPos.x : lpSceneMng.GameScreenSize.x / 2 - (_endPos.x - lpSceneMng.GameScreenSize.x / 2);
		}
		else
		{
			_endPos.x = _startPos.x > lpSceneMng.GameScreenSize.x / 2 ? _endPos.x : lpSceneMng.GameScreenSize.x / 2 + (lpSceneMng.GameScreenSize.x / 2 - _endPos.x);
		}

		// sigmoidの中心点
		_middleSig = (_endPos.x + _startPos.x) / 2.0;

		_speed.x = (_endPos.x - _startPos.x) / 90.0;
		_speed.y = 0.04;

		// 方向を判断する
		_direction = _speed.x > 0 ? 1 : -1;

		// sigmoidの縦幅
		_sizeSig = _endPos.y - _startPos.y;

		_count = 0;

		break;
	case MOVE_TYPE::SPIRAL:
		_move = &EnemyMove::MoveSpiral;

		// _endPos.x が　ｹﾞｰﾑｼｰﾝ画面を中心に、どちらの側を判断して、処理される
		if (_endPos.x > lpSceneMng.GameScreenSize.x / 2)
		{
			_endPos.x = _startPos.x < lpSceneMng.GameScreenSize.x / 2 ? lpSceneMng.GameScreenSize.x / 2 - (_endPos.x - lpSceneMng.GameScreenSize.x / 2) : _endPos.x;
		}
		else
		{
			_endPos.x = _startPos.x < lpSceneMng.GameScreenSize.x / 2 ? _endPos.x : lpSceneMng.GameScreenSize.x / 2 + (lpSceneMng.GameScreenSize.x / 2 - _endPos.x);
		}
		
		// startPosを判断して、角度と方向を処理する
		if (_startPos.y > _endPos.y)
		{
			_angle = PI / 2;
			_rotaSpeed = _direction > 0 ? -0.1 : 0.1;

		}
		else
		{
			_angle = -PI / 2;
			_rotaSpeed = _direction > 0 ? 0.1 : -0.1;
		}

		_length = _endPos - _pos;

		break;
	case MOVE_TYPE::PITIN:
		_move = &EnemyMove::PitIn;

		
		
		_limit = 20;
		

		/*if ((_aimCnt + 1) < _aim.size())*/
		{
			if (_aim[_aimCnt + 1].first == MOVE_TYPE::EXPEND)
			{
				_endPos = _savePos;
				_pos = { plPos.x, -100.0 };
			}
			else
			{
				_center = _aim[_aimCnt].second;
			}
		}

		
		_count = 0;
		
		break;
	case MOVE_TYPE::LR:
		_move = &EnemyMove::MoveLR;
		_rad = 0.0;

		break;
	case MOVE_TYPE::EXPEND:
		_move = &EnemyMove::Expend;

		

		_length = _startPos - _aim[_aimCnt].second;
		_expendLength = 1.6*_length - _length;
		_speed = _expendLength / 60.0;


		if (_savePos != Vector2Dbl{0,0})
		{
			_count = 15;
		}
		else
		{
			_count = 0;
		}
		
		break;
	case MOVE_TYPE::ATTACK:
		_move = &EnemyMove::MoveAttack;

		_endPos = plPos + Vector2Dbl{ 0.0,100.0 };
		_speed = (_endPos - _startPos) / 60.0;
		_count = 0;

		break;
	default:
		AST();
		_move = &EnemyMove::Wait;	
		break;
	}

}

void EnemyMove::MoveSigmoid(void)
{
	_count++;

	if (_count <= 90)
	{
		
		_posOld = _pos;
		_pos.x += _speed.x;

		_pos.y = _startPos.y + 
			_sizeSig / (1 + exp(_speed.y*_direction*(_middleSig - _pos.x)));

		_rad = PI / 2 + atan2(_pos.y - _posOld.y, _pos.x - _posOld.x);
	}
	else
	{
		_pos = _endPos;
		SetMovePrg();
	}

}

void EnemyMove::MoveSpiral(void)
{
	
	if (_radius > 0.0)
	{
		_posOld = _pos;

		_angle += _rotaSpeed;
		_radius -= 1.4;

		_pos.x = _endPos.x + _radius * cos(_angle);
		_pos.y = _endPos.y + _radius * sin(_angle);

		_rad = PI / 2 + atan2(_pos.y - _posOld.y, _pos.x - _posOld.x);
	}
	else
	{
		_pos = _endPos;
		SetMovePrg();
	}
}

void EnemyMove::PitIn(void)
{
	
	if (_count < 60)
	{
		// 次はMOVELRなら
		/*if ((_aimCnt + 1) < _aim.size())*/
		{
			if (_aim[_aimCnt + 1].first == MOVE_TYPE::LR)
			{
				_endPos.x = _aim[_aimCnt].second.x + (abs(lpSceneMng.frame() - (_limit*4) * ((lpSceneMng.frame() + _limit*2) / (_limit * 4))) - _limit);
			}
		}
		
		_posOld = _pos;
		_length = _endPos - _pos;

		_speed = _length / (60.0 - _count);		// 120 - _count = 0 の状態を避けたいため、_count < 120

		_pos += _speed;
		_rad = PI / 2 + atan2(_pos.y - _posOld.y, _pos.x - _posOld.x);
		_count++;
	}
	else
	{
		_pos = _endPos;
		_enemyCnt++;
		SetMovePrg();
	}

}

void EnemyMove::Wait(void)
{
	_count++;	
	if (_count > _aim[_aimCnt].second.x)
	{
		
		SetMovePrg();
	}	
}

void EnemyMove::MoveLR(void)
{
	_pos.x = _center.x + (abs(lpSceneMng.frame() - (_limit * 4) * ((lpSceneMng.frame() + _limit * 2) / (_limit * 4))) - _limit);

	if (_dead)
	{
		_enemyCnt--;
		TRACE("%d\n",_enemyCnt);
	}


	if (EnemyMove::_enemyCnt == Enemy::_enemyNum)
	{
		if (_pos == _center)
		{
			SetMovePrg();
		}
	}
}

void EnemyMove::Expend(void)
{
	_pos = (_count/ 30) % 2 == 0 ? _pos + _speed : _pos - _speed;
	_count++;
	_rad = _rad > 0 ? _rad - 0.05: 0;
	//if (_count >= 120 * 5)
	//{
	//	_savePos = _pos;	// ExpendのendPosを保存する
	//	SetMovePrg();
	//}

	if (_attackFlag)
	{
		TRACE("Count : %d", _count);
		_savePos = _pos;
		SetMovePrg();
	}
}

void EnemyMove::MoveAttack(void)
{
	if (_count < 60)
	{
		_posOld = _pos;
		_pos += _speed;
		_rad = PI / 2 + atan2(_pos.y - _posOld.y, _pos.x - _posOld.x);
		_count++;
	}
	else
	{

		_pos = _endPos;

		
		for (auto itr = _aim.begin(); itr != _aim.end(); itr++)
		{
			if ((*itr).first == MOVE_TYPE::LR)
			{
				_aim.erase(itr);
				break;
			}
		}

		for (_aimCnt = 0; _aimCnt < _aim.size(); _aimCnt++)
		{
			if (_aim[_aimCnt].first == MOVE_TYPE::PITIN)
			{
				_aimCnt--;
				break;
			}
		}


		SetMovePrg();

	}
}
