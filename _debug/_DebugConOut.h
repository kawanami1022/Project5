#pragma once
#ifdef _DEBUG
#include <Windows.h>
#include <memory>


// ﾃﾞﾊﾞｯｸﾞﾒｯｾｰｼﾞ用定義
#define AST() {\
    CHAR ast_mes[256];\
    wsprintf(ast_mes, "%s %d行目\0", __FILE__, __LINE__);\
    MessageBox(0, ast_mes, "ｱｻｰﾄ表示", MB_OK);\
    }

#define TRACE(fmt, ...) printf(fmt, __VA_ARGS__); //fmtは””の中身 ...は％などの可変式

class _DebugConOut
{
public:
	static _DebugConOut& GetInstance(void)
	{
		return *sInstance;
	}

private:
	struct _DebugConOutDeleter
	{
		void operator()(_DebugConOut* _debugConOut) const // 一つ目　オーバーロードしたい　二つ目　右辺式何をﾃﾞﾘｰﾄしたいかを右側に書く
		{
			delete _debugConOut;
		}
	};

	_DebugConOut();
	~_DebugConOut();

	FILE* _debugFp = nullptr;

	static std::unique_ptr<_DebugConOut, _DebugConOutDeleter> sInstance;
};

#else	//_DEBUG
#define AST()
#define TRACE(fmt, ...)

#endif //_DEBUG

