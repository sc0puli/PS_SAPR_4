#pragma once
#include <fstream>
#include <iostream>
#include "colorref.h"

#include "Func.h"

class FuncCollection
{
public:
	FuncCollection(unsigned int fc, int pc, double x_scale, double y_scale) : 
		FuncCount(fc), pointsCount(pc), X_AXIS_SCALE(x_scale), Y_AXIS_SCALE(y_scale){};
	
	int FuncCount; 

	int pointsCount;

	double X_AXIS_SCALE;
	double Y_AXIS_SCALE;

	vector<Func> func;
	vector<string> axisLable;

	bool ReadExternalGraph(OPENFILENAME);
	void WriteGraphsToExternal(OPENFILENAME);

	void CalculateScales(RECT);

	void DrawFuncs(HDC hdc, RECT r);
	void MaxMinAxis(HDC hdc, RECT r);

	double MinX();
	double MaxX();
	double MinY();
	double MaxY();
};

bool FuncCollection::ReadExternalGraph(OPENFILENAME ofn)
{
	string lable;
	double temppoint;
	vector<double> pointsToAssign;

	fstream instream(ofn.lpstrFile);
	if (!instream.is_open()) return false;

	instream >> FuncCount;
	func.resize(FuncCount);

	for (int i = 0; i < FuncCount + 1; i++)
	{
		instream >> lable;
		axisLable.push_back(lable);
	}

	//Считывание всех точек
	while (!instream.eof())
	{
		instream >> temppoint;
		pointsToAssign.push_back(temppoint);
	}

	instream.close();

	//Распределение точек по функциям
	for (int j = 0; j < (pointsToAssign.size() - 1); j += FuncCount + 1)
	{
		int i = 0;
		for (int n = j + 1; n <= FuncCount + j; n++)
		{
			func[i].xList.push_back(pointsToAssign[j]);
			func[i].yList.push_back(pointsToAssign[n]);
			i++;
		}
	}


	return true;
}

void FuncCollection::WriteGraphsToExternal(OPENFILENAME ofn)
{
	if (FuncCount == 0) return;

	ofstream outstream(ofn.lpstrFile);

	outstream << FuncCount << endl;

	outstream << axisLable[0]; 
	for (int i = 1; i < FuncCount+1; i++)
	{
		outstream << " " << axisLable[i];
	}
	outstream << endl;

	for (int x = 0; x < func[0].xList.size(); x++)
	{
		outstream << abs(func[0].xList[x]) << " ";
		for (int i = 0; i < FuncCount; i++)
		{
			outstream << abs(func[i].yList[x]) << " ";
		}
		
		outstream << endl;
	}
	
	outstream.close();
}

void FuncCollection::CalculateScales(RECT r) //расчет масштабных множителей по осям
{
	double x_scale = 99999999999999999.9;
	double y_scale = 99999999999999999.9;

	for (size_t i = 0; i < FuncCount; i++)
	{
		if (x_scale > (r.right - 40) / (func[i].max_X() - func[i].min_X()))
		{
			x_scale = (r.right - 40) / (func[i].max_X() - func[i].min_X());
		}
	}

	this->X_AXIS_SCALE = x_scale;

	for (size_t i = 0; i < FuncCount; i++)
	{
		if (y_scale > (r.bottom - 40) / (func[i].max_Y() - func[i].min_Y()))
		{
			y_scale = (r.bottom - 40) / (func[i].max_Y() - func[i].min_Y());
		}
	}

	this->Y_AXIS_SCALE = y_scale;
}

void FuncCollection::DrawFuncs(HDC hdc, RECT r)
{
	CalculateScales(r);
	
	for (int i = 0; i < FuncCount; i++)
	{
		func[i].DrawGraph(hdc, r, X_AXIS_SCALE, Y_AXIS_SCALE, i);
	}
	if (FuncCount!=0)
	{
		MaxMinAxis(hdc, r);
	}
}

void FuncCollection::MaxMinAxis(HDC hdc, RECT r)
{
	wchar_t min[256];
	swprintf(min, 256, L"min y: %f", MinY());

	wchar_t max[256];
	swprintf(max, 256, L"max y: %f", MaxY());

	SetBkColor(hdc, RGB(255, 255, 255));
	TextOutW(hdc, r.right / 2, r.top , max, wcslen(max));
	TextOutW(hdc, r.right / 2, r.bottom - 15, min, wcslen(min));
}

double FuncCollection::MinX()
{
	double min = 9999999999999999.9;
	for (int i = 0; i < FuncCount; i++)
	{
		if (min > func[i].min_X())
		{
			min = func[i].min_X();
		}
	}
	return min;
}

double FuncCollection::MinY()
{
	double min = 9999999999999999.9;
	for (int i = 0; i < FuncCount; i++)
	{
		if (min > func[i].min_Y())
		{
			min = func[i].min_Y();
		}
	}
	return min;
}

double FuncCollection::MaxX()
{
	double max = 0;
	for (int i = 0; i < FuncCount; i++)
	{
		if (max < func[i].max_X())
		{
			max = func[i].max_X();
		}
	}
	return max;
}

double FuncCollection::MaxY()
{
	double max = 0;
	for (int i = 0; i < FuncCount; i++)
	{
		if (max < func[i].max_Y())
		{
			max = func[i].max_Y();
		}
	}
	return max;
}
