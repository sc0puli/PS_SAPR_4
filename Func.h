#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include "colorref.h"

using namespace std;

class Func
{
public:
	vector<double> xList;
	vector<double> yList;
	
	void DrawGraph(HDC hdc, RECT r, double X_AXIS_SCALE, double Y_AXIS_SCALE, int CLR);
	void MarkPoint(HDC hdc, RECT r, double x, double y, double X_AXIS_SCALE, double Y_AXIS_SCALE);

	double max_X();
	double max_Y();
	double min_X();
	double min_Y();
};

void Func::DrawGraph(HDC hdc, RECT r, double X_AXIS_SCALE, double Y_AXIS_SCALE, int CLR)
{
	HPEN pen, old_pen;

	if (CLR % 2 != 0)
	{
		pen = CreatePen(PS_SOLID, 1, GREEN);
		old_pen = (HPEN)SelectObject(hdc, pen);
	}
	else
	{
		pen = CreatePen(PS_SOLID, 1, DARK_RED);
		old_pen = (HPEN)SelectObject(hdc, pen);
	}

	MoveToEx(hdc, (xList[0] * X_AXIS_SCALE) + 20, r.bottom - ((yList[0] - min_Y()) * Y_AXIS_SCALE) - 20, nullptr);
	for (int i = 0; i < xList.size(); i++)
	{
		LineTo(hdc, (xList[i] * X_AXIS_SCALE) + 20, r.bottom - ((yList[i] - min_Y()) * Y_AXIS_SCALE) - 20);
		MoveToEx(hdc, (xList[i] * X_AXIS_SCALE) + 20, r.bottom - ((yList[i] - min_Y()) * Y_AXIS_SCALE) - 20, nullptr);

		if (xList.size() < 10)
		{
			MarkPoint(hdc, r, xList[i], yList[i], X_AXIS_SCALE, Y_AXIS_SCALE);
		}
		else if (i % 10 == 0) 
		{
			MarkPoint(hdc, r, xList[i], yList[i], X_AXIS_SCALE, Y_AXIS_SCALE);
		}
	}

	SelectObject(hdc, old_pen);
	DeleteObject(pen);
}

void Func::MarkPoint(HDC hdc, RECT r, double x, double y, double X_AXIS_SCALE, double Y_AXIS_SCALE)
{
	MoveToEx(hdc, x*X_AXIS_SCALE + 20, r.bottom - 20 - ((y - min_Y()) * Y_AXIS_SCALE) - 6, nullptr);
	LineTo(hdc, x * X_AXIS_SCALE + 20 + 4, r.bottom - 20 - ((y - min_Y()) * Y_AXIS_SCALE));
	LineTo(hdc, x * X_AXIS_SCALE + 20, r.bottom - 20 - ((y - min_Y()) * Y_AXIS_SCALE) + 6);
	LineTo(hdc, x * X_AXIS_SCALE + 20 - 4, r.bottom - 20 - ((y - min_Y()) * Y_AXIS_SCALE));
	LineTo(hdc, x * X_AXIS_SCALE + 20, r.bottom - 20 - ((y - min_Y()) * Y_AXIS_SCALE) - 6);
	MoveToEx(hdc, x * X_AXIS_SCALE + 20, r.bottom - 20 - ((y - min_Y()) * Y_AXIS_SCALE), nullptr);
}

bool comp(double& t1, double& t2)
{
	return t1 > t2;
}

double Func::max_X()
{
	vector<double> temp = xList;
	sort(temp.begin(), temp.end());
	return(temp[temp.size()-1]);
}

double Func::max_Y()
{
	vector<double> temp = yList;
	sort(temp.begin(), temp.end());
	return(temp[temp.size() - 1]);
}


double Func::min_X()
{
	vector<double> temp = xList;
	sort(temp.begin(), temp.end(), comp);
	return(temp[temp.size()-1]);
}

double Func::min_Y()
{
	vector<double> temp = yList;
	sort(temp.begin(), temp.end(), comp);
	return(temp[temp.size()-1]);
}
