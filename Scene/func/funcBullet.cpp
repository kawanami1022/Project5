#include "funcBullet.h"
#include <Object.\\Bullet.h>
#include <Scene\GameScene.h>
#include <algorithm>

std::map<UNIT_ID, int> FuncBullet::_MaxCount = { {UNIT_ID::PL_BULLET,2},{UNIT_ID::EM_BULLET,2} };

FuncBullet::FuncBullet()
{

}

FuncBullet::~FuncBullet()
{
}

bool FuncBullet::operator()(ActQueT& aQue, void* scene)
{
	/*if (aQue.first == ACT_QUE::SHOT)*/
	{
		UNIT_ID unitID = aQue.second.unitID() == UNIT_ID::PLAYER ? UNIT_ID::PL_BULLET : UNIT_ID::EM_BULLET;
		double speed = unitID == UNIT_ID::PL_BULLET ? -5 : 5;

		if (FuncBullet::_MaxCount[unitID] > std::count_if(((GameScene*)scene)->_objList.begin(), ((GameScene*)scene)->_objList.end(),
			[&](sharedObj& obj)
		{
			return (*obj).unitID() == unitID;
		})
			)
		{
			((GameScene*)scene)->_objList.emplace_back(new Bullet(unitID, aQue.second.pos(), speed));
			return true;
		}
	}
	

	return false;
}
