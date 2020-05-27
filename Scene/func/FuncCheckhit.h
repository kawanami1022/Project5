#pragma once
#include <Scene\BaseScene.h>

struct FuncCheckHit
{
	bool operator()(ActQueT& aQue, void* scene);
};

