#pragma once

#include <vector>
#include <tools/utils.hpp>

class Stochastic
{
public:
	Stochastic(int period_, int dSmoothPeriod_) : period(period_), dSmoothPeriod(dSmoothPeriod_) {}

public:
	std::pair<double, double> calculate(std::vector<double> const& prices, int i) //returns last K and D
	{
		std::pair<double, double> result(0, 0);

		if (i >= period)
		{
			double low = findMin(prices, i - period + 1, i + 1);
			double high = findMax(prices, i - period + 1, i + 1);

			double curK = 100 * (prices[i] - low) / (high - low);

			K.push_back(curK);
			result.first = curK;

			if (i > period + dSmoothPeriod - 1)
			{
				sum += curK;
				sum -= K[i - dSmoothPeriod - period];

				double curD = sum / dSmoothPeriod;

				D.push_back(curD);
				result.second = curD;
			}

			else
			{
				sum += curK;
				if (i == period + dSmoothPeriod - 1)
				{
					double curD = sum / dSmoothPeriod;
					D.push_back(curD);
					result.second = curD;
				}
			}
		}

		return result;
	}

private:
	int period;
	int dSmoothPeriod = 3;

	double sum = 0;

	std::vector<double> D;
	std::vector<double> K;
};