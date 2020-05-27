#include "FuncShake.h"
#include <Scene\GameScene.h>


bool FuncShake::operator()(ActQueT & aQue, void* scene)
{
	((GameScene*)scene)->_shakeCount = 20;
	return false;
}
