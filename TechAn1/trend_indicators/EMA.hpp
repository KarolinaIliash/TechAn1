#pragma once

#include <vector>

class EMA
{
public:
	EMA(int period_, int daysAmount_) : period(period_), daysAmount(daysAmount_) 
	{
		SMA.assign(daysAmount_, 0);
		prevEMA.assign(daysAmount_, 0);
		hasSMA.assign(daysAmount_, false);
	}

public:

	double calculateEMA(double const& lastPrice, double const& period, double const& prevEMA)
	{
		double weightMult = 2 / (period + 1);
		double EMA = (lastPrice - prevEMA) * weightMult + prevEMA;
		return EMA;
	}


	double calculate(std::vector<double> const& data, int i)
	{
		if (i > daysAmount - 1)
		{
			int count = i % daysAmount;
			sum += data[i];
			sum -= data[i - daysAmount];

			double avg = sum / double(daysAmount);

			if (!hasSMA[count]/*&& (i + 1) % daysAmount == 0*/)
			{
				SMA[count] += avg;

				if ((i + 1) / daysAmount == period)
				{
					SMA[count] /= double(period);
					hasSMA[count] = true;

					prevEMA[count] = SMA[count];

					result.push_back(SMA[count]);
				}
				else
				{
					result.push_back(data[i]);
				}
			}
			else
			{
				double curEMA = calculateEMA(data[i], period, prevEMA[count]);
				prevEMA[count] = curEMA;
				result.push_back(curEMA);
			}
		}
		else
		{
			sum += data[i];
			result.push_back(data[i]);

			if (i == daysAmount - 1)
			{
				SMA[i] += sum / double(daysAmount);
			}
		}

		return result.back();
	}

private:

	int period;
	int daysAmount;
	std::vector<double> result;

	 double sum = 0;
	 std::vector<double> SMA;
	 std::vector<double> prevEMA;
	 std::vector<bool> hasSMA;
};