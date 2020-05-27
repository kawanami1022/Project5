#pragma once
#include <Scene\BaseScene.h>
#include <map>

class FuncBullet
{
public:
	FuncBullet();
	~FuncBullet();
	bool operator()(ActQueT& aQue, void* scene);
private:
	static std::map<UNIT_ID, int> _MaxCount;

};

