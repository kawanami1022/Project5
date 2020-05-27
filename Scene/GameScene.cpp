#include <algorithm>
#include <DxLib.h>
#include <_debug\_DebugConOut.h>
#include "GameScene.h"
#include <common\imageMng.h>
#include <Object\Player.h>
#include <Object\Enemy.h>
#include <Object\Obj.h>
#include <Scene\SceneMng.h>
#include <Object\Bullet.h>
#include <Scene/func/funcBullet.h>
#include <Scene/func/FuncCheckhit.h>
#include <Scene/func/FuncShake.h>
#include <Scene\func\BlastEffect.h>
#include < EffekseerForDXLib.h>
#include <stdio.h>
#include <Scene\TitleScene.h>

GameScene::GameScene()
{
	TRACE("ｹﾞｰﾑｼｰﾝの生成\n");
	lpImageMng.GetID("ｷｬﾗ",    "image/char.png",     { 30,32 }, { 10,10 });
	lpImageMng.GetID("弾",     "image/shot.png",     { 8,3 },   { 1,2 });
	lpImageMng.GetID("敵爆発", "image/en_blast.png", { 64,64 }, { 5,1 });
	lpImageMng.GetID("PL爆発", "image/pl_blast.png", { 64,64 }, { 4,1 });
	
	effectResourceHandle = LoadEffekseerEffect("effect/test_bs.efk", 10.0f);


	_objList.emplace_back(
		new Player({ lpSceneMng.GameScreenSize.x / 2.0 ,lpSceneMng.GameScreenSize.y - 16.0}, { 30,32 })
	);

	FILE* file;
	int enemyNum[50];
	fopen_s(&file,"data/enemyMap.txt", "r");
	if (file != nullptr)
	{
		TRACE("enemy map \n");
		for (int i = 0; i < 50; i++)
		{
			if(i%10 == 0) TRACE("%d\n")
			fscanf_s(file, "%d", &enemyNum[i]);
			TRACE("%d ", enemyNum[i])
		}
		TRACE("\n");
		fclose(file);
	}
	

	for (int y = 0; y < 5; y++)
	{
		for (int x = 0; x < 10; x++)
		{
			if (enemyNum[x + 10 * y] < 0) continue;

			int xPos = (x/5) % 2;
			int yPos = ((x + y * 10)/10) % 3;

			MoveState temMoveState;	// moveで所有権を渡してもﾛｰｶﾙで設定しているから大丈夫
			int typeNum = (enemyNum[x + 10 * y] - 10) / 2;
			TRACE("Enemy type : %d\n", typeNum);
			ENEMY_TYPE enemyType = (ENEMY_TYPE)typeNum;
			
			Vector2Dbl pos[7] = {
				{30.0*(x + y * 10) , 0.0},
				{100.0, 350.0},
				{100.0, 250.0},
				{80.0 + x * (32.0 + 3),30 + 16.0 + y * (32 + 3)},
				{20.0 , 0.0},
				{(80.0 + 9 * (32.0 + 3))/2, (30 + 16.0 + 4 * (32 + 3))/2},
				{0.0, 0.0}
			};
			
			temMoveState.emplace_back(MOVE_TYPE::WAIT, pos[0]);		

			temMoveState.emplace_back(MOVE_TYPE::SIGMOID, pos[1]);

			temMoveState.emplace_back(MOVE_TYPE::SPIRAL, pos[2]);

			temMoveState.emplace_back(MOVE_TYPE::PITIN, pos[3]);

			temMoveState.emplace_back(MOVE_TYPE::LR, pos[4]);

			temMoveState.emplace_back(MOVE_TYPE::EXPEND, pos[5]);

			temMoveState.emplace_back(MOVE_TYPE::ATTACK, pos[6]);

			temMoveState.emplace_back(MOVE_TYPE::PITIN, pos[3]);

			EnemyState data = { enemyType,
			{ - 30 + ((double)lpSceneMng.GameScreenSize.x + 64) * (double)(xPos) , 16 + (double)(yPos)*(((double)lpSceneMng.GameScreenSize.y - 48 - 16) / 2) },
			{ 30,32 } ,
			temMoveState
			};
			_objList.emplace_back(new Enemy(data));

			Enemy::_enemyNum++;
		}		


		
	}
	
	TRACE("Enemy Alive : %d\n", Enemy::_enemyNum);

	initFunc();


	/*EnemyState data = { ENEMY_TYPE::A,{100,100},{0,0} };
	obj[0] = new Player({100,100}, {0,0});
	*/


	/*obj[0] = new Obj("image/char.png", { 0,0 }, 10, 10, 30, 32);
	obj[1] = new Obj("image/char.png", { 100,100 }, 10, 10, 30, 32);*/

	_shakeCount = 0;
}


GameScene::~GameScene()
{
	DeleteEffekseerEffect(effectResourceHandle);
}

unique_Base GameScene::Update(unique_Base own)
{
	lpSceneMng.AddDrawQue({ IMAGE_ID("bg")[0],lpSceneMng.ScreenCenter.x, lpSceneMng.ScreenCenter.y,0.0,0,LAYER::BG,DX_BLENDMODE_NOBLEND,255 });
	auto plObj = std::find_if(_objList.begin(), _objList.end(), [&](sharedObj obj) {return (*obj).unitID() == UNIT_ID::PLAYER; });
	
	

	if (!FadeUpdate())
	{
		for (auto data : _objList)
		{
			(*data).Update(*plObj);
		}
	}

	if ((*plObj)->isDead() || Enemy::_enemyNum <= 0)
	{
		_objList.clear();
		Enemy::_enemyNum = 0;
		EnemyMove::_enemyCnt = 0;
		return std::make_unique<TitleScene>();

	}
	

	/*int sizeObj = (_count / _frame) < _objList.size() ? _count / _frame : _objList.size();

	for (int i = 0; i < sizeObj; i++)
	{
		_objList[i]->Update();
	}

	_count++;*/


	for(auto data : _objList)
	{
		/*data->Draw();*/
		/*if (CheckHitKey(KEY_INPUT_SPACE))
		{
			(*data).SetAlive(false);
		}*/
		
		(*data).Draw();
	}

	/*auto prg = [](sharedObj&obj) {return(*obj).isDead(); };
	prg(*_objList.begin());*/

	auto itr = std::remove_if(
				_objList.begin(),	// ﾁｪｯｸ範囲の開始
				_objList.end(),		// ﾁｪｯｸ範囲の終了
				[](sharedObj& obj) {return(*obj).isDead(); } 	// 死んでるやつを除外 ここは真偽を必ず返す(プレディケート)
			);
	// itr 消したい要素の先頭アドレス
	_objList.erase(itr,_objList.end());

	if (_shakeCount)
	{
		_shakeCount--;
		_screenPos = { rand() % 20 - 10,rand() % 20 - 10 };
		if (!_shakeCount)
		{
			_screenPos = { 0,0 };
		}
	}

	return std::move(own);
}

void GameScene::RunActQue(std::vector<ActQueT> actList)
{
	for (auto aQue : actList)
	{ 
		try
		{
			_funcQue.at(aQue.first)(aQue, this);
		}
		catch (...)
		{
			AST();
		}
	}
}

void GameScene::initFunc(void)
{
	_funcQue[ACT_QUE::SHOT] = FuncBullet();

	_funcQue[ACT_QUE::CHECK_HIT] = FuncCheckHit();

	_funcQue[ACT_QUE::SHAKE] = FuncShake();
	
	_funcQue[ACT_QUE::BLAST] = BlastEffect();
}

