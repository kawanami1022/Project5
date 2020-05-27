#include <memory>
#include "TitleScene.h"
#include "GameScene.h"
#include "BaseScene.h"
#include <Scene\SceneMng.h>
#include <common\imageMng.h>
#include <DxLib.h>


TitleScene::TitleScene()
{
}


TitleScene::~TitleScene()
{
}

unique_Base TitleScene::Update(unique_Base own)
{
	lpSceneMng.AddDrawQue({IMAGE_ID("title")[0],lpSceneMng.ScreenCenter.x,lpSceneMng.ScreenCenter.y,0.0,0,LAYER::UI,DX_BLENDMODE_NOBLEND, 255 });

	if ((lpSceneMng.frame() / 30) % 2 == 0)
	{
		lpSceneMng.AddDrawQue({ IMAGE_ID("start")[0],lpSceneMng.ScreenCenter.x ,lpSceneMng.ScreenCenter.y + 100, 0.0,0,LAYER::UI,DX_BLENDMODE_NOBLEND, 255 });
	}

	if (CheckHitKey(KEY_INPUT_SPACE))
	{
		return std::make_unique<GameScene>();
	}

	return std::move(own);
}
