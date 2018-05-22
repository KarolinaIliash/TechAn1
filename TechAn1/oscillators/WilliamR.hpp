#pragma once

#include <vector>
#include <tools/utils.hpp>

class WilliamR
{
public:
	WilliamR(int period_) : period(period_) {}

public:
	double calculate(std::vector<double> const& prices, int i)
	{
		if (i >= period)
		{
			double high = findMax(prices, i - period + 1, i + 1);

			double low = findMin(prices, i - period + 1, i + 1);

			double curR = ((high - prices[i]) / (high - low)) * -100;

			R.push_back(curR);

			return curR;
		}
	}

private:

	int period;
	std::vector<double> R;
};