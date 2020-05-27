#pragma once
#include <memory>
#include <tuple> 
#include <common\Vector2.h>
#include "BaseScene.h"
#include <vector>
#include <map>
#include <Scene\LAYER.h>

enum class DRAW_QUE // enum�͌^���B�� class�������class�������O��Ԃ�����X�Ƃ������O�������ł��Ⴄ���̂ɂȂ� 
{
	IMAGE,
	X,
	Y,
	RAD,
	ZORDER,			// ڲ԰���̕`�揇(���l�̒Ⴂ������)
	LAYER,			// ������ID����
	DRAW_MODE,		// ��ʃ��[�h
	DRAW_NUM		// ��ʃ��[�h�ɓn���l
};



using DrawQueT = std::tuple<int, double, double, double, int, LAYER, int, int>; // 4��,��]�p

#define lpSceneMng SceneMng::GetInstance()



class SceneMng
{
public:
	static SceneMng &GetInstance(void)
	{
		Create();
		return *sInstance;
	}

	static void Create()
	{
		if (sInstance == nullptr)
		{
			sInstance = new SceneMng();
		}
	}

	static void Destroy()
	{
		if (sInstance != nullptr)
		{
			delete sInstance;
		}
		sInstance = nullptr; // ��ذĂ��Ă��Ђ��̂��邩��null�������
	}

	void Run(void);

	// std::tuple<int, int, int> data1;
	// std::pair<int, double> data2;

	bool AddDrawQue(DrawQueT dQue);
	bool AddActQue(ActQueT aQue);

	const Vector2 ScreenSize;
	const Vector2 ScreenCenter;
	const Vector2 GameScreenSize;
	const Vector2 GameScreenOffset;
	const int frame(void);
private:
	static SceneMng* sInstance;

    unique_Base _activeScene;

	void Draw(void);
	int _layerGID;
	/*std::map<LAYER,int> _screenID;*/
	std::vector<DrawQueT> _drawList;
	std::vector<ActQueT> _actList;

	int _frame;

	~SceneMng();
	SceneMng();
	bool SysInit(void);
};
