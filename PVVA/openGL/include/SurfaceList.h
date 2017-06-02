#pragma once
#ifndef SURFACELIST_H
#define SURFACELIST_H

#include "openGL/include/Surface.h"
#include <list>
#include "tool/modleID.h"
#include <fstream>
#include <QStringList>
using namespace std;

class SurfaceList
{
public:
	SurfaceList() = default;
	~SurfaceList()
	{
		Surface_Data.clear();
	}

	void push_back(const Surface &_surface) // 增加一个Surface
	{
		Surface_Data.push_back(_surface);
	}

	Surface const *  getSurfaceData(modleID paraModleID) const //返回一个surface的指针
	{
		return &*findSurface(paraModleID);
	}

	void ModifySurfaceData(modleID paraModleID, Surface _surface)
	{
		list<Surface>::iterator iter;
		for (iter = Surface_Data.begin(); iter != Surface_Data.end(); iter++)
		{
			if (paraModleID.ID == iter->getModleID().ID &&
				paraModleID.name == iter->getModleID().name)
			{
				*iter = _surface;
				return;
			}
		}
	}

	void erase(modleID paraModleID)
	{
		Surface_Data.erase(findSurface(paraModleID));	
	}

	void draw()
	{
		if (!Surface_Data.empty())
		{
			list<Surface>::const_iterator iter;
			for (iter = Surface_Data.begin(); iter != Surface_Data.end(); iter++)
				iter->draw();
		}
		
	}

	void getAllSurface(list<modleID> &allSurfaceID, QStringList &allSurfaceName) // 返回所有面的名字和ID
	{
		for (list<Surface>::const_iterator iter_find = Surface_Data.begin();
			iter_find != Surface_Data.end(); iter_find++)
		{
			allSurfaceID.push_back(iter_find->getModleID());
			allSurfaceName.push_back(iter_find->getName());
		}	
	}

	void getAllRectangle(list<modleID> &allRectangleID, QStringList &allRectangleName) // 返回所有面的名字和ID
	{
		for (list<Surface>::const_iterator iter_find = Surface_Data.begin();
			iter_find != Surface_Data.end(); iter_find++)
			if (iter_find->getModleID().name == RECTANGLE_NAME)
			{
				allRectangleID.push_back(iter_find->getModleID());
				allRectangleName.push_back(iter_find->getName());
			}
		
	}

	void clear() // 删除所有的surface
	{
		Surface_Data.clear();
	}

	void save(ofstream & savefile)
	{
		savefile << Surface_Data.size() << " " << "Surfaces" << endl;
		list<Surface>::const_iterator iter;
		for (iter = Surface_Data.begin(); iter != Surface_Data.end(); iter++)
			iter->save(savefile);
		
	}

	void open(ifstream & openfile)
	{
		Surface_Data.clear(); //先清除已存数据

		int NumSurface;
		string temp;
		openfile >> NumSurface;
		openfile >> temp;
		for (int i = 0; i < NumSurface; i++)
		{
			Surface tempSurface;
			tempSurface.open(openfile);
			Surface_Data.push_back(tempSurface);
		}
	}
	
private:

	list<Surface>::const_iterator findSurface(modleID paraModleID) const
	{
		list<Surface>::const_iterator iter;
		for (iter = Surface_Data.begin(); iter != Surface_Data.end(); iter++)
		{
			if (paraModleID.ID == iter->getModleID().ID &&
				paraModleID.name == iter->getModleID().name)
				return iter;		
		}
	}

	list<Surface> Surface_Data;
};

#endif // !SURFACELIST_H
