#pragma once

#include <vector>

class SMA
{
public:
	SMA(int period_, int daysAmount_) : period(period_), daysAmount(daysAmount_) 
	{
		SMAdays.assign(daysAmount_, 0);
	}

public:

	double calculate(std::vector<double> const& data, int i)
	{
		if (i > daysAmount - 1)
		{
			sum += data[i];
			sum -= data[i - daysAmount];

			int count = i % daysAmount;

			double avg = sum / double(daysAmount);
			averages.push_back(avg);

			if ((i + 1) / daysAmount >= period)
			{
				SMAdays[count] += avg;

				if ((i + 1) / daysAmount != period)
					SMAdays[count] -= averages[i - daysAmount * period];

				result.push_back(SMAdays[count] / period);
				return SMAdays[count] / period;
			}
			else
			{
				SMAdays[count] += avg;
				result.push_back(data[i]);
				return data[i];
			}

		}
		else
		{
			sum += data[i];

			if (i == daysAmount - 1)
			{
				double avg = sum / double(daysAmount);
				averages.push_back(avg);
				SMAdays[i] += avg;
			}
			result.push_back(data[i]);
			return data[i];
		}
	}

private:
	int period;
	int daysAmount;


	double sum = 0;
	std::vector<double> SMAdays;
	std::vector<double> averages;

	std::vector<double> result;
};