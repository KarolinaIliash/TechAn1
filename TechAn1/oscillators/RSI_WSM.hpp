#include "RSI.hpp"

#pragma once

class RSI_WSM : public RSI
{
public:
	RSI_WSM(int period_) : RSI(period_) {}
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
				//start data for EMA RSI and WSM RSI
				prevAvgU = avgU;
				prevAvgD = avgD;

				return curRSI;
			}
		}
		else if (i > period)
		{
			double curU = 0; // for EMA and
			double curD = 0; // WSM RSI

			double deltaPrev = data[i - period] - data[i - period - 1];
			if (deltaPrev > 0)
			{
				sumU = sumU - deltaPrev;
			}
			else if (deltaPrev < 0)
			{
				sumD = sumD + deltaPrev;
			}

			double delta = data[i] - data[i - 1];
			if (delta > 0)
			{
				sumU += delta;
				curU = delta;
			}
			else if (delta < 0)
			{
				sumD -= delta;
				curD = -delta;
			}
			////for SMA RSI
			//double avgU = sumU / period;
			//double avgD = sumD / period;
			//double RS = avgU / avgD;

			//double curRSI = 100 - 100 / (1. + RS);
			// EMA and WSM RSI
			double alphaEMA = 2. / (period + 1.);
			double alphaWSM = 1. / period;

			double avgU = alphaWSM * curU + (1 - alphaWSM) * prevAvgU;
			double avgD = alphaWSM * curD + (1 - alphaWSM) * prevAvgD;
			double RS = avgU / avgD;
			double curRSI = 100 - 100 / (1. + RS);
			result.push_back(curRSI);
			prevAvgU = avgU;
			prevAvgD = avgD;

			return curRSI;
		}
		return 0;
	}
private:

	double prevAvgU= -1;
	double prevAvgD = -1;
};
