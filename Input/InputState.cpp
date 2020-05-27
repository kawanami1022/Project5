#include <_debug\_DebugConOut.h>
#include "InputState.h"




InputState::InputState()
{
	for (auto id : INPUT_ID()) // enum class のbigin関数とend関数がないため、その場で作る　そうすれば範囲for文が作れる
	{
		_state.try_emplace(INPUT_ID::LEFT, KeyPair{ 0,1 }); // try_emplace その場でﾃﾞｰﾀを作る
	}
}


InputState::~InputState()
{
}

const KeyMap & InputState::state(void) const
{
	return _state;
}

const KeyPair & InputState::state(INPUT_ID id) const // 書き込みに関することができない　_stateの型はmapで作られている　idに該当するキーがmapの中にないとき新しくmakeしてしまう。その行為がconstに引っかかる　だからfindを使う
{
	static KeyPair defData = { 0,0 }; // &で参照しているからstaticの特性を使う　最初からこのデータは存在するから安全
	try
	{
		return _state.at(id); // ないやつ（例外）が来た瞬間落ちられたら困る
	}
	catch (...) // 全ｴﾗｰ
	{
		AST();
		return defData;
	}
}

bool InputState::state(INPUT_ID id , int data)
{
	if (_state.find(id) != _state.end())
	{
		_state[id].first = data;
		return true;
	}
	
	return false;
}

void InputState::SetOld(void)
{
	for (auto id : INPUT_ID())
	{
		try
		{
			_state[id].second = _state[id].first;
		}
		catch(...)
		{
			AST();
			_state.emplace(id, KeyPair{ 0,1 }); // emplaceを使う
		}
	}
}
