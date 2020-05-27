#include <DxLib.h>
#include "KeyState.h"
#include "InputState.h"
#include <_debug\_DebugConOut.h>



KeyState::KeyState()
{
	_KeyConDef.reserve(static_cast<size_t>(end(INPUT_ID()))); // メモリの再配置を避けるため、あらかじめ必要分確保するreserveを使用する
	_KeyConDef.emplace_back(KEY_INPUT_LEFT);
	_KeyConDef.emplace_back(KEY_INPUT_RIGHT);
	_KeyConDef.emplace_back(KEY_INPUT_UP);
	_KeyConDef.emplace_back(KEY_INPUT_DOWN);
	_KeyConDef.emplace_back(KEY_INPUT_Z);
	_KeyConDef.emplace_back(KEY_INPUT_X);
	_KeyConDef.emplace_back(KEY_INPUT_A);
	_KeyConDef.emplace_back(KEY_INPUT_S);

	FILE *file;
	fopen_s(&file, "data/key.data", "rb");
	if (file == nullptr)
	{
		_KeyCon = _KeyConDef; // 中身のｺﾋﾟｰ
	}
	else
	{
		//_keyConには値が入っていないから要素(INPUT_ID分)のサイズを確保しないといけない
		_KeyCon.resize(static_cast<int>(end(INPUT_ID())));
		fread(
			_KeyCon.data(), 
			sizeof(_KeyCon[0]),
			_KeyCon.size(),
			file);
		fclose(file);
	}

	modeKeyOld = 1;	// F1のｵｰﾙﾄﾞｷｰ


	func = &KeyState::RefKeyData;  // 関数ﾎﾟｲﾝﾀを使うときは&を使う　今回はﾒﾝﾊﾞｰ関数ﾎﾟｲﾝﾀ クラスから辿る＝名前空間
}


KeyState::~KeyState()
{
}

void KeyState::Update(void)	
{
	SetOld();
	modeKeyOld = _buf[KEY_INPUT_F1];
	GetHitKeyStateAll(_buf); // _bufはすべてのﾎﾞﾀﾝの押下状態をとる
	(this->*func)(); // 自分自身のﾎﾟｲﾝﾀｰはthis
}

void KeyState::RefKeyData(void)
{
	// _bufに入っている inputIDのｷｰ(enumclass)の情報をstateのsetを使っていれる _buf　inputID分の情報をsetを使って　範囲for文を使う　_keyconを使ってsetする　f1キーを押したらfuncの内容をsetConFigに変える　f1keyを押した瞬間refKeyDataの呼び出しにもどす　トレースで切り替わったら変わったことを表示する
	for (auto id : INPUT_ID())
	{
		state(id, _buf[_KeyCon[static_cast<int>(id)]]);	// <size_t>でも可
	}


	if (_buf[KEY_INPUT_F1] && !modeKeyOld)
	{
		_confID = INPUT_ID::LEFT;	// ｺﾝｽﾄﾗｸﾀでは一回目しか回せない max値が入るため何もしないまま抜ける  
		func = &KeyState::SetKeyConfig;
		TRACE("キーコンフィグ開始\n");
		TRACE("%d%d番目のキー設定\n", static_cast<int>(_confID));
	}
}

void KeyState::SetKeyConfig(void)
{
	if (_buf[KEY_INPUT_F1] && !modeKeyOld)
	{
		func = &KeyState::RefKeyData;
		TRACE("キーコンフィグ終了\n");
		return;
	}

	// ﾌﾟﾚｲﾔｰのｷｰ情報に0を代入
	for (auto key : INPUT_ID())
	{
		state(key, 0);
	}

	// 1度押したｷｰかどうかを調べる
	auto checkKey = [&](int id){
		for (auto ckId = INPUT_ID::LEFT; ckId < _confID; ++ckId)
		{
			if (_KeyCon[static_cast<int>(ckId)] == id)
			{
				return false;	// 重複あり
			}
		}
		return true;	// 重複無し
	};
	

	// ｷｰ変更
	for (int id = 0; id < sizeof(_buf); id++)
	{
		if (!checkKey(id))
		{
			continue;	// コンティニュー先はfor (int id = 0; id < sizeof(_buf); id++)
		}

		if (_buf[id] && (!_buf[KEY_INPUT_F1]))
		{
			_KeyCon[static_cast<int>(_confID)] = id;
			++_confID; // 何を++しているかわからないから前演算をする
			TRACE("%dを設定\n", id);

			if (_confID >= end(INPUT_ID()))
			{
				FILE* file;
				fopen_s(&file, "data/key.data", "wb");
				if (file != nullptr)
				{
					/*for (int j = 0; j < _KeyCon.size(); j++)
					{
						fwrite(
							&_KeyCon[j],
							sizeof(_KeyCon[j]),
							1,
							file
						);
					}*/ // ﾃﾞｰﾀがバラバラにあるときに使う 

					fwrite(
						_KeyCon.data(), // 要素のﾎﾟｲﾝﾀｰを取得してそれを渡す 
						sizeof(_KeyCon[0]),// sizeof(_KeyCon[0]) * _KeyCon.size() , 1 でもよい　ﾃﾞｰﾀが連続で存在している時
						_KeyCon.size(),
						file
					);
					fclose(file);
				}

				TRACE("キーコンフィグ終了\n");
				func = &KeyState::RefKeyData;
				break;
			}


			TRACE("%d%d番目のキー設定\n",
				   static_cast<int>(_confID) + 1,
				   end(INPUT_ID())
				   );
		}
	}
}
