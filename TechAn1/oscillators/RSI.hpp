#pragma once

#include <vector>

class RSI
{
public:

	RSI(int period_) : period(period_) {}

public:

	virtual double calculate(std::vector<double> const& data, int i) = 0;

protected:
	int period;
	/*std::vector<double> RSI_SMA;
	std::vector<double> RSI_EMA;
	std::vector<double> RSI_WSM;*/
	std::vector<double> result;
	double sumU = 0;
	double sumD = 0;

	/*double prevAvgU_EMA = -1;
	double prevAvgD_EMA = -1;
	double prevAvgU_WSM = -1;
	double prevAvgD_WSM = -1;*/
	//double calculateSMA(std::vector<double> const& data, int i) 
	//{
	//	if (i > 0 && i < period + 1)
	//	{
	//		double delta = data[i] - data[i - 1];
	//		if (delta > 0)
	//		{
	//			sumU += delta; // for SMA RSI
	//		}
	//		else if (delta < 0)
	//		{
	//			sumD += (-delta); // for SMA RSI
	//		}

	//		if (i == period)
	//		{
	//			double avgU = sumU / period;
	//			double avgD = sumD / period;
	//			double RS = avgU / avgD;

	//			double curRSI = 100 - 100 / (1. + RS);
	//			RSI_SMA.push_back(curRSI);
	//			RSI_EMA.push_back(curRSI);
	//			RSI_WSM.push_back(curRSI);
	//			//start data for EMA RSI and WSM RSI
	//			prevAvgU_EMA = avgU;
	//			prevAvgU_WSM = avgU;

	//			prevAvgD_EMA = avgD;
	//			prevAvgD_WSM = avgD;

	//			return curRSI;
	//		}
	//	}
	//	else if (i > period)
	//	{
	//		double curU = 0; // for EMA and
	//		double curD = 0; // WSM RSI

	//		double deltaPrev = data[i - period] - data[i - period - 1];
	//		if (deltaPrev > 0)
	//		{
	//			sumU = sumU - deltaPrev;
	//		}
	//		else if (deltaPrev < 0)
	//		{
	//			sumD = sumD + deltaPrev;
	//		}

	//		double delta = data[i] - data[i - 1];
	//		if (delta > 0)
	//		{
	//			sumU += delta;
	//			curU = delta;
	//		}
	//		else if (delta < 0)
	//		{
	//			sumD -= delta;
	//			curD = -delta;
	//		}
	//		//for SMA RSI
	//		double avgU = sumU / period;
	//		double avgD = sumD / period;
	//		double RS = avgU / avgD;

	//		double curRSI = 100 - 100 / (1. + RS);
	//		RSI_SMA.push_back(curRSI);
	//		// EMA and WSM RSI
	//		double alphaEMA = 2. / (period + 1.);
	//		double alphaWSM = 1. / period;

	//		double avgU_EMA = alphaEMA * curU + (1 - alphaEMA) * prevAvgU_EMA;
	//		double avgD_EMA = alphaEMA * curD + (1 - alphaEMA) * prevAvgD_EMA;
	//		double RS_EMA = avgU_EMA / avgD_EMA;
	//		double curRSI_EMA = 100 - 100 / (1. + RS_EMA);
	//		RSI_EMA.push_back(curRSI_EMA);
	//		prevAvgU_EMA = avgU_EMA;
	//		prevAvgD_EMA = avgD_EMA;

	//		double avgU_WSM = alphaWSM * curU + (1 - alphaWSM) * prevAvgU_WSM;
	//		double avgD_WSM = alphaWSM * curD + (1 - alphaWSM) * prevAvgD_WSM;
	//		double RS_WSM = avgU_WSM / avgD_WSM;
	//		double curRSI_WSM = 100 - 100 / (1. + RS_WSM);
	//		RSI_WSM.push_back(curRSI_WSM);
	//		prevAvgU_WSM = avgU_WSM;
	//		prevAvgD_WSM = avgD_WSM;

	//		return;
	//	}
	//}

	//double calculateEMA(std::vector<double> const& data, int i)
	//{
	//	if (i > 0 && i < period + 1)
	//	{
	//		double delta = data[i] - data[i - 1];
	//		if (delta > 0)
	//		{
	//			sumU += delta; // for SMA RSI
	//		}
	//		else if (delta < 0)
	//		{
	//			sumD += (-delta); // for SMA RSI
	//		}

	//		if (i == period)
	//		{
	//			double avgU = sumU / period;
	//			double avgD = sumD / period;
	//			double RS = avgU / avgD;

	//			double curRSI = 100 - 100 / (1. + RS);
	//			RSI_SMA.push_back(curRSI);
	//			RSI_EMA.push_back(curRSI);
	//			RSI_WSM.push_back(curRSI);
	//			//start data for EMA RSI and WSM RSI
	//			prevAvgU_EMA = avgU;
	//			prevAvgU_WSM = avgU;

	//			prevAvgD_EMA = avgD;
	//			prevAvgD_WSM = avgD;

	//			return curRSI;
	//		}
	//	}
	//	else if (i > period)
	//	{
	//		double curU = 0; // for EMA and
	//		double curD = 0; // WSM RSI

	//		double deltaPrev = data[i - period] - data[i - period - 1];
	//		if (deltaPrev > 0)
	//		{
	//			sumU = sumU - deltaPrev;
	//		}
	//		else if (deltaPrev < 0)
	//		{
	//			sumD = sumD + deltaPrev;
	//		}

	//		double delta = data[i] - data[i - 1];
	//		if (delta > 0)
	//		{
	//			sumU += delta;
	//			curU = delta;
	//		}
	//		else if (delta < 0)
	//		{
	//			sumD -= delta;
	//			curD = -delta;
	//		}
	//		//for SMA RSI
	//		double avgU = sumU / period;
	//		double avgD = sumD / period;
	//		double RS = avgU / avgD;

	//		double curRSI = 100 - 100 / (1. + RS);
	//		RSI_SMA.push_back(curRSI);
	//		// EMA and WSM RSI
	//		double alphaEMA = 2. / (period + 1.);
	//		double alphaWSM = 1. / period;

	//		double avgU_EMA = alphaEMA * curU + (1 - alphaEMA) * prevAvgU_EMA;
	//		double avgD_EMA = alphaEMA * curD + (1 - alphaEMA) * prevAvgD_EMA;
	//		double RS_EMA = avgU_EMA / avgD_EMA;
	//		double curRSI_EMA = 100 - 100 / (1. + RS_EMA);
	//		RSI_EMA.push_back(curRSI_EMA);
	//		prevAvgU_EMA = avgU_EMA;
	//		prevAvgD_EMA = avgD_EMA;

	//		double avgU_WSM = alphaWSM * curU + (1 - alphaWSM) * prevAvgU_WSM;
	//		double avgD_WSM = alphaWSM * curD + (1 - alphaWSM) * prevAvgD_WSM;
	//		double RS_WSM = avgU_WSM / avgD_WSM;
	//		double curRSI_WSM = 100 - 100 / (1. + RS_WSM);
	//		RSI_WSM.push_back(curRSI_WSM);
	//		prevAvgU_WSM = avgU_WSM;
	//		prevAvgD_WSM = avgD_WSM;

	//		return;
	//	}
	//}

	//double calculateWSM(std::vector<double> const& data, int i)
	//{}
};