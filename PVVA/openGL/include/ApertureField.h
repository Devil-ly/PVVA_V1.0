#pragma once
#ifndef APERTUREFIELD_H
#define APERTUREFIELD_H

#include <vector>
#include <fstream>
#include <QString>
#include "openGL/include/GraphTrans.h"

class ApertureField
{
public:
	ApertureField()
	{
		EfieldFile = "";
		HfieldFile = "";
		name = "";
	}
	ApertureField(vector<float> _para, const GraphTrans & _graphTransPara, float _factor, float _phase,
		const QString & _EfieldFile,const QString & _HfieldFile, int _StartNum, int _NumU, int _NumV, 
		int _indexEHfield, const QString & _name)
	{
		para = _para;
		graphTransPara = _graphTransPara;
		factor = _factor;
		phase = _phase;
		EfieldFile = _EfieldFile;
		HfieldFile = _HfieldFile;
		StartNum = _StartNum;
		NumU = _NumU;
		NumV = _NumV;
		indexEHfield = _indexEHfield;
		name = _name;
	}
	~ApertureField()
	{

	}

	void getApertureField(vector<float> &_para, GraphTrans &_graphTransPara, float &_factor,
		float &_phase, QString &_EfieldFile, QString &_HfieldFile, int &_StartNum, int &_NumU,
		int &_NumV, int &_indexEHfield, QString &_name) const
	{
		_para = para;
		_graphTransPara = graphTransPara;
		_factor = factor;
		_phase = phase;
		_EfieldFile = EfieldFile;
		_HfieldFile = HfieldFile;
		_StartNum = StartNum;
		_NumU = NumU;
		_NumV = NumV;
		_indexEHfield = indexEHfield;
		_name = name;
	}

	void setApertureField(vector<float> _para, const GraphTrans & _graphTransPara, float _factor, float _phase,
		const QString & _EfieldFile, const QString & _HfieldFile, int _StartNum, int _NumU, int _NumV,
		int _indexEHfield, const QString & _name)
	{
		para = _para;
		graphTransPara = _graphTransPara;
		factor = _factor;
		phase = _phase;
		EfieldFile = _EfieldFile;
		HfieldFile = _HfieldFile;
		StartNum = _StartNum;
		NumU = _NumU;
		NumV = _NumV;
		indexEHfield = _indexEHfield;
		name = _name;
	}

	QString getName() const
	{
		return name;
	}

	QString EfieldFile, HfieldFile;

	QString name;

private:
	vector<float> para; // ģ�Ͳ���
	GraphTrans graphTransPara; // ��ת����

	float factor;
	float phase;

	int StartNum;
	int NumU, NumV;

	int indexEHfield; // 0��ʾֻ��E 1��ʾֻ��H 2��ʾ����
};

#endif // !APERTUREFIELD_H
