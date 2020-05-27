#pragma once
#include <memory>
#include <Object\Obj.h>

class BaseScene;		// Ãﬂ€ƒ¿≤ÃﬂêÈåæ


enum class ACT_QUE
{
	NON,
	SHOT,
	CHECK_HIT,
	SHAKE,
	BLAST,
	MAX
};

using unique_Base = std::unique_ptr<BaseScene>;
using ActQueT = std::pair<ACT_QUE, Obj&>;



class BaseScene
{
public:
	BaseScene();
	virtual ~BaseScene();
	virtual unique_Base Update(unique_Base own) = 0;
	virtual void RunActQue(std::vector<ActQueT> actList);
	Vector2 _screenPos;
protected:
	void FadeInit(std::string fadeType);
	bool FadeUpdate(void);
private:
	int _fadeScrID;
	int _fadeCount;
	std::string _fadeType;
};

