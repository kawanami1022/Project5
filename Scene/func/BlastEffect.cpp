#include "BlastEffect.h"
#include <Scene\GameScene.h>
#include <EffekseerForDxlib.h>
#include <Scene\SceneMng.h>



bool BlastEffect::operator()(ActQueT & aQue, void * scene)
{
	auto flag = PlayEffekseer2DEffect(((GameScene*)scene)->effectResourceHandle);
	SetPosPlayingEffekseer2DEffect(flag,lpSceneMng.GameScreenOffset.x + aQue.second.pos().x, lpSceneMng.GameScreenOffset.y + aQue.second.pos().y, 0);
	return false;
}
