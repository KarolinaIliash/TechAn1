#pragma once

#include "RSI.hpp"

class RSI_SMA : public RSI
{
public:
	RSI_SMA(int period_) : RSI(period_) {}
public:
	virtual double calculate(std::vector<double> const& data, int i) override
	{
		if (i > 0 && i < period + 1)
		{
			double delta = data[i] - data[i - 1];
			if (delta > 0)
			{
				sumU += delta; // for SMA RSI
			}
			else if (delta < 0)
			{
				sumD += (-delta); // for SMA RSI
			}

			if (i == period)
			{
				double avgU = sumU / period;
				double avgD = sumD / period;
				double RS = avgU / avgD;

				double curRSI = 100 - 100 / (1. + RS);
				result.push_back(curRSI);

				return curRSI;
			}
		}
		else if (i > period)
		{
			double deltaPrev = data[i - period] - data[i - period - 1];
			if (deltaPrev > 0)
			{
				sumU = sumU - deltaPrev;
			}
			else if (deltaPrev < 0)
			{
				sumD = sumD + deltaPrev;
			}

			//for SMA RSI
			double avgU = sumU / period;
			double avgD = sumD / period;
			double RS = avgU / avgD;

			double curRSI = 100 - 100 / (1. + RS);
			result.push_back(curRSI);

			return curRSI;
		}
		return 0;
	}

};