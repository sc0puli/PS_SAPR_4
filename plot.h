#pragma once

#include <fstream>
#include <vector>

using namespace std;

class Plot
{
public:
	Plot(int plotCount) : plotCount(plotCount) {}

	int plotCount;

	vector<float> x;
	vector<float> y;

	vector<string> axisName;

	bool ReadPlot(OPENFILENAME);
	void DrawPlot(HDC hdc);
	void minmax(HDC hdc);
	void Point(HDC hdc);
};

bool Plot::ReadPlot(OPENFILENAME ofn)
{
	string axis;

	ifstream in(ofn.lpstrFile);
	if (in.fail())
	{
		return false;
	}

	in >> plotCount;

	for (size_t i = 0; i < plotCount; i++)
	{
		getline(in, axis);
		axisName.push_back(axis);
	}

	float xTemp;
	float yTemp;

	do
	{
		in >> xTemp;
		in >> yTemp;
		x.push_back(xTemp);
		y.push_back(yTemp);
	} while (!in.eof());

	in.close();
	return true;
}

void Plot::DrawPlot(HDC hdc)
{
	for (int i = 0; i < x.size(); i++)
	{
		MoveToEx(hdc, x[i], y[i], nullptr);
		LineTo(hdc, x[i + 1], y[i + 1]);
	}
}