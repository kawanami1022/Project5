#include <Dxlib.h>
#include "SceneMng.h"
#include "GameScene.h"
#include "TitleScene.h"
#include <_debug\_DebugDispOut.h>
#include <common\imageMng.h>
#include <algorithm>
#include <EffekseerForDXLib.h>


SceneMng*  SceneMng ::sInstance = nullptr;

SceneMng::SceneMng() :ScreenSize{ 800,600 },
		ScreenCenter{ ScreenSize / 2 },
		GameScreenSize{ 500,390 },
		GameScreenOffset{ScreenCenter - GameScreenSize / 2} // �ݽ�׸����������u�ԁAۯ���������B
{
	_frame = 0;
}


void SceneMng::Draw(void)
{
	_dbgAddDraw();

	std::sort(_drawList.begin(), _drawList.end(),
	[](DrawQueT dQueA, DrawQueT dQueB)
	{
		int zOrderA, zOrderB;
		LAYER layerA, layerB;
		std::tie(std::ignore, std::ignore, std::ignore, std::ignore, zOrderA, layerA, std::ignore, std::ignore) = dQueA;
		std::tie(std::ignore, std::ignore, std::ignore, std::ignore, zOrderB, layerB, std::ignore, std::ignore) = dQueB;

		/*return layerA == layerB ? zorderA < zorderB : layerA < layerB;*/
		return std::tie(layerA, zOrderA) < std::tie(layerB, zOrderB);
	}
	);
	SetDrawScreen(DX_SCREEN_BACK);
	ClsDrawScreen();

	// �`��ڲ԰��`�惂�[�h�̏����l��ݒ肷��
	LAYER drawLayer  = begin(LAYER());
	int blendMode	 = DX_BLENDMODE_NOBLEND;
	int blendModeNum = 255;

	// Que�̕`����ݒ�
	SetDrawScreen(_layerGID);
	ClsDrawScreen();
	SetDrawBlendMode(blendMode, blendModeNum);

	// �����ɂ��܂��Ă���QUE��`�悷��
	for (auto dQue : _drawList)
	{
		double x, y, rad;
		int id;
		LAYER layer;
		int blendModeOld = blendMode;
		int blendModeNumOld = blendModeNum;

		std::tie(id, x, y, rad, std::ignore, layer, blendMode, blendModeNum) = dQue;

		// ���o����Que�̓��e���O�܂ł�Layer�������͕`��`�����Ⴂ�ꍇ
		// ��U�A�ޯ��ޯ̧�ɏ����o��
		if ((layer != drawLayer) || (blendModeOld != blendMode))
		{
			// _layerGID�ɏ��������e���ޯ��ޯ̧�ɕ`�悷��
			SetDrawScreen(DX_SCREEN_BACK);
			SetDrawBlendMode(blendModeOld, blendModeNumOld);
			auto layPos = ScreenCenter + (*_activeScene)._screenPos;
			DrawRotaGraph(layPos.x, layPos.y, 1.0, 0, _layerGID, true);

			// ����Que�̂��߂̏��������A�`�����ꎞ�`���ɐݒ肷��
			SetDrawScreen(_layerGID);
			SetDrawBlendMode(blendMode, blendModeNum);
			ClsDrawScreen();
		}

		// Que�̓��e��`�悷��
		DrawRotaGraph(
			x,
			y,
			1.0,
			rad,
			id,
			true);
	}

	// for���𔲂���Ō��_layerGID�̓��e���ޯ��ޯ̧�ɏ����o��
	SetDrawScreen(DX_SCREEN_BACK);
	SetDrawBlendMode(blendMode, blendModeNum);
	DrawRotaGraph(ScreenCenter.x, ScreenCenter.y, 1.0, 0, _layerGID, true);

	DrawEffekseer2D();

	ScreenFlip();

	// ��ʂɏo�����߂�Draw 

	//for (auto layer : LAYER())
	//{
	//	SetDrawScreen(_screenID[layer]);
	//	ClearDrawScreen();
	//}
	//

	//// �����ɂ��܂��Ă���Que��`�悷��
	//for (auto dQue : _drawList)
	//{
	//	double x, y , rad;
	//	int id;
	//	LAYER layer;

	//	std::tie(id, x, y, rad, std::ignore, layer) = dQue;

	//	if (_screenID[layer] != GetDrawScreen())
	//	{
	//		SetDrawScreen(_screenID[layer]);
	//	}

	//	DrawRotaGraph(
	//		x,
	//		y,
	//		1.0,
	//		rad,
	//		id,
	//		true);	
	//}
	////for (int no = 0; no < _drawList.size(); no++)	// size�ŗv�f��������
	////{
	////	// _drawList[no];// �����DRAW_QUE���Ƃ��
	////		DrawGraph(
	////			std::get<static_cast<int>(DRAW_QUE::X)>(_drawList[no]),
	////			std::get<static_cast<int>(DRAW_QUE::Y)>(_drawList[no]),
	////			std::get<static_cast<int>(DRAW_QUE::IMAGE)>(_drawList[no]),
	////			true);
	////}
	////{

	////}
	////for(auto dQue = _drawList.begin(); dQue != _drawList.end(); dQue++) // ��ڰ���auto���g��
	////{
	////	DrawGraph(
	////	    std::get<static_cast<int>(DRAW_QUE::X)>(*dQue),
	////		std::get<static_cast<int>(DRAW_QUE::Y)>(*dQue),
	////		std::get<static_cast<int>(DRAW_QUE::IMAGE)>(*dQue),
	////		true);
	////}
	//SetDrawScreen(DX_SCREEN_BACK);
	//ClsDrawScreen();

	//auto layPos = ScreenCenter + (*_activeScene)._screenPos;
	//
	//DrawRotaGraph(ScreenCenter.x, ScreenCenter.y, 1.0, 0, _screenID[LAYER::BG], true);
	//DrawRotaGraph(layPos.x, layPos.y, 1.0, 0, _screenID[LAYER::CHAR], true);
	//DrawRotaGraph(ScreenCenter.x, ScreenCenter.y, 1.0, 0, _screenID[LAYER::UI], true);

	//ScreenFlip();

}

SceneMng::~SceneMng()
{

}

void SceneMng::Run(void)
{
	SysInit();
	_activeScene = std::make_unique<TitleScene>();

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		_dbgStartDraw();
		_drawList.clear();
		AddDrawQue({IMAGE_ID("�g")[0],400,300,0.0,0,LAYER::UI, DX_BLENDMODE_NOBLEND, 255});
		_activeScene = (*_activeScene).Update(std::move(_activeScene));	// move���g�����ƂŃR�s�[����炸�ɏ��L���̏��n���ł���
		// �X�}�[�g�|�C���^�Ƃ��Ă킩��₷���̂���@_activeScene->Update();
		UpdateEffekseer2D();
		Draw();
		(*_activeScene).RunActQue(std::move(_actList));
		_frame++;
	}
	
}

bool SceneMng::AddDrawQue(DrawQueT dQue)
{
	

	if (std::get<static_cast<int>(DRAW_QUE::IMAGE)>(dQue) <= 0) 	// std::get<���Ԗ�>�Ŏ��o��
	{
		// �摜ID���s���Ȃ̂ŁA�ǉ����Ȃ�
		return false;
	}
	// Que��ǉ�
	// _drawList.push_back(); �v�f��ǉ�����Ƃ��Ɏg��

	_drawList.emplace_back(dQue);
	return true;

	
}

bool SceneMng::AddActQue(ActQueT aQue)
{
	_actList.emplace_back(aQue);
	return true;
}

const int SceneMng::frame(void)
{
	return _frame;
}

bool SceneMng::SysInit(void)
{
	//���я���
	SetWindowText("kadai5");
	SetGraphMode(ScreenSize.x, ScreenSize.y, 16);				// 800*600�ޯāA65536�FӰ�ނɐݒ�
	ChangeWindowMode(true);										// true:window  false:�ٽ�ذ�

	// DirectX11�����悤����悤�ɂ���
	SetUseDirect3DVersion(DX_DIRECT3D_11);



	if (DxLib_Init() == -1)										// DXײ���؂̏�����
	{
		return false;												// DxLibײ���؏���������
	}

	if (Effekseer_Init(8000) == -1)
	{
		DxLib_End();
		return false;
	}

	// �t���X�N���[���E�C���h�̐؂�ւ��Ń��\�[�X��������̂�h��
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	// DX���C�u�����̃f�o�C�X���X�g�����Ƃ��̃R�[���o�b�N��ݒ肷��
	// �E�C���h�ƃt���X�N���[���̐؂�ւ�����������ꍇ�͕K�����s����
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

	SetDrawScreen(DX_SCREEN_BACK);	// �`�����ޯ��ޯ̧�ɐݒ�

	_layerGID = MakeScreen(ScreenSize.x, ScreenSize.y, true);

	/*_screenID.try_emplace(LAYER::BG, MakeScreen(ScreenSize.x, ScreenSize.y, true));
	_screenID.try_emplace(LAYER::CHAR, MakeScreen(ScreenSize.x, ScreenSize.y, true));
	_screenID.try_emplace(LAYER::UI, MakeScreen(ScreenSize.x, ScreenSize.y, true));*/

	// Effekseer��2D�`��̐ݒ肷��
	Effekseer_Set2DSetting(ScreenSize.x, ScreenSize.y);
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);

	_dbgSetup(200);
	lpImageMng.GetID("�g", "image/frame.png");
	lpImageMng.GetID("white", "image/white.png");
	lpImageMng.GetID("black", "image/black.png");
	lpImageMng.GetID("title", "image/title.png");
	lpImageMng.GetID("start", "image/start.png");
	lpImageMng.GetID("bg", "image/bg.png");

	return false;

}
