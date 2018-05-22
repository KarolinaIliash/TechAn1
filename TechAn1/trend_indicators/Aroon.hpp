#pragma once

#include <vector>
#include <tools/utils.hpp>

class Aroon
{
public:
	Aroon(int period_) : period(period_) {}

public:

	std::pair<double, double> calculate(std::vector<double> const& data, int i) //return up and down
	{
		std::pair<double, double> res(0, 0);

		if (i >= period)
		{
			double curAroonUp = ((period - (i - findMaxIndex(data, i - period, i))) / period) * 100;
			double curAroonDown = ((period - (i - findMinIndex(data, i - period, i))) / period) * 100;

			AroonUp.push_back(curAroonUp);
			AroonDown.push_back(curAroonDown);

			res.first = curAroonUp;
			res.second = curAroonDown;
		}

		return res;
	}

private:
	int period;
	std::vector<double> AroonUp;
	std::vector<double> AroonDown;
};