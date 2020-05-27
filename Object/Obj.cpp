#include <DxLib.h>
#include "Obj.h"
#include <Scene\SceneMng.h>
#include <common\imageMng.h>




Obj::Obj()
{
	_alive = true;
	_dead = false;
	_animFrame = 0;
	_animCount = 0;
	_rad = 0;
	_zOrder = 0;
	_unitID = UNIT_ID::NON;
}


void Obj::Draw(void)
{
	if (_animMap.find(_state) == _animMap.end()) // 見つかったらｲﾃﾚｰﾀを返す　endまで来るということは、そのﾃﾞｰﾀが存在しないということ
	{
		return;
	}
	if (_animFrame < 0 || _animFrame >= _animMap[_state].size()) // 要素数を調べるとき、取得する時はsizeを使う
	{
		return;
	}
	
	// _animMap[_animKey].size() コマ数

	if (_animMap[_state][_animFrame].first >= 0)
	{
		if (_animCount >= _animMap[_state][_animFrame].second)
		{
			_animFrame++;
		}

		_animCount++;

		if (_animFrame >= _animMap[_state].size())
		{
			_animFrame = 0;
			_animCount = 0;
		}

		lpSceneMng.AddDrawQue({ _animMap[_state][_animFrame].first,lpSceneMng.GameScreenOffset.x + _pos.x,lpSceneMng.GameScreenOffset.y +_pos.y , _rad, _zOrder, LAYER::CHAR, DX_BLENDMODE_NOBLEND, 255 });
	}	
}

void Obj::Draw(int id)
{
	lpSceneMng.AddDrawQue({ id,lpSceneMng.GameScreenOffset.x + _pos.x,lpSceneMng.GameScreenOffset.y + _pos.y , _rad,_zOrder,LAYER::CHAR,DX_BLENDMODE_NOBLEND, 255 });
}


Obj::~Obj()
{
}


bool Obj::state(const STATE state)
{
	if (_animMap.find(state) == _animMap.end())
	{
		return false;
	}

	if (_state != state)
	{
		_animFrame = 0;
		_animCount = 0;
	}

	_state = state;
	return true;
}

const STATE Obj::state(void) const
{
	return _state;
}

const UNIT_ID Obj::unitID(void) const
{
	return _unitID;
}

bool Obj::SetAnim(const STATE state, AnimVector& data)
{
	return (_animMap.try_emplace(state, std::move(data))).second; // first iteratorが返る second bool値が返る　17以降しか使えない
	// emplace 新規にﾃﾞｰﾀを入力する tryしてほしいキー　トライした結果
    

	/*if (_animMap.find(Key) == _animMap.end())
	{
		_animMap[Key] = data;
		return true;
	}
	
	return false;*/
}

bool Obj::SetAlive(bool alive)
{
	_alive = alive;
	if (!_alive)
	{
		state(STATE::DETH);
	}

	return true;
}

bool Obj::isAnimEnd(void)
{
	// _animMap[_state][_animFrame].first >= 0
	if (_animMap.find(_state) == _animMap.end()) // 見つかったらｲﾃﾚｰﾀを返す　endまで来るということは、そのﾃﾞｰﾀが存在しないということ
	{
		return true;
	}
	if (_animFrame < 0 || _animFrame >= _animMap[_state].size()) // 要素数を調べるとき、取得する時はsizeを使う
	{
		return true;
	}


	if (_animMap[_state][_animFrame].first == -1)
	{
		return true;
	}
	else
	{
		return false;
	}

	return false;
}

const Vector2Dbl Obj::pos(void) const
{
	return _pos;
}

const Vector2Dbl Obj::size(void) const
{
	return _size;
}

bool Obj::DestroyPrpc(void) // 破棄していいかの確認
{
	if (_alive)
	{
		return false;
	}
	if (isAnimEnd())
	{
		_dead = true;
	}

	return true;
}
