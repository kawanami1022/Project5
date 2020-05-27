#pragma once
#include <vector>
#include "BaseScene.h"
#include "Object\Obj.h"
#include <functional>
#include <map>

struct FuncShake;
struct FuncCheckHit;
struct FuncBullet;
struct BlastEffect;

using FuncAct = std::function<bool(ActQueT&, void* scene) >;

class GameScene :
	public BaseScene
{
public:
	GameScene();
	~GameScene();
	unique_Base Update(unique_Base own) override;
	void RunActQue(std::vector<ActQueT> actList) override;
private:
	friend FuncCheckHit;
	friend FuncBullet;
	friend FuncShake;
	friend BlastEffect;
	std::vector<sharedObj> _objList;
	std::map<ACT_QUE, FuncAct> _funcQue;
	void initFunc(void);
	int _shakeCount;
	int effectResourceHandle;

};

