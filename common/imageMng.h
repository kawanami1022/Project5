#pragma once
#include <memory>
#include <map>
#include <vector>
#include <common\Vector2.h>

using VecInt = std::vector<int>;

#define lpImageMng imageMng::GetInstance()
#define IMAGE_ID(KEY) (imageMng::GetInstance().GetID(KEY))

class imageMng
{
public:
	static imageMng &GetInstance(void)
	{
		return *sInstance;
	}

	const VecInt& GetID(const std::string& key); // 参照渡し
	const VecInt& GetID(const std::string& key , const std::string& fileName);
	const VecInt& GetID(const std::string& key, const std::string& fileName , const Vector2& divSize,const Vector2& divCnt);
	
private:
	struct imageMngDeleter
	{
		void operator()(imageMng* imageMng) const // 一つ目　オーバーロードしたい　二つ目　右辺式何をﾃﾞﾘｰﾄしたいかを右側に書く
		{
			delete imageMng;
		}
	};

	imageMng();
	~imageMng();
	static std::unique_ptr<imageMng, imageMngDeleter> sInstance;
	//デフォルトﾃﾞﾘｰﾀは~imageMng();
	//ｶｽﾀﾑﾃﾞﾘｰﾀｰ　自分が作ったﾃﾞﾘｰﾀｰ

	std::map<std::string, VecInt> imgMap;
};

