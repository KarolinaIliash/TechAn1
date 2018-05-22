#pragma once

#include <vector>

#include "EMA.hpp"
#include "SMA.hpp"

class Dispersion
{
public:
	Dispersion(int period_, int daysAmount_, bool isEMA_) : period(period_), daysAmount(daysAmount_), isEMA(isEMA_), 
															ema(period_, daysAmount_), sma(period_, daysAmount_)
	{
		prevEMA.assign(daysAmount, 0);
		SMA_vec.assign(daysAmount, 0);
		hasSMA.assign(daysAmount, false);
		prevDispersion.assign(daysAmount, 0);
		dispersion.assign(daysAmount, 0);
	}

public:
	double calculate(std::vector<double> const& data, int i)
	{
		int count = i % daysAmount;

		if (isEMA)
			MA.push_back(ema.calculate(data, i));
		else
			MA.push_back(sma.calculate(data, i));


		if (i > daysAmount - 1)
		{
			sum += data[i];
			sum -= data[i - daysAmount];

			double avg = sum / double(daysAmount);
			averages.push_back(avg);

			if ((i + 1) / daysAmount >= period)
			{
				dispersion[count] += pow(avg - MA[i], 2);

				if ((i + 1) / daysAmount != period)
					dispersion[count] -= pow(averages[i - daysAmount * period] - MA[i - daysAmount * period], 2);

				return dispersion[count] / (period - 1);
			}
			else
			{
				dispersion[count] += pow(avg - MA[i], 2);
				return 0;
			}

		}
		else // we can't calculate average yet
		{
			sum += data[i];

			if (i == daysAmount - 1)
			{
				double avg = sum / double(daysAmount);
				averages.push_back(avg);
				dispersion[i] += pow(avg - MA[i], 2);
			}
			return 0;
		}
	}


	std::vector<double> getMA() { return MA; }
private:
	int daysAmount;
	int period;

	//std::vector<double> dispersion;

	std::vector<double> MA;

	double sum = 0;
	std::vector<double> SMA_vec;
	std::vector<double> prevEMA;
	std::vector<bool> hasSMA;

	bool isEMA = false;

	EMA ema;
	SMA sma;

//	double sum = 0;
	std::vector<double> prevDispersion;
	std::vector<double> dispersion;
	std::vector<double> averages;
};