#include <common\imageMng.h>
#include "Player.h"
#include <Scene\GameScene.h>
#include <Scene\SceneMng.h>
#include <Input\KeyState.h>
#include <DxLib.h>

Player::Player()
{
	Init();
}

Player::Player(Vector2Dbl pos, Vector2Dbl size)
{
	_pos = pos;
	_size = size;
	_zoom = 3;
	_unitID = UNIT_ID::PLAYER;
	_glowID[0] = MakeScreen(_zoom * _size.x, _zoom * _size.y, true);
	Init();
}


void Player::Update(sharedObj obj)
{
	if (DestroyPrpc())
	{
		return;
	}



	// (*_input).Update(); .ÇÕíÜÅ@->ÇÕäO
	_input->Update();
	/*if (_input->state(INPUT_ID::LEFT).first)
	{
		_pos.x -= 2;
	}*/

	auto move = [](std::weak_ptr<InputState> keyData, INPUT_ID id, double& pNum, const int speed) {
		// keyData.expired() èIÇÌÇ¡ÇƒÇ¢ÇÈÇ©Ç¢Ç»Ç¢Ç©
		if (!keyData.expired())
		{
			if((*keyData.lock()).state(id).first)
			{
				pNum += speed;
            }
		}	
	};

	move(_input, INPUT_ID::LEFT,  _pos.x, -5);
	move(_input, INPUT_ID::RIGHT, _pos.x,  5);
	move(_input, INPUT_ID::UP,    _pos.y, -5);
	move(_input, INPUT_ID::DOWN,  _pos.y,  5);

	
	{
		if ((*_input).state(INPUT_ID::BTN_1).first && !(*_input).state(INPUT_ID::BTN_1).second)
		{
			lpSceneMng.AddActQue({ ACT_QUE::SHOT , *this });
		}
	}
	

	if (_pos.x <= (_size.x / 2)) _pos.x = (_size.x / 2) ;
	if (_pos.y <= _size.y / 2 ) _pos.y = _size.y / 2;
	if (_pos.x > (lpSceneMng.GameScreenSize.x - _size.x / 2)) _pos.x = (lpSceneMng.GameScreenSize.x - _size.x / 2);
	if (_pos.y > (lpSceneMng.GameScreenSize.y - _size.y / 2)) _pos.y = (lpSceneMng.GameScreenSize.y - _size.y / 2);

	/*lpSceneMng.AddActQue({ ACT_QUE::CHECK_HIT, *this });*/
}




Player::~Player()
{
}

void Player::Draw(void)
{
	Obj::Draw();

	SetDrawScreen(_glowID[0]);
	ClearDrawScreen();
	SetDrawBright(0, 200 + rand() % 55, 0);
	DrawRotaGraph((_zoom / 2)*_size.x, (_zoom / 2)* _size.y, 2, 0, _animMap[_state][_animFrame].first, true);
	SetDrawBright(255, 255, 255);
	GraphFilter(_glowID[0], DX_GRAPH_FILTER_GAUSS, 5, 500);
	lpSceneMng.AddDrawQue({_glowID[0], lpSceneMng.GameScreenOffset.x + _pos.x,lpSceneMng.GameScreenOffset.y + _pos.y - 5, _rad, _zOrder - 1, LAYER::CHAR, DX_BLENDMODE_ADD, 255 });
}

void Player::Init(void)
{
	AnimVector data;
	data.emplace_back(IMAGE_ID("∑¨◊")[0], 30);
	data.emplace_back(IMAGE_ID("∑¨◊")[1], 60);
	SetAnim(STATE::NORMAL, data);

	data.reserve(1);
	data.emplace_back(IMAGE_ID("∑¨◊")[2], 60);
	SetAnim(STATE::EX, data);


	data.emplace_back(IMAGE_ID("PLîöî≠")[0], 20);
	data.emplace_back(IMAGE_ID("PLîöî≠")[1], 25);
	data.emplace_back(IMAGE_ID("PLîöî≠")[2], 30);
	data.emplace_back(IMAGE_ID("PLîöî≠")[3], 35);
	data.emplace_back(-1, 40);
	SetAnim(STATE::DETH, data);

	//_input = ìnÇµÇΩÇ¢Ç‡ÇÃ
	//_input (ìnÇµÇΩÇ¢Ç‡ÇÃ);
	//_input.íáâÓêl(ìnÇµÇΩÇ¢Ç‡ÇÃ)

	_input = std::make_shared<KeyState>();
	// _input.reset(new KeyState()); 

	state(STATE::NORMAL);

}
