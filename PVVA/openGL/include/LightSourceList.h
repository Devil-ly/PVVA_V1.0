#pragma once
#ifndef LIGHTSOURCELIST_H
#define LIGHTSOURCELIST_H
#include "openGL/include/LightSource.h"

class LightSourceList
{
public:
	LightSourceList() = default;
	~LightSourceList()
	{
		LightSource_Data.clear();
	}

	void addNew(const LightSource & _LightSource) // 增加一个LightSource
	{
		LightSource_Data.push_front(_LightSource);
		//LightSource_Data.
		LightSource_Data.begin()->calReflectPoint();
	}

	LightSource getLightSource(int LightSourceID) const
	{
		return *find(LightSourceID);
	}

	void getAllLightSource(list<modleID> &allLightSource, QStringList &allLightSourceName) // 返回所有面的名字和ID
	{
		for (list<LightSource>::const_iterator iter_find = LightSource_Data.begin();
			iter_find != LightSource_Data.end(); iter_find++)
		{
			modleID modleIDpara;
			modleIDpara.name = LIGHTSOURCE_NAME;
			modleIDpara.ID = iter_find->getID();
			allLightSource.push_back(modleIDpara);
			allLightSourceName.push_back(iter_find->getName());
		}
	}

	void modify(int LightSourceID, const LightSource & _LightSource)
	{
		list<LightSource>::iterator iter;
		for (iter = LightSource_Data.begin(); iter != LightSource_Data.end(); iter++)
		{
			if (LightSourceID == iter->getID())
			{
				*iter = _LightSource;
				iter->calReflectPoint();
				return;
			}
		}
	}

	void erase(int LightSourceID)
	{
		LightSource_Data.erase(find(LightSourceID));
	}

	void draw() const
	{
		if (!LightSource_Data.empty())
		{
			list<LightSource>::const_iterator iter;
			for (iter = LightSource_Data.begin(); iter != LightSource_Data.end(); iter++)
				iter->draw();
		}

	}

	void clear()  // 删除所有lightsource
	{
		LightSource_Data.clear();
	}

	void save(ofstream & savefile) const
	{
		savefile << LightSource_Data.size() << " " << "LightSource" << endl;
		list<LightSource>::const_iterator iter;
		for (iter = LightSource_Data.begin(); iter != LightSource_Data.end(); iter++)
			iter->save(savefile);

	}

	void open(ifstream & openfile)
	{
		LightSource_Data.clear(); //先清除已存数据

		int NumLightSource;
		string temp;
		openfile >> NumLightSource;
		openfile >> temp;
		for (int i = 0; i < NumLightSource; i++)
		{
			LightSource tempLightSource;
			tempLightSource.open(openfile);
			LightSource_Data.push_back(tempLightSource);
		}
	}

	void updateLightSource(SurfaceList const * _SurfaceListPtr)
	{
		list<LightSource>::iterator iter;
		for (iter = LightSource_Data.begin(); iter != LightSource_Data.end(); iter++)
		{
			iter->setSurfaceListPtr(_SurfaceListPtr);
			iter->calReflectPoint();
		}
	}

private:
	list<LightSource> LightSource_Data;

	list<LightSource>::const_iterator find(int LightSourceID) const
	{
		list<LightSource>::const_iterator iter;
		for (iter = LightSource_Data.begin(); iter != LightSource_Data.end(); iter++)
		{
			if (LightSourceID == iter->getID())
				return iter;
		}
	}
	
	//SurfaceList const * SurfaceListPtr;
};
#endif // ! LIGHTSOURCELIST_H
