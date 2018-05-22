#pragma once

#include <vector>
#include "Dispersion.hpp"

enum TrendDirection
{
	UP,
	WARNING_UP,
	LiGHT_WARNING_UP,
	NON,
	LIGHT_WARNING_DOWN,
	WARNING_DOWN,
	DOWN
};

enum Smooth
{
	NO,
	SMASmooth,
	EMASmooth
};

class CustomTrendDirection
{

public:
	CustomTrendDirection(int period_, Smooth smooth_, int daysAmount_, int deltaPeriod_, int lookBackPeriod_, bool isEma,
		double multBiggest_, double multMiddle_, double multSmallest_) :
		period(period_), smooth(smooth_), daysAmount(daysAmount_),
		sma(period_, daysAmount_), ema(period_, daysAmount_),
		deltaPeriod(deltaPeriod_), lookBackPeriod(lookBackPeriod_), smaDisp(deltaPeriod_, 1),
		disp(deltaPeriod_, 1, isEma), multBiggest(multBiggest_), multMiddle(multMiddle_), multSmallest(multSmallest_)
	{
		if (smooth_ == Smooth::NO)
		{
			daysAmount = 1;
			period = 1;
		}

		SMA_vec.assign(daysAmount, 0);
		prevEMA.assign(daysAmount, 0);
		hasSMA.assign(daysAmount, false);
	}

public:

	TrendDirection calculate(std::vector<double> const& prices, int i)
	{
		TrendDirection res = NON;

		if (smooth == Smooth::NO)
		{
			data.push_back(prices[i]);
		}

		else
		{
			if (smooth = Smooth::SMASmooth)
			{
				data.push_back(sma.calculate(prices, i));
			}
			else if (smooth = Smooth::EMASmooth)
			{
				data.push_back(ema.calculate(prices, i));
			}
		}

		double disp_ = disp.calculate(data, i);

		dispersion.push_back(sqrt(disp_));

		if (i > daysAmount * period - 1 && i > lookBackPeriod - 1) //+ lookBackPeriod)//(i > 0)
		{
			//if (data[i] - dataSMA[i - 1] >= dispersion[i - 1])
			//if (data[i] - dataSMA[i - 1] >= dispersion[i - 1] * multMiddle)
			//if (data[i] - disp.getSMA_vec[i - 1] >= dispersion[i] * multMiddle)
			if (data[i] - disp.getMA()[i] >= dispersion[i] * multMiddle)
			{
				bool everyMoreThenHalfOfSigma = true;
				bool trendContinue = true;
				bool trendReverse = false;

				// if (!direction.empty() && direction.back() != TrendDirection::NON && direction.back() != TrendDirection::WARNING_UP_NEED_CHECK)
				for (int k = 1; k <= lookBackPeriod; k++)
				{
					//if (data[i - k] - dataSMA[i - k - 1] < dispersion[i - 1] * multSmallest)
					//if (data[i - k] - disp.getSMA_vec[i - k - 1] < dispersion[i] * multSmallest)
					if (data[i - k] - disp.getMA()[i - k] < dispersion[i - k] * multSmallest)
					{
						everyMoreThenHalfOfSigma = false;
						break;
					}
				}
				/*else
				{
				if (!direction.empty())
				{
				if (direction.back() == TrendDirection::UP || direction.back() == TrendDirection::WARNING_UP) trendContinue = true;
				else if(direction.back() == TrendDirection::)
				}
				}*/

				if (everyMoreThenHalfOfSigma)
				{
					//if (data[i] - dataSMA[i - 1] >= /*2 **/ dispersion[i - 1] * multBiggest)
					//if (data[i] - disp.getSMA_vec[i - 1] >= /*2 **/ dispersion[i] * multBiggest)
					if (data[i] - disp.getMA()[i] >=  dispersion[i] * multBiggest)
					{
						direction.push_back(TrendDirection::UP);
						res = TrendDirection::UP;
					}
					else
					{
						direction.push_back(TrendDirection::WARNING_UP);
						res = TrendDirection::WARNING_UP;
					}
				}
				else
				{
					direction.push_back(TrendDirection::LiGHT_WARNING_UP);
					res = TrendDirection::LiGHT_WARNING_UP;
				}
			}

			//			else if (data[i] - dataSMA[i - 1] <= -dispersion[i - 1] * multMiddle)
			//else if (data[i] - disp.getSMA_vec[i - 1] <= -dispersion[i] * multMiddle)
			else if (data[i] - disp.getMA()[i] <= -dispersion[i] * multMiddle)
			{
				bool everyLessThenHalfOfSigma = true;

				//if (!direction.empty() && direction.back() != TrendDirection::NON)
				for (int k = 1; k <= lookBackPeriod; k++)
				{
					//if (data[i - k] - dataSMA[i - k - 1] > -dispersion[i - 1]/* / 2.*/ * multSmallest)
					//if (data[i - k] - disp.getSMA_vec[i - k - 1] > -dispersion[i]/* / 2.*/ * multSmallest)
					if (data[i - k] - disp.getMA()[i - k] > -dispersion[i - k]/* / 2.*/ * multSmallest)
					{
						everyLessThenHalfOfSigma = false;
						break;
					}
				}

				if (everyLessThenHalfOfSigma)
				{
					//if (data[i] - dataSMA[i - 1] <= -2 * dispersion[i - 1] * multBiggest)
					//if (data[i] - disp.getSMA_vec[i - 1] <= -2 * dispersion[i] * multBiggest)
					if (data[i] - disp.getMA()[i] <= -2 * dispersion[i] * multBiggest)
					{
						direction.push_back(TrendDirection::DOWN);
						res = TrendDirection::DOWN;
					}
					else
					{
						direction.push_back(TrendDirection::WARNING_DOWN);
						res = TrendDirection::WARNING_DOWN;
					}
				}
				else
				{
					direction.push_back(TrendDirection::LIGHT_WARNING_DOWN);
					res = TrendDirection::LIGHT_WARNING_DOWN;
				}
			}
			else
			{
				direction.push_back(TrendDirection::NON);
				res = TrendDirection::NON;
			}
		}

		/*if (dispersion.size() - direction.size() > 3)
		{
			throw std::exception("Sth went wrong");
		}*/
		return res;
	
	}
	
private:

	int deltaPeriod;
	int lookBackPeriod;

	int period;
	int daysAmount = 1;
	Smooth smooth;

	std::vector<TrendDirection> direction;

	std::vector<double> dispersion;

	double sum = 0;
	std::vector<double> SMA_vec;
	std::vector<double> prevEMA;
	std::vector<bool> hasSMA;

	double multBiggest;
	double multSmallest;
	double multMiddle;

	std::vector<double> data;

	//std::vector<double> dataSMA;

	SMA smaDisp;

	SMA sma;
	EMA ema;

	Dispersion disp;
};