#include <Dxlib.h>
#include <Scene\SceneMng.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	/*SceneMng sceneMng;
	sceneMng.Run();*/
	SceneMng::Create();
	SceneMng::GetInstance().Run();
	SceneMng::Destroy();
	return 0;


}

