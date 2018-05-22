#pragma once

#include <vector>
#include <tools/utils.hpp>
#include <tuple>

class ADX
{
public:
	ADX(int period_, int daysAmount_) : period(period_), daysAmount(daysAmount_) {}

public:

	std::tuple<double, double, double> calculate(std::vector<double> const& prices, int i) //adx, pdi, ndi
	{
		double nDi = 0;
		double pDi = 0;
		double resADX = 0; 

		int count = i % daysAmount;

		if (i >= 2 * daysAmount - 1 && (i + 1) % daysAmount == 0)
		{
			curHigh = findMax(prices, i - daysAmount + 1, i + 1);
			curLow = findMin(prices, i - daysAmount + 1, i + 1);
			//calc dm plus
			double DMP = curHigh - prevHigh;
			if (DMP < 0) DMP = 0;

			double DMM = prevLow - curLow;
			if (DMM < 0) DMM = 0;

			double tempDMM = DMM;
			double tempDMP = DMP;

			if (DMP > DMM)
			{
				//DMM = 0;
				tempDMM = 0;
			}
			else if (DMM > DMP)
			{
				tempDMP = 0;
			}
			DMM = tempDMM;
			DMP = tempDMP;
			double TR = std::max(
				std::initializer_list<double>{ curHigh - curLow, abs(curHigh - prices[i - daysAmount]),
				abs(prices[i - daysAmount] - curLow) });

			prevHigh = curHigh;
			prevLow = curLow;

			if (!hasDMPSMA/*&& (i + 1) % daysAmount == 0*/ || !hasDMMSMA)
			{
				sumDMP += DMP;
				sumDMM += DMM;
				sumTR += TR;
				++DM_TR_SMA_index;
				//DMPSMA[count] += sumDMP;
				//DMMSMA[count] += sumDMM;

				//if ((i + 1) / daysAmount == period)
				if (DM_TR_SMA_index == period)
				{
					DMPSMA = sumDMP / double(period);
					hasDMPSMA = true;

					DMP = DMPSMA;
					//prevEMA[count] = SMA[count];
					prevDMP = DMPSMA;


					DMMSMA = sumDMM / double(period);
					hasDMMSMA = true;
					DMM = DMMSMA;
					prevDMM = DMMSMA;

					TRSMA = sumTR / double(period);
					hasTRSMA = true;
					prevTR = TRSMA;
					TR = TRSMA;
				}
			}
			else
			{
				DMP = (prevDMP * (period - 1) + DMP) / period;

				DMM = (prevDMM * (period - 1) + DMM) / period;

				TR = (prevTR * (period - 1) + TR) / period;

				prevDMP = DMP;
				prevDMM = DMM;
				prevTR = TR;
			}
			if (hasDMMSMA)
			{
				double posDI = (DMP / TR) * 100;
				double negDI = (DMM / TR) * 100;

				positiveDI.push_back(posDI);
				negativeDI.push_back(negDI);

				pDi = posDI;
				nDi = nDi;

				double DlDiff = abs(posDI - negDI);
				double DlSum = posDI + negDI;

				double DX = (DlDiff / DlSum) * 100;
				if (!hasDXSMA)//check it, can contain bug
				{
					sumDX += DX;
					++DX_SMA_index;
					//if ((i + 1) / daysAmount == 2 * period + count)
					if (DX_SMA_index == period)
					{
						hasDXSMA = true;

						DXSMA = sumDX / period;

						prevADX = DXSMA;

						ADX_res.push_back(DXSMA);

						resADX = DXSMA;
					}
				}
				else
				{
					double curADX = (prevADX * (period - 1) + DX) / period;
					ADX_res.push_back(curADX);
					prevADX = curADX;
					resADX = curADX;
				}
			}
		}
		else if (i > 2 * daysAmount - 1)
		{
			if (hasDMMSMA)
			{
				negativeDI.push_back(negativeDI.back());
				nDi = negativeDI.back();
			}
			if (hasDMPSMA)
			{
				positiveDI.push_back(positiveDI.back());
				pDi = positiveDI.back();
			}
			if (hasDXSMA)
			{
				ADX_res.push_back(ADX_res.back());
				resADX = ADX_res.back();
			}
		}
		else
		{
			int count = i % daysAmount;
			//sum += prices[i];
			if (prices[i] > firstHigh)
			{
				firstHigh = prices[i];
			}
			if (prices[i] < firstLow)
			{
				firstLow = prices[i];
			}
			if (i == daysAmount - 1)
			{
				prevHigh = firstHigh;
				prevLow = firstLow;
			}
		}

		return std::make_tuple(resADX, pDi, nDi);
	}
private:
	int period;
	int daysAmount;

	std::vector<double> positiveDI;
	std::vector<double> negativeDI;
	std::vector<double> ADX_res;

	int sum = 0;
	double prevADX;

	double prevDMP;
	double prevDMM;
	double prevTR;

	double prevHigh;
	double curHigh;

	double prevLow;
	double curLow;

	double DMPSMA;
	double DMMSMA;
	double TRSMA;
	double DXSMA;

	double firstHigh = INT_MIN;
	double firstLow = INT_MAX;

	bool hasDMPSMA = false;
	bool hasDMMSMA = false;
	bool hasTRSMA = false;
	bool hasDXSMA = false;

	double sumDMP = 0;
	double sumDMM = 0;
	double sumTR = 0;
	double sumDX = 0;

	int DM_TR_SMA_index = 0;
	int DX_SMA_index = 0;
};