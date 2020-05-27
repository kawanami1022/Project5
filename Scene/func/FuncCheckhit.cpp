#include "FuncCheckhit.h"
#include <Scene\GameScene.h>

bool FuncCheckHit::operator()(ActQueT & aQue, void * scene)
{
	/*if (aQue.first == ACT_QUE::CHECK_HIT)*/
	{
		UNIT_ID unitID = aQue.second.unitID() == UNIT_ID::PL_BULLET ? UNIT_ID::ENEMY : UNIT_ID::PLAYER;

		for (auto obj : ((GameScene*)scene)->_objList)
		{
			if ((*obj).unitID() == unitID && (*obj).isAlive())
			{
				Vector2Dbl length = abs(aQue.second.pos() - (*obj).pos());
				Vector2Dbl hitLength = (aQue.second.size() + (*obj).size())/2.0;

				if (std::sqrt(length.x*length.x + length.y*length.y) < std::sqrt(hitLength.x*hitLength.x + hitLength.y*hitLength.y))
				/*if (length < hitLength)*/
				{
					(*obj).SetAlive(false);
					aQue.second.SetAlive(false);
					return true;
				}
			}

		}
	}


	return false;
}
