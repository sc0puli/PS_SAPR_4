#pragma once

#include <fstream>
#include <vector>

using namespace std;

class Plot
{
public:
	Plot();

	pair<vector<float>, vector<float>> VecCoord;
	vector<string> axisName;

	bool readPlot(wchar_t[MAX_PATH]);

private:

};

Plot::Plot()
{
}

bool Plot::readPlot(wchar_t filename[MAX_PATH])
{
	int plotCount = 0;
	vector<string> axisName;
	string axis;

	ifstream in(filename);
	if (in.fail())
	{
		return false;
	}

	in >> plotCount;

	for (size_t i = 0; i < plotCount; i++)
	{
		in >> axis;
		axisName.push_back(axis);
	}

	float xTemp;
	float yTemp;

	do
	{
		in >> xTemp;
		in >> yTemp;
		VecCoord.first.push_back(xTemp);
		VecCoord.second.push_back(yTemp);
	} while (in.eof());

	return true;
}