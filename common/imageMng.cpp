#include <DxLib.h>
#include "imageMng.h"

std::unique_ptr<imageMng, imageMng::imageMngDeleter> imageMng::sInstance(new imageMng());

const VecInt & imageMng::GetID(const std::string & key)
{
	return GetID(key, key);
}

const VecInt & imageMng::GetID(const std::string & key, const std::string & fileName)
{
	if (imgMap.find(key) == imgMap.end())
	{
		imgMap[key].resize(1);
		imgMap[key][0] = LoadGraph(fileName.c_str());  
	}
	return imgMap[key];
}

const VecInt & imageMng::GetID(const std::string & key, const std::string & fileName, const Vector2 & divSize, const Vector2 & divCnt)
{
	if (imgMap.find(key) == imgMap.end())
	{
		/*VecInt tmpData;
		tmpData.resize(divCnt.x * divCnt.y);
		if (LoadDivGraph(fileName.c_str(), divCnt.x * divCnt.y, divCnt.x, divCnt.y, divSize.x, divSize.y, &tmpData[0]) == 0)
		{
			imgMap[key] = std::move(tmpData);
		}*/
	    /*else
	    {
		     return std::move(tmpData);
	    }*/
		//return imgMap[key];
		imgMap[key].resize(divCnt.x * divCnt.y);
		LoadDivGraph(fileName.c_str(),divCnt.x * divCnt.y , divCnt.x, divCnt.y,divSize.x,divSize.y,&imgMap[key][0]);
	}
	return imgMap[key];
}

imageMng::imageMng()
{
}


imageMng::~imageMng()
{
}
