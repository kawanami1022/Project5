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
		GameScreenOffset{ScreenCenter - GameScreenSize / 2} // ｺﾝｽﾄﾗｸﾀが走った瞬間、ﾛｯｸがかかる。
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

	// 描画ﾚｲﾔｰや描画モードの初期値を設定する
	LAYER drawLayer  = begin(LAYER());
	int blendMode	 = DX_BLENDMODE_NOBLEND;
	int blendModeNum = 255;

	// Queの描画先を設定
	SetDrawScreen(_layerGID);
	ClsDrawScreen();
	SetDrawBlendMode(blendMode, blendModeNum);

	// ｽﾀｯｸにたまっているQUEを描画する
	for (auto dQue : _drawList)
	{
		double x, y, rad;
		int id;
		LAYER layer;
		int blendModeOld = blendMode;
		int blendModeNumOld = blendModeNum;

		std::tie(id, x, y, rad, std::ignore, layer, blendMode, blendModeNum) = dQue;

		// 取り出したQueの内容が前までとLayerもしくは描画形式が違い場合
		// 一旦、ﾊﾞｯｸﾊﾞｯﾌｧに書き出す
		if ((layer != drawLayer) || (blendModeOld != blendMode))
		{
			// _layerGIDに書いた内容をﾊﾞｯｸﾊﾞｯﾌｧに描画する
			SetDrawScreen(DX_SCREEN_BACK);
			SetDrawBlendMode(blendModeOld, blendModeNumOld);
			auto layPos = ScreenCenter + (*_activeScene)._screenPos;
			DrawRotaGraph(layPos.x, layPos.y, 1.0, 0, _layerGID, true);

			// 次のQueのための初期化し、描画先を一時描画先に設定する
			SetDrawScreen(_layerGID);
			SetDrawBlendMode(blendMode, blendModeNum);
			ClsDrawScreen();
		}

		// Queの内容を描画する
		DrawRotaGraph(
			x,
			y,
			1.0,
			rad,
			id,
			true);
	}

	// for文を抜ける最後の_layerGIDの内容をﾊﾞｯｸﾊﾞｯﾌｧに書き出す
	SetDrawScreen(DX_SCREEN_BACK);
	SetDrawBlendMode(blendMode, blendModeNum);
	DrawRotaGraph(ScreenCenter.x, ScreenCenter.y, 1.0, 0, _layerGID, true);

	DrawEffekseer2D();

	ScreenFlip();

	// 画面に出すためのDraw 

	//for (auto layer : LAYER())
	//{
	//	SetDrawScreen(_screenID[layer]);
	//	ClearDrawScreen();
	//}
	//

	//// ｽﾀｯｸにたまっているQueを描画する
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
	////for (int no = 0; no < _drawList.size(); no++)	// sizeで要素数が取れる
	////{
	////	// _drawList[no];// これでDRAW_QUEをとれる
	////		DrawGraph(
	////			std::get<static_cast<int>(DRAW_QUE::X)>(_drawList[no]),
	////			std::get<static_cast<int>(DRAW_QUE::Y)>(_drawList[no]),
	////			std::get<static_cast<int>(DRAW_QUE::IMAGE)>(_drawList[no]),
	////			true);
	////}
	////{

	////}
	////for(auto dQue = _drawList.begin(); dQue != _drawList.end(); dQue++) // ｲﾃﾚｰﾀはautoを使う
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
		AddDrawQue({IMAGE_ID("枠")[0],400,300,0.0,0,LAYER::UI, DX_BLENDMODE_NOBLEND, 255});
		_activeScene = (*_activeScene).Update(std::move(_activeScene));	// moveを使うことでコピーを作らずに所有権の譲渡ができる
		// スマートポインタとしてわかりやすいのが上　_activeScene->Update();
		UpdateEffekseer2D();
		Draw();
		(*_activeScene).RunActQue(std::move(_actList));
		_frame++;
	}
	
}

bool SceneMng::AddDrawQue(DrawQueT dQue)
{
	

	if (std::get<static_cast<int>(DRAW_QUE::IMAGE)>(dQue) <= 0) 	// std::get<何番目>で取り出す
	{
		// 画像IDが不正なので、追加しない
		return false;
	}
	// Queを追加
	// _drawList.push_back(); 要素を追加するときに使う

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
	//ｼｽﾃﾑ処理
	SetWindowText("kadai5");
	SetGraphMode(ScreenSize.x, ScreenSize.y, 16);				// 800*600ﾄﾞｯﾄ、65536色ﾓｰﾄﾞに設定
	ChangeWindowMode(true);										// true:window  false:ﾌﾙｽｸﾘｰﾝ

	// DirectX11をしようするようにする
	SetUseDirect3DVersion(DX_DIRECT3D_11);



	if (DxLib_Init() == -1)										// DXﾗｲﾌﾞﾗﾘの初期化
	{
		return false;												// DxLibﾗｲﾌﾞﾗﾘ初期化処理
	}

	if (Effekseer_Init(8000) == -1)
	{
		DxLib_End();
		return false;
	}

	// フルスクリーンウインドの切り替えでリソースが消えるのを防ぐ
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	// DXライブラリのデバイスロストしたときのコールバックを設定する
	// ウインドとフルスクリーンの切り替えが発生する場合は必ず実行する
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

	SetDrawScreen(DX_SCREEN_BACK);	// 描画先をﾊﾞｯｸﾊﾞｯﾌｧに設定

	_layerGID = MakeScreen(ScreenSize.x, ScreenSize.y, true);

	/*_screenID.try_emplace(LAYER::BG, MakeScreen(ScreenSize.x, ScreenSize.y, true));
	_screenID.try_emplace(LAYER::CHAR, MakeScreen(ScreenSize.x, ScreenSize.y, true));
	_screenID.try_emplace(LAYER::UI, MakeScreen(ScreenSize.x, ScreenSize.y, true));*/

	// Effekseerに2D描画の設定する
	Effekseer_Set2DSetting(ScreenSize.x, ScreenSize.y);
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);

	_dbgSetup(200);
	lpImageMng.GetID("枠", "image/frame.png");
	lpImageMng.GetID("white", "image/white.png");
	lpImageMng.GetID("black", "image/black.png");
	lpImageMng.GetID("title", "image/title.png");
	lpImageMng.GetID("start", "image/start.png");
	lpImageMng.GetID("bg", "image/bg.png");

	return false;

}
