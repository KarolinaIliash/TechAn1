//#define _WIN32_WINNT 0x0400

//#include "rest_client.hpp"

#include <vector>
#include <fstream>
#include <string>
#include <algorithm>

#include <oscillators/RSI_EMA.hpp>

#include <trend_indicators/CustomTrendDirection.hpp>

#include <manager\Manager.hpp>

std::vector<double> getData(std::string fileName)
{
	std::ifstream in(fileName);
	std::vector<double> data;
	while (!in.eof())
	{
		double price;
		in >> price;
		data.push_back(price);
	}
	in.close();
	return data;
}

double calculateSMA(std::vector<double> const& data)
{
	double sum = 0;
	for (double const& data_ : data)
	{
		sum += data_;
	}
	sum /= data.size();
	return sum;
}

double calculateEMA(double const& lastPrice, double const& period, double const& prevEMA)
{
	double weightMult = 2 / (period + 1);
	double EMA = (lastPrice - prevEMA) * weightMult + prevEMA;
	return EMA;
}

std::string doubleWithComma(double & price)
{
	std::string str = std::to_string(price);

	size_t delim = str.find(".");

	str.replace(delim, 1, ",");

	return str;
}

void rewriteWithCommas(std::string from, std::string to)
{
	std::ifstream in(from);
	std::ofstream out(to);

	int i = 0;

	while (!in.eof())
	{
		double price;
		in >> price;

		std::string priceComma = doubleWithComma(price);

		if (i > 0)
		{
			out << std::endl;
		}

		out << priceComma;
		i++;
	}
}

void calculateAll(std::vector<double> const& data, std::vector<double> & allEMA10,
	std::vector<double> & allEMA20, std::vector<double> &, std::vector<double> &, std::vector<double> &);

void calc(std::vector<double> const& data, std::vector<double> & EMA3, std::vector<double> & EMA21,
	std::vector<double> &, std::vector<double> &);

void calcSmoothEMA(std::vector<double> const& data, std::vector<double> & EMA, int const& period, int const& daysAmount);
void calcSmoothEMA_1_1(std::vector<double> const& data, std::vector<double> & EMA, int const& period, int const& daysAmount);

void calcDispersion(std::vector<double> const& prices, int daysAmount, int period, std::vector<double> & dispersion, bool isEMA = true);

void calcSMA(std::vector<double> const& prices, int daysAmount, int period, std::vector<double> & SMA);

void calcFromClass(std::vector<double> const& data, std::vector<double> & RSI_EMA_14);

void save(std::vector<double> const& data, std::string const& name)
{
	std::ofstream out(name);

	int i = 0;

	for (double price : data)
	{
		if (i > 0)
		{
			out << std::endl;
		}
		out << doubleWithComma(price);
		i++;
	}
	out.close();
}

void save_with_dots(std::vector<double> const& data, std::string const& name)
{
	std::ofstream out(name);

	int i = 0;

	for (double elem : data)
	{
		if (i > 0)
		{
			out << std::endl;
		}

		out << elem;
		i++;
	}
	out.close();
}


template<typename T>
void save(std::vector<T> const& data, std::string const& name)
{
	std::ofstream out(name);

	int i = 0;

	for (T elem : data)
	{
		if (i > 0)
		{
			out << std::endl;
		}

		out << elem;
		i++;
	}
	out.close();
}

void main_()
{
	rewriteWithCommas("pricesUSD.txt", "pricesUSDComma.txt");
}

//enum TrendDirection
//{
//	UP,
//	WARNING_UP,
//	LiGHT_WARNING_UP,
//	NON,
//	WARNING_DOWN,
//	LIGHT_WARNING_DOWN,
//	DOWN
//};
//
//enum Smooth
//{
//	NO,
//	SMA,
//	EMA
//};


void calcEMATrendDirection(std::vector<double> const& prices, std::vector<TrendDirection> & direction,
	int shortPeriod, int longPeriod, int shortDaysAmount, int longDaysAmount, int deltaPeriod, int lookBackPeriod,
	std::vector<double> & shortEMA, std::vector<double> & longEMA);

void calcADX(std::vector<double> const& prices, int const& period, int const& daysAmount, std::vector<double> & ADX,
	std::vector<double> & positiveDI, std::vector<double> & negativeDI);

void calcWilliamR(std::vector<double> const& prices, int period, std::vector<double> & R);

void calcStochastic(std::vector<double> const& prices, int period, std::vector<double> & K, std::vector<double> & D);

void calcTrendDirectionByPrices(std::vector<double> const& prices, std::vector<TrendDirection> & direction, int const& deltaPeriod, int const& lookBackPeriod, Smooth const& smooth, int daysAmount, int period,
	std::vector<double> & data, std::vector<double> & dispersion, std::vector<double> & dataSMA,
	double multSmallest, double multMiddle, double multBiggest);

void calcADX_(std::vector<double> const& prices, int const& period, int const& daysAmount, std::vector<double> & ADX,
	std::vector<double> & positiveDI, std::vector<double> & negativeDI);


void calcDirWithClassFix(std::vector<double> const& prices, int deltaPeriod, int lookBackPeriod,
	Smooth smooth, int period, int daysAmount, bool isEma, std::vector<TrendDirection> & res,
	double multBiggest, double multMiddle, double multSmallest)
{
	CustomTrendDirection custom(period, smooth, daysAmount, deltaPeriod, lookBackPeriod, isEma,
		multBiggest, multMiddle, multSmallest);

	for (int i = 0; i < prices.size(); i++)
	{
		res.push_back(custom.calculate(prices, i));
	}
}

void main()
{
	//std::string res = getData("EUR", "20180424", true);
	//std::string fileName = "pricesComma.txt";
	//std::string fileName = "pricesUSDComma.txt";
	//std::string fileName = "prices.txt";
	
	std::string fileName = "pricesEurWithoutWeekend.txt";
	std::vector<double> data = getData(fileName);
	/*std::string comma = "pricesEurWithoutWeekend_comma.txt";
	rewriteWithCommas(fileName, comma);

	fileName = "pricesUsdWithoutWeekend.txt";
	comma = "pricesUsdWithoutWeekend_comma.txt";
	rewriteWithCommas(fileName, comma);*/


	Manager manager;

	manager.start(data);


	std::vector <double> rsi = manager.getRSIRes();
	std::vector<int> signals = manager.getSignals();

	std::vector<int> sellSignalsDays = manager.sellSignalsDays;
	std::vector<int> buySignalsDays = manager.buySignalsDays;
	save(sellSignalsDays, "sellSignalsDays_no_weekends.txt");
	save(buySignalsDays, "buySignalsDays_no_weekeds.txt");
	save(rsi, "rsi_ema_14_from_manager_no_weekends.txt");
	save(signals, "rsi_ema_14_signals_no_weekends.txt");
	//std::vector<double> firstEma, allEMA10, allEMA20, RSI_SMA, RSI_EMA, RSI_WSM;
	//calculateAll(data, allEMA10, allEMA20, RSI_SMA, RSI_EMA, RSI_WSM);
	//std::vector<double> EMA3, EMA21, AroonUp, AroonDown;
	//calc(data, EMA3, EMA21, AroonUp, AroonDown);
	//save(AroonUp, "AroonUp13.txt");
	//save(AroonDown, "AroonDown13.txt");

	std::vector<double> shortEMA;
	std::vector<double> longEMA;
	std::vector<TrendDirection> direction;


	std::vector<double> ADX;
	std::vector<double> positiveDI;
	std::vector<double> negativeDI;

	std::vector<double> RSI_EMA_14;

	/*calcFromClass(data, RSI_EMA_14);

	save(RSI_EMA_14, "RSI_EMA_14_class.txt");*/

	/*calcDirWithClassFix(data, 15, 5, NO, 1, 1, 0, direction, 2, 1, 0.5);

	save(direction, "direction_15_5_class_fix.txt");*/

	/*calcADX(data, 7, 5, ADX, positiveDI, negativeDI);

	save(ADX, "ADX_7_per5day_last.txt");
	save(positiveDI, "positiveDI_7_per5day_last.txt");
	save(negativeDI, "negativeDI_7_per5day_last.txt");*/

	/*calcADX_(data, 7, 5, ADX, positiveDI, negativeDI);

	save(ADX, "ADX_7_per5day_step__f.txt");
	save(positiveDI, "positiveDI_7_per5day_step__f.txt");
	save(negativeDI, "negativeDI_7_per5day_step__f.txt");*/

	/*std::vector<double> R;

	calcWilliamR(data, 14, R);
	save(R, "WilliamR_14.txt");*/


	/*std::vector<double> K;
	std::vector<double> D;
	calcStochastic(data, 14, K, D);

	save(K, "Stochastic_K_14.txt");
	save(D, "Stochastic_D_14.txt");*/

	/*calcEMATrendDirection(data, direction, 1, 7, 1, 5, 15, 5, shortEMA, longEMA);
	save(direction, "EMA1per1_7_Delta_15_5_direction.txt");

	save_with_dots(longEMA, "EMA7_per5day_dots.txt");*/
	//save(shortEMA, "EMA1_per1day.txt");

	/*calcEMATrendDirection(data, direction, 3, 21, 5, 5, 21, 7, shortEMA, longEMA);

	save(direction, "EMA3_21_Delta_21_7_direction.txt");
	save_with_dots(shortEMA, "EMA3_per5day_with_dots.txt");
	save_with_dots(longEMA, "EMA21_per5day_with_dots.txt");*/

	/*std::vector<double> res;
	std::vector<double> dispersion;
	std::vector<double> resSMA;

	calcTrendDirectionByPrices(data, direction, 13, 3, Smooth::NO, 1, 1, res, dispersion, resSMA,
		0.5, 1., 2.);

	save(direction, "trendByPrices_13_3_mults_1div2_1_2.txt");
	save(res, "dataByPricesInTrendSearch_13_3.txt");
	save(resSMA, "dataSMAByPricesInTrendSerach_13_3.txt");
	save(dispersion, "dispersionInTrendSearch_13_3.txt");*/

	//	std::vector<double> SMA;
	//calcSMA(data, 5, 21, SMA);
	//save(SMA, "SMA21.txt");
	//std::vector<double> Disp;

	//calcDispersion(data, 5, 21, Disp, false);
	//save(Disp, "DispersionSMA21.txt");
	//calcSmoothEMA(data, EMA, 21, 5);
	//save(EMA, "smoothEMA21PerDay.txt");
	//calcSmoothEMA_1_1(data, EMA, 21, 5);
	//save(EMA, "EMA21smooth_v1_1.txt");

	//save(EMA3, "EMA3.txt");
	//save(EMA21, "EMA21.txt");

	//save(EMA3, "EMA3.txt");
	//save(EMA21, "EMA21.txt");
	/*save(RSI_SMA, "RSI_SMA6.txt");
	save(RSI_EMA, "RSI_EMA6.txt");
	save(RSI_WSM, "RSI_WSM6.txt");*/
	//save(RSI, "RSI14.txt");

	/*save(allEMA10, "EMA10.txt");
	save(allEMA20, "EMA20.txt");

	rewriteWithCommas("prices.txt", "pricesComma.txt");*/
}

void calcFromClass(std::vector<double> const& data, std::vector<double> & RSI_EMA_14)
{
	RSI_EMA calc(14);
	for (int i = 0; i < data.size(); i++)
	{
		double curRSI = calc.calculate(data, i);
		RSI_EMA_14.push_back(curRSI);
	}
}

void calculateAll(std::vector<double> const& data, std::vector<double> & allEMA10, std::vector<double> & allEMA20,
	std::vector<double> & RSI_SMA, std::vector<double> & RSI_EMA, std::vector<double> & RSI_WSM)
{
	//int i = 0;

	double sum5 = 0;
	double avg5;

	double prevEMA3;
	double prevEMA21;
	double SMA3 = 0;
	double SMA21 = 0;


	double prevEMA10;
	double prevEMA20;
	double SMA10 = 0;
	double SMA20 = 0;

	bool hadSMA10 = false;
	bool hadSMA20 = false;

	bool hadPrevEMA10 = false;
	bool hadPrevEMA20 = false;

	double curEMA10 = -1;
	double curEMA20 = -1;
	//std::vector<double> allEMA10;
	//std::vector <double> allEMA20;

	double sumU15 = 0;
	double sumD15 = 0;



	double prevAvgU_EMA = -1;
	double prevAvgD_EMA = -1;
	double prevAvgU_WSM = -1;
	double prevAvgD_WSM = -1;

	const double RSIperiod = 14.;


	for (int i = 0; i < data.size(); i++)
	{
		if (i != 0 && i % 5 == 0)
		{
			avg5 = sum5 / 5;

			sum5 = 0;
			if (!hadSMA10)
			{
				SMA10 += avg5;
				allEMA10.push_back(data[i]);
				if (i / 5 == 10)
				{
					SMA10 *= 0.1; // SMA10 /= 10;
					hadSMA10 = true;
					prevEMA10 = SMA10;
				}
			}
			else
			{
				/*double */curEMA10 = calculateEMA(avg5, 10, prevEMA10);
				allEMA10.push_back(curEMA10);
				prevEMA10 = curEMA10;
			}


			if (!hadSMA20)
			{
				SMA20 += avg5;
				allEMA20.push_back(data[i]);
				if (i / 5 == 20)
				{
					SMA20 /= 20;
					hadSMA20 = true;
					prevEMA20 = SMA20;
				}
			}
			else
			{
				/*double */curEMA20 = calculateEMA(avg5, 20, prevEMA20);
				allEMA20.push_back(curEMA20);
				prevEMA20 = curEMA20;
			}

		}
		else
		{
			if (curEMA10 != -1)
			{
				allEMA10.push_back(curEMA10);
			}
			else
			{
				allEMA10.push_back(data[i]);
			}
			if (curEMA20 != -1)
			{
				allEMA20.push_back(curEMA20);
			}
			else
			{
				allEMA20.push_back(data[i]);
			}
		}
		sum5 += data[i];

		//Calculation of RSI
		if (i > 0 && i < RSIperiod + 1)
		{
			double delta = data[i] - data[i - 1];
			if (delta > 0)
			{
				sumU15 += delta; // for SMA RSI
			}
			else if (delta < 0)
			{
				sumD15 += (-delta); // for SMA RSI
			}

			if (i == RSIperiod)
			{
				double avgU = sumU15 / RSIperiod;
				double avgD = sumD15 / RSIperiod;
				double RS = avgU / avgD;

				double curRSI = 100 - 100 / (1. + RS);
				RSI_SMA.push_back(curRSI);
				RSI_EMA.push_back(curRSI);
				RSI_WSM.push_back(curRSI);
				//start data for EMA RSI and WSM RSI
				prevAvgU_EMA = avgU;
				prevAvgU_WSM = avgU;

				prevAvgD_EMA = avgD;
				prevAvgD_WSM = avgD;
			}
		}
		else if (i > RSIperiod)
		{
			double curU = 0; // for EMA and
			double curD = 0; // WSM RSI

			double deltaPrev = data[i - RSIperiod] - data[i - RSIperiod - 1];
			if (deltaPrev > 0)
			{
				sumU15 = sumU15 - deltaPrev;
			}
			else if (deltaPrev < 0)
			{
				sumD15 = sumD15 + deltaPrev;
			}

			double delta = data[i] - data[i - 1];
			if (delta > 0)
			{
				sumU15 += delta;
				curU = delta;
			}
			else if (delta < 0)
			{
				sumD15 -= delta;
				curD = -delta;
			}
			//for SMA RSI
			double avgU = sumU15 / RSIperiod;
			double avgD = sumD15 / RSIperiod;
			double RS = avgU / avgD;

			double curRSI = 100 - 100 / (1. + RS);
			RSI_SMA.push_back(curRSI);
			// EMA and WSM RSI
			double alphaEMA = 2. / (RSIperiod + 1.);
			double alphaWSM = 1. / RSIperiod;

			double avgU_EMA = alphaEMA * curU + (1 - alphaEMA) * prevAvgU_EMA;
			double avgD_EMA = alphaEMA * curD + (1 - alphaEMA) * prevAvgD_EMA;
			double RS_EMA = avgU_EMA / avgD_EMA;
			double curRSI_EMA = 100 - 100 / (1. + RS_EMA);
			RSI_EMA.push_back(curRSI_EMA);
			prevAvgU_EMA = avgU_EMA;
			prevAvgD_EMA = avgD_EMA;

			double avgU_WSM = alphaWSM * curU + (1 - alphaWSM) * prevAvgU_WSM;
			double avgD_WSM = alphaWSM * curD + (1 - alphaWSM) * prevAvgD_WSM;
			double RS_WSM = avgU_WSM / avgD_WSM;
			double curRSI_WSM = 100 - 100 / (1. + RS_WSM);
			RSI_WSM.push_back(curRSI_WSM);
			prevAvgU_WSM = avgU_WSM;
			prevAvgD_WSM = avgD_WSM;
		}
	}
}

//TODO try give to RSI and roon not prices but their sma or ema (daily) maybe about 13 period 


//size_t findMaxIndex(std::vector<double> const& data, size_t from, size_t to)
//{
//	return std::distance(data.begin(), std::max_element(data.begin() + from, data.begin() + to));
//
//	//try this, i actually don't remember but 
//	// return std::distance(std::max_element(data.begin() + from, data.begin() + to), data.begin() + to);
//}
//
//size_t findMinIndex(std::vector<double> const& data, size_t from, size_t to)
//{
//	return std::distance(data.begin(), std::min_element(data.begin() + from, data.begin() + to));
//
//	//try this, i actually don't remember but 
//	// return std::distance(std::min_element(data.begin() + from, data.begin() + to), data.begin() + to);
//}
//
//template<typename T>
//T findMax(std::vector<T> const& data, size_t from, size_t to)
//{
//	return *std::max_element(data.begin() + from, data.begin() + to);
//}
//
//
//template<typename T>
//T findMin(std::vector<T> const& data, size_t from, size_t to)
//{
//	return *std::min_element(data.begin() + from, data.begin() + to);
//}

void calc(std::vector<double> const& data, std::vector<double> & EMA3, std::vector<double> & EMA21,
	std::vector<double> & AroonUp, std::vector<double> & AroonDown)
{
	double sum5 = 0;
	double avg5;

	double prevEMA3;
	double prevEMA21;
	double SMA3 = 0;
	double SMA21 = 0;

	bool hadSMA3 = false;
	bool hadSMA21 = false;

	bool hadPrevEMA3 = false;
	bool hadPrevEMA21 = false;

	double curEMA3 = -1;
	double curEMA21 = -1;

	const double aroonPeriod = 13;//55;//25;
	int indexMax = 0;
	int indexMin = 0;

	for (int i = 0; i < data.size(); i++)
	{
		if (i != 0 && i % 5 == 0)
		{
			avg5 = sum5 / 5;
			sum5 = 0;
			if (!hadSMA3)
			{
				SMA3 += avg5;
				EMA3.push_back(data[i]);
				if (i / 5 == 3)
				{
					SMA3 /= 3;
					hadSMA3 = true;
					prevEMA3 = SMA3;
				}
			}
			else
			{
				/*double */curEMA3 = calculateEMA(avg5, 3, prevEMA3);
				EMA3.push_back(curEMA3);
				prevEMA3 = curEMA3;
			}


			if (!hadSMA21)
			{
				SMA21 += avg5;
				EMA21.push_back(data[i]);
				if (i / 5 == 21)
				{
					SMA21 /= 21;
					hadSMA21 = true;
					prevEMA21 = SMA21;
				}
			}
			else
			{
				/*double */curEMA21 = calculateEMA(avg5, 21, prevEMA21);
				EMA21.push_back(curEMA21);
				prevEMA21 = curEMA21;
			}

		}
		else
		{
			if (curEMA3 != -1)
			{
				EMA3.push_back(curEMA3);
			}
			else
			{
				EMA3.push_back(data[i]);
			}
			if (curEMA21 != -1)
			{
				EMA21.push_back(curEMA21);
			}
			else
			{
				EMA21.push_back(data[i]);
			}
		}
		sum5 += data[i];


		//aroon calc
		if (i != 0 && i < aroonPeriod)
		{

		}

		else if (i >= aroonPeriod)
		{
			double curAroonUp = ((aroonPeriod - (i - findMaxIndex(data, i - aroonPeriod, i))) / aroonPeriod) * 100;
			double curAroonDown = ((aroonPeriod - (i - findMinIndex(data, i - aroonPeriod, i))) / aroonPeriod) * 100;

			AroonUp.push_back(curAroonUp);
			AroonDown.push_back(curAroonDown);
		}

	}
}


void calcSmoothEMAForDay(double data, double data_to_delete, std::vector<double> & EMA, int i, int daysAmount, int period,
	std::vector <double> & SMA, std::vector<double> & prevEMA, std::vector<bool> & hasSMA, double & sum)
{
	//static double sum = 0;
	//static std::vector<double> SMA(daysAmount, 0);
	//static std::vector<double> prevEMA(daysAmount, 0);
	//static std::vector<bool> hasSMA(daysAmount, false);

	if (i > daysAmount - 1)
	{

		int count = i % daysAmount;
		sum += data;
		sum -= data_to_delete;//data[i - daysAmount];

		double avg = sum / double(daysAmount);

		if (!hasSMA[count]/*&& (i + 1) % daysAmount == 0*/)
		{
			SMA[count] += avg;

			if ((i + 1) / daysAmount == period)
			{
				SMA[count] /= double(period);
				hasSMA[count] = true;

				prevEMA[count] = SMA[count];

				EMA.push_back(SMA[count]);
			}
			else
			{
				EMA.push_back(data);
			}
		}
		else
		{
			double curEMA = calculateEMA(data, period, prevEMA[count]);
			prevEMA[count] = curEMA;
			EMA.push_back(curEMA);
		}
	}
	else
	{
		sum += data;
		EMA.push_back(data);

		if (i == daysAmount - 1)
		{
			SMA[i] += sum / double(daysAmount);
		}
	}
}


void calcSmoothEMA(std::vector<double> const& data, std::vector<double> & EMA, int const& period, int const& daysAmount)
{
	//int sum = 0;
	//int sum1Middle = 0;
	//int sum2Middle = 0;
	//int sum3Middle = 0;
	//int sum4Middle = 0;

	//std::vector<int> sum(daysAmount, 0);
	//double sum = 0;
	//std::vector<double> SMA(daysAmount, 0);
	//double SMA;
	//	std::vector<double> prevEMA(daysAmount, 0);
	//double prevEMA = 0;
	//std::vector<int> hadSMA(daysAmount, 0);
	//bool hadSMA = false;
	//int SMA;
	//int prevEMA;
	//
	//int SMA1Middle;
	//int prevEMA1Middle;
	//
	//int SMA2Middle;
	//int prevEMA2Middle;
	//
	//int SMA3Middle;
	//int prevEMA3Middle;
	//
	//int SMA4Middle;
	//int prevEMA4Middle;


	double sum = 0;
	std::vector<double> SMA(daysAmount, 0);
	std::vector<double> prevEMA(daysAmount, 0);
	std::vector<bool> hasSMA(daysAmount, false);

	for (int i = 0; i < data.size(); i++)
	{
		if (i > daysAmount - 1)
			calcSmoothEMAForDay(data[i], data[i - daysAmount], EMA, i, daysAmount, period, SMA, prevEMA, hasSMA, sum);
		else
			calcSmoothEMAForDay(data[i], 0, EMA, i, daysAmount, period, SMA, prevEMA, hasSMA, sum);
		//if (i > daysAmount - 1)
		//{

		//	int count = i % daysAmount;
		//	sum += data[i];
		//	sum -= data[i - daysAmount];

		//	double avg = sum / double(daysAmount);

		//	if (!hadSMA[count]/*&& (i + 1) % daysAmount == 0*/)
		//	{
		//		SMA[count] += avg;


		//		if ((i+1) / daysAmount == period)
		//		{
		//			SMA[count] /= double(period);
		//			hadSMA[count] = 1;

		//			prevEMA[count] = SMA[count];

		//			EMA.push_back(SMA[count]);
		//		}
		//		else
		//		{
		//			EMA.push_back(data[i]);
		//		}
		//	}
		//	else
		//	{
		//		double curEMA = calculateEMA(data[i], period, prevEMA[count]);
		//		prevEMA[count] = curEMA;
		//		EMA.push_back(curEMA);
		//	}
		//}
		//else
		//{
		//	sum += data[i];
		//	EMA.push_back(data[i]);

		//	if (i == daysAmount - 1)
		//	{
		//		SMA[i] += sum / double(daysAmount);
		//	}
		//}
	}
}

void calcSmoothEMA_1_1(std::vector<double> const& data, std::vector<double> & EMA, int const& period, int const& daysAmount)
{
	//int sum = 0;
	//int sum1Middle = 0;
	//int sum2Middle = 0;
	//int sum3Middle = 0;
	//int sum4Middle = 0;

	//std::vector<int> sum(daysAmount, 0);
	double sum = 0;
	//std::vector<double> SMA(daysAmount, 0);
	double SMA = 0;
	//std::vector<double> prevEMA(daysAmount, 0);
	double prevEMA = 0;
	//std::vector<int> hadSMA(daysAmount, 0);
	bool hadSMA = false;
	//int SMA;
	//int prevEMA;
	//
	//int SMA1Middle;
	//int prevEMA1Middle;
	//
	//int SMA2Middle;
	//int prevEMA2Middle;
	//
	//int SMA3Middle;
	//int prevEMA3Middle;
	//
	//int SMA4Middle;
	//int prevEMA4Middle;

	for (int i = 0; i < data.size(); i++)
	{
		if (i > daysAmount - 1)
		{

			int count = i % daysAmount;
			sum += data[i];
			sum -= data[i - daysAmount];

			double avg = sum / double(daysAmount);

			if (!hadSMA/*[count]*/ && (i + 1) % daysAmount == 0)
			{
				SMA/*[count]*/ += avg;


				if ((i + 1) / daysAmount == period)
				{
					SMA/*[count]*/ /= double(period);
					hadSMA/*[count]*/ = 1;

					prevEMA/*[count]*/ = SMA/*[count]*/;

					EMA.push_back(SMA/*[count]*/);
				}
				else
				{
					EMA.push_back(data[i]);
				}
			}
			else if (hadSMA)
			{
				double curEMA = calculateEMA(data[i], period, prevEMA/*[count]*/);
				//prevEMA/*[count]*/ = curEMA;
				if ((i + 1) % daysAmount == 0)
				{
					prevEMA = curEMA;
				}
				EMA.push_back(curEMA);
			}

			else
			{
				EMA.push_back(data[i]);
			}
		}
		else
		{
			sum += data[i];
			EMA.push_back(data[i]);

			if (i == daysAmount - 1)
			{
				SMA/*[i]*/ += sum / double(daysAmount);
			}
		}
	}
}


double calcSMAPerDay(double data, double dataToDelete, int daysAmount, int period, int i)
{
	static double sum = 0;
	static std::vector<double> SMAdays(daysAmount, 0);
	static std::vector<double> averages;

	if (i > daysAmount - 1)
	{

		sum += data;
		sum -= dataToDelete;

		int count = i % daysAmount;

		double avg = sum / double(daysAmount);
		averages.push_back(avg);

		if ((i + 1) / daysAmount >= period)
		{
			SMAdays[count] += avg;

			if ((i + 1) / daysAmount != period)
				SMAdays[count] -= averages[i - daysAmount * period];

			return SMAdays[count] / period;
		}
		else
		{
			SMAdays[count] += avg;
			return data;
		}

	}
	else
	{
		sum += data;

		if (i == daysAmount - 1)
		{
			double avg = sum / double(daysAmount);
			averages.push_back(avg);
			SMAdays[i] += avg;
		}

		return data;
	}
}

void calcSMA(std::vector<double> const& prices, int daysAmount, int period, std::vector<double> & SMA)
{
	for (int i = 0; i < prices.size(); i++)
	{
		double dataTodelete = 0;
		if (i > daysAmount - 1)
			dataTodelete = prices[i - daysAmount];

		SMA.push_back(calcSMAPerDay(prices[i], dataTodelete, daysAmount, period, i));
	}
}

double calcDispersionPerDay(double data, double dataToDelete, double EMAToDelete, double EMA, int daysAmount, int period, int i)
{
	static double sum = 0;
	static std::vector<double> prevDispersion(daysAmount, 0);
	static std::vector<double> dispersion(daysAmount, 0);

	int count = i % daysAmount;

	static std::vector<double> averages;

	if (i > daysAmount - 1)
	{
		sum += data;
		sum -= dataToDelete;

		double avg = sum / double(daysAmount);
		averages.push_back(avg);

		if ((i + 1) / daysAmount >= period)
		{
			dispersion[count] += pow(avg - EMA, 2);

			if ((i + 1) / daysAmount != period)
				dispersion[count] -= pow(averages[i - daysAmount * period] - EMAToDelete, 2);

			return dispersion[count] / (period - 1);
		}
		else
		{
			dispersion[count] += pow(avg - EMA, 2);
			return 0;
		}

	}
	else // we can't calculate average yet
	{
		sum += data;

		if (i == daysAmount - 1)
		{
			double avg = sum / double(daysAmount);
			averages.push_back(avg);
			dispersion[i] += pow(avg - EMA, 2);
		}
		return 0;
	}
}


void calcDispersion(std::vector<double> const& prices, int daysAmount, int period, std::vector<double> & dispersion, bool isEMA/* = true*/)
{
	std::vector<double> MA;

	double sum = 0;
	std::vector<double> SMA(daysAmount, 0);
	std::vector<double> prevEMA(daysAmount, 0);
	std::vector<bool> hasSMA(daysAmount, false);

	for (int i = 0; i < prices.size(); i++)
	{
		double disp = 0;
		if (i > daysAmount - 1)
		{
			if (isEMA)
				calcSmoothEMAForDay(prices[i], prices[i - daysAmount], MA, i, daysAmount, period,
					SMA, prevEMA, hasSMA, sum);
			else
				MA.push_back(calcSMAPerDay(prices[i], prices[i - daysAmount], daysAmount, period, i));

			if ((i + 1) / daysAmount > period)
				disp = calcDispersionPerDay(prices[i], prices[i - daysAmount], MA[i - daysAmount * period], MA[i], daysAmount, period, i);
			else
				disp = calcDispersionPerDay(prices[i], prices[i - daysAmount], 0, MA[i], daysAmount, period, i);
		}
		else
		{
			if (isEMA)
				calcSmoothEMAForDay(prices[i], 0, MA, i, daysAmount, period,
					SMA, prevEMA, hasSMA, sum);
			else
				MA.push_back(calcSMAPerDay(prices[i], 0, daysAmount, period, i));

			disp = calcDispersionPerDay(prices[i], 0, prices[i], MA[i], daysAmount, period, i);
		}

		dispersion.push_back(disp);
	}
}

void calcEMATrendDirection(std::vector<double> const& prices, std::vector<TrendDirection> & direction,
	int shortPeriod, int longPeriod, int shortDaysAmount, int longDaysAmount, int deltaPeriod, int lookBackPeriod,
	std::vector<double> & shortEMA, std::vector<double> & longEMA)
{
	std::vector<double> deltas;
	//std::vector<double> shortEMA;
	//std::vector<double> longEMA;
	std::vector<double> dispDelta;
	std::vector<double> deltaSMA;

	double shortSum = 0;
	std::vector<double> shortSMA(shortDaysAmount, 0);
	std::vector<double> shortPrevEMA(shortDaysAmount, 0);
	std::vector<bool> shortHasSMA(shortDaysAmount, false);

	double longSum = 0;
	std::vector<double> longSMA(longDaysAmount, 0);
	std::vector<double> longPrevEMA(longDaysAmount, 0);
	std::vector<bool> longHasSMA(longDaysAmount, false);



	for (int i = 0; i < prices.size(); i++)
	{
		if (i > shortDaysAmount - 1)
			calcSmoothEMAForDay(prices[i], prices[i - shortDaysAmount], shortEMA, i, shortDaysAmount, shortPeriod,
				shortSMA, shortPrevEMA, shortHasSMA, shortSum);
		else
			calcSmoothEMAForDay(prices[i], 0, shortEMA, i, shortDaysAmount, shortPeriod,
				shortSMA, shortPrevEMA, shortHasSMA, shortSum);

		if (i > longDaysAmount - 1)
			calcSmoothEMAForDay(prices[i], prices[i - longDaysAmount], longEMA, i, longDaysAmount, longPeriod,
				longSMA, longPrevEMA, longHasSMA, longSum);
		else
			calcSmoothEMAForDay(prices[i], 0, longEMA, i, longDaysAmount, longPeriod,
				longSMA, longPrevEMA, longHasSMA, longSum);

		double delta = shortEMA[i] - longEMA[i];

		deltas.push_back(delta);

		if (i > longDaysAmount * longPeriod - 1) //+ lookBackPeriod)//(i > 0)
		{
			if (delta - deltaSMA[i - 1] >= dispDelta[i - 1])
			{
				bool everyMoreThenHalfOfSigma = true;

				for (int k = 1; k <= lookBackPeriod; k++)
				{
					if (deltas[i - k] - deltaSMA[i - k - 1] < dispDelta[i - 1] / 2.)
					{
						everyMoreThenHalfOfSigma = false;
						break;
					}
				}


				if (everyMoreThenHalfOfSigma)
				{
					if (delta - deltaSMA[i - 1] >= 2 * dispDelta[i - 1])
					{
						direction.push_back(TrendDirection::UP);
					}
					else
					{
						direction.push_back(TrendDirection::WARNING_UP);
					}
				}
				else
				{
					direction.push_back(TrendDirection::LiGHT_WARNING_UP);
				}
			}

			else if (delta - deltaSMA[i - 1] <= -dispDelta[i - 1])
			{
				bool everyLessThenHalfOfSigma = true;
				for (int k = 1; k <= lookBackPeriod; k++)
				{
					if (deltas[i - k] - deltaSMA[i - k - 1] > -dispDelta[i - 1] / 2.)
					{
						everyLessThenHalfOfSigma = false;
						break;
					}
				}

				if (everyLessThenHalfOfSigma)
				{
					if (delta - deltaSMA[i - 1] <= -2 * dispDelta[i - 1])
					{
						direction.push_back(TrendDirection::DOWN);
					}
					else
					{
						direction.push_back(TrendDirection::WARNING_DOWN);
					}
				}
				else
				{
					direction.push_back(TrendDirection::LIGHT_WARNING_DOWN);
				}
			}
			else
			{
				direction.push_back(TrendDirection::NON);
			}
		}
		double disp = 0;
		if (i > 0)
		{

			deltaSMA.push_back(calcSMAPerDay(deltas[i], deltas[i - 1], 1, deltaPeriod, i));

			if ((i + 1) > deltaPeriod)
				disp = calcDispersionPerDay(deltas[i], deltas[i - 1], deltaSMA[i - deltaPeriod], deltaSMA[i], 1, deltaPeriod, i);
			else
				disp = calcDispersionPerDay(deltas[i], deltas[i - 1], 0, deltaSMA[i], 1, deltaPeriod, i);
		}
		else
		{
			deltaSMA.push_back(calcSMAPerDay(deltas[i], 0, 1, deltaPeriod, i));

			disp = calcDispersionPerDay(deltas[i], 0, deltas[i], deltaSMA[i], 1, deltaPeriod, i);
		}

		dispDelta.push_back(sqrt(disp));
	}
}

void calcTrendDirectionByPrices(std::vector<double> const& prices, std::vector<TrendDirection> & direction, int const& deltaPeriod, int const& lookBackPeriod, Smooth const& smooth, int daysAmount, int period,
	std::vector<double> & data, std::vector<double> & dispersion, std::vector<double> & dataSMA,
	double multSmallest, double multMiddle, double multBiggest)
{

	double sum = 0;
	std::vector<double> SMA(daysAmount, 0);
	std::vector<double> prevEMA(daysAmount, 0);
	std::vector<bool> hasSMA(daysAmount, false);

	// std::vector<double> dispersion;
	//std::vector<double> dataSMA;

	//std::vector<double> data;

	if (smooth == Smooth::NO)
	{
		daysAmount = 1;
		period = 1;
	}

	for (int i = 0; i < prices.size(); i++)
	{

		if (i == 34)
		{
			int a = 2;
		}

		/* if (i > daysAmount - 1)
		{
		if (isEMA)
		calcSmoothEMAForDay(prices[i], prices[i - daysAmount], MA, i, daysAmount, period,
		SMA, prevEMA, hasSMA, sum);
		else
		MA.push_back(calcSMAPerDay(prices[i], prices[i - daysAmount], daysAmount, period, i));

		if ((i + 1) / daysAmount >= period)
		disp = calcDispersionPerDay(prices[i], prices[i - daysAmount], MA[i - daysAmount * period + 1], MA[i], daysAmount, period, i);
		else
		disp = calcDispersionPerDay(prices[i], prices[i - daysAmount], 0, MA[i], daysAmount, period, i);
		}
		else
		{
		if (isEMA)
		calcSmoothEMAForDay(prices[i], 0, MA, i, daysAmount, period,
		SMA, prevEMA, hasSMA, sum);
		else
		MA.push_back(calcSMAPerDay(prices[i], 0, daysAmount, period, i));

		disp = calcDispersionPerDay(prices[i], 0, prices[i], MA[i], daysAmount, period, i);
		}*/

		if (smooth == Smooth::NO)
		{
			data.push_back(prices[i]);
		}

		else
		{
			if (i > daysAmount - 1)
			{
				if (smooth == Smooth::SMASmooth)
				{
					data.push_back(calcSMAPerDay(prices[i], prices[i - daysAmount], daysAmount, period, i));
				}
				else if (smooth == Smooth::EMASmooth)
				{
					calcSmoothEMAForDay(prices[i], prices[i - daysAmount], data, i, daysAmount, period, SMA, prevEMA, hasSMA, sum);
				}
			}
			else
			{
				if (smooth == Smooth::SMASmooth)
				{
					data.push_back(calcSMAPerDay(prices[i], 0, daysAmount, period, i));
				}
				else if (smooth == Smooth::EMASmooth)
				{
					calcSmoothEMAForDay(prices[i], 0, data, i, daysAmount, period, SMA, prevEMA, hasSMA, sum);
				}
			}
		}

		double disp;
		if (i > 0)
		{

			dataSMA.push_back(calcSMAPerDay(data[i], data[i - 1], 1, deltaPeriod, i));

			if ((i + 1) > deltaPeriod)
				disp = calcDispersionPerDay(data[i], data[i - 1], dataSMA[i - deltaPeriod], dataSMA[i], 1, deltaPeriod, i);
			else
				disp = calcDispersionPerDay(data[i], data[i - 1], 0, dataSMA[i], 1, deltaPeriod, i);
		}
		else
		{
			dataSMA.push_back(calcSMAPerDay(data[i], 0, 1, deltaPeriod, i));

			disp = calcDispersionPerDay(data[i], 0, data[i], dataSMA[i], 1, deltaPeriod, i);
		}
		dispersion.push_back(sqrt(disp));

		if (i > daysAmount * period - 1 && i > lookBackPeriod - 1) //+ lookBackPeriod)//(i > 0)
		{
			//if (data[i] - dataSMA[i - 1] >= dispersion[i - 1])
			//if (data[i] - dataSMA[i - 1] >= dispersion[i - 1] * multMiddle)
			if (data[i] - dataSMA[i - 1] >= dispersion[i] * multMiddle)
			{
				bool everyMoreThenHalfOfSigma = true;
				bool trendContinue = true;
				bool trendReverse = false;

				// if (!direction.empty() && direction.back() != TrendDirection::NON && direction.back() != TrendDirection::WARNING_UP_NEED_CHECK)
				for (int k = 1; k <= lookBackPeriod; k++)
				{
					//if (data[i - k] - dataSMA[i - k - 1] < dispersion[i - 1] * multSmallest)
					if (data[i - k] - dataSMA[i - k - 1] < dispersion[i] * multSmallest)
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
					if (data[i] - dataSMA[i - 1] >= /*2 **/ dispersion[i] * multBiggest)
					{
						direction.push_back(TrendDirection::UP);
					}
					else
					{
						direction.push_back(TrendDirection::WARNING_UP);
					}
				}
				else
				{
					direction.push_back(TrendDirection::LiGHT_WARNING_UP);
				}
			}

//			else if (data[i] - dataSMA[i - 1] <= -dispersion[i - 1] * multMiddle)
			else if (data[i] - dataSMA[i - 1] <= -dispersion[i] * multMiddle)
			{
				bool everyLessThenHalfOfSigma = true;

				//if (!direction.empty() && direction.back() != TrendDirection::NON)
				for (int k = 1; k <= lookBackPeriod; k++)
				{
					//if (data[i - k] - dataSMA[i - k - 1] > -dispersion[i - 1]/* / 2.*/ * multSmallest)
					if (data[i - k] - dataSMA[i - k - 1] > -dispersion[i]/* / 2.*/ * multSmallest)
					{
						everyLessThenHalfOfSigma = false;
						break;
					}
				}

				if (everyLessThenHalfOfSigma)
				{
					//if (data[i] - dataSMA[i - 1] <= -2 * dispersion[i - 1] * multBiggest)
					if (data[i] - dataSMA[i - 1] <= -2 * dispersion[i] * multBiggest)
					{
						direction.push_back(TrendDirection::DOWN);
					}
					else
					{
						direction.push_back(TrendDirection::WARNING_DOWN);
					}
				}
				else
				{
					direction.push_back(TrendDirection::LIGHT_WARNING_DOWN);
				}
			}
			else
			{
				direction.push_back(TrendDirection::NON);
			}
		}
		
		if (dispersion.size() - direction.size() > 3)
		{
			throw std::exception("Sth went wrong");
		}
	}
}

void calcADX(std::vector<double> const& prices, int const& period, int const& daysAmount, std::vector<double> & ADX,
	std::vector<double> & positiveDI, std::vector<double> & negativeDI)
{
	int sum = 0;
	std::vector<double> prevADX(daysAmount, 0);

	std::vector<double> prevDMP(daysAmount, 0);
	std::vector<double> prevDMM(daysAmount, 0);
	std::vector<double> prevTR(daysAmount, 0);

	std::vector<double> prevHigh(daysAmount, INT_MIN);
	std::vector<double> curHigh(daysAmount, INT_MIN);

	std::vector<double> prevLow(daysAmount, INT_MAX);
	std::vector<double> curLow(daysAmount, INT_MAX);

	std::vector<double> prevClose;//?

	std::vector<double> DMPSMA(daysAmount, 0);
	std::vector<double> DMMSMA(daysAmount, 0);
	std::vector<double> TRSMA(daysAmount, 0);
	std::vector<double> DXSMA(daysAmount, 0);

	std::vector<double> averages;

	double firstHigh = INT_MIN;
	double firstLow = INT_MAX;

	std::vector<bool> hasDMPSMA(daysAmount, false);
	std::vector<bool> hasDMMSMA(daysAmount, false);
	std::vector<bool> hasTRSMA(daysAmount, false);
	std::vector<bool> hasDXSMA(daysAmount, false);

	std::vector<double> sumDMP(daysAmount, 0);
	std::vector<double> sumDMM(daysAmount, 0);
	std::vector<double> sumTR(daysAmount, 0);
	std::vector<double> sumDX(daysAmount, 0);

	for (int i = 0; i < prices.size(); i++)
	{
		int count = i % daysAmount;

		if (i > 2*daysAmount - 1)
		{
			//find new high
			//if (prices[i - daysAmount] == prevHigh[count])
			//{
			//	curHigh[count] = findMax(prices, i - daysAmount + 1, i + 1);
			//}
			//else if (prices[i] > prevHigh[count])
			//{
			//	curHigh[count] = prices[i];
			//}
			//else
			//{
			//	curHigh[count] = prevHigh[count];
			//}
			////find new low
			//if (prices[i - daysAmount] == prevLow[count])
			//{
			//	curLow[count] = findMin(prices, i - daysAmount + 1, i + 1);
			//}
			//else if (prices[i] < prevLow[count])
			//{
			//	curLow[count] = prices[i];
			//}
			//else
			//{
			//	curLow[count] = prevLow[count];
			//}

			curHigh[count] = findMax(prices, i - daysAmount + 1, i + 1);
			curLow[count] = findMin(prices, i - daysAmount + 1, i + 1);
			//calc dm plus

			double DMP = curHigh[count] - prevHigh[count];
			if (DMP < 0) DMP = 0;

			double DMM = prevLow[count] - curLow[count];
			if (DMM < 0) DMM = 0;

			/*if (prevHigh[(count + 1) % daysAmount] == INT_MIN)
			{
				prevHigh[(count + 1) % daysAmount] = curHigh[count];
			}
			if (prevLow[(count + 1) % daysAmount] == INT_MAX)
			{
				prevLow[(count + 1) % daysAmount] = curLow[count];
			}*/

			double TR = std::max(
				std::initializer_list<double>{ curHigh[count] - curLow[count], abs(curHigh[count] - prices[i - daysAmount]),
				abs(prices[i - daysAmount] - curLow[count]) });

			prevHigh[count] = curHigh[count];
			prevLow[count] = curLow[count];

			if (!hasDMPSMA[count]/*&& (i + 1) % daysAmount == 0*/ || !hasDMMSMA[count])
			{
				sumDMP[count] += DMP;
				sumDMM[count] += DMM;
				sumTR[count] += TR;
				//DMPSMA[count] += sumDMP;
				//DMMSMA[count] += sumDMM;

				if ((i + 1) / daysAmount == period)
				{
					DMPSMA[count] = sumDMP[count] / double(period);
					hasDMPSMA[count] = true;

					//prevEMA[count] = SMA[count];
					prevDMP[count] = DMPSMA[count];


					DMMSMA[count] = sumDMM[count] / double(period);
					hasDMMSMA[count] = true;
					prevDMM[count] = DMMSMA[count];

					TRSMA[count] = sumTR[count] / double(period);
					hasTRSMA[count] = true;
					prevTR[count] = TRSMA[count];
				}
			}
			else
			{
				DMP = (prevDMP[count] * (period - 1) + DMP) / period;

				DMM = (prevDMM[count] * (period - 1) + DMM) / period;

				TR = (prevTR[count] * (period - 1) + TR) / period;

				prevDMP[count] = DMP;
				prevDMM[count] = DMM;
				prevTR[count] = TR;

				double posDI = (DMP / TR) * 100;
				double negDI = (DMM / TR) * 100;

				positiveDI.push_back(posDI);
				negativeDI.push_back(negDI);

				double DlDiff = abs(posDI - negDI);
				double DlSum = posDI + negDI;

				double DX = (DlDiff / DlSum) * 100;
				if (!hasDXSMA[count])
				{
					sumDX[count] += DX;

					if ((i + 1) / daysAmount == 2 * period + count)
					{
						hasDXSMA[count] = true;

						DXSMA[count] = sumDX[count] / period;

						prevADX[count] = DXSMA[count];

						ADX.push_back(DXSMA[count]);
					}
				}
				else
				{
					double curADX = (prevADX[count] * (period - 1) + DX) / period;
					ADX.push_back(curADX);
					prevADX[count] = curADX;
				}

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
			if (i > count)
			{
				prevHigh[count] = firstHigh;
				prevLow[count] = firstLow;
			}
		}
	}
}

void calcADX_(std::vector<double> const& prices, int const& period, int const& daysAmount, std::vector<double> & ADX,
	std::vector<double> & positiveDI, std::vector<double> & negativeDI)
{
	int sum = 0;
	//std::vector<double> prevADX(daysAmount, 0);
	double prevADX;

	double prevDMP;
	double prevDMM;
	double prevTR;

	double prevHigh;
	double curHigh;

	double prevLow;
	double curLow;

	double prevClose;//?

	double DMPSMA;
	double DMMSMA;
	double TRSMA;
	double DXSMA;

	std::vector<double> averages;

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

	for (int i = 0; i < prices.size(); i++)
	{
		int count = i % daysAmount;

		if (i >= 2 * daysAmount - 1 && (i + 1) % daysAmount == 0)
		{
			//find new high
			//if (prices[i - daysAmount] == prevHigh[count])
			//{
			//	curHigh[count] = findMax(prices, i - daysAmount + 1, i + 1);
			//}
			//else if (prices[i] > prevHigh[count])
			//{
			//	curHigh[count] = prices[i];
			//}
			//else
			//{
			//	curHigh[count] = prevHigh[count];
			//}
			////find new low
			//if (prices[i - daysAmount] == prevLow[count])
			//{
			//	curLow[count] = findMin(prices, i - daysAmount + 1, i + 1);
			//}
			//else if (prices[i] < prevLow[count])
			//{
			//	curLow[count] = prices[i];
			//}
			//else
			//{
			//	curLow[count] = prevLow[count];
			//}

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
			else if(DMM > DMP)
			{
				tempDMP = 0;
			}
			DMM = tempDMM;
			DMP = tempDMP;
			/*if (prevHigh[(count + 1) % daysAmount] == INT_MIN)
			{
			prevHigh[(count + 1) % daysAmount] = curHigh[count];
			}
			if (prevLow[(count + 1) % daysAmount] == INT_MAX)
			{
			prevLow[(count + 1) % daysAmount] = curLow[count];
			}*/

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
				if(DM_TR_SMA_index == period)
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

						ADX.push_back(DXSMA);
					}
				}
				else
				{
					double curADX = (prevADX * (period - 1) + DX) / period;
					ADX.push_back(curADX);
					prevADX = curADX;
				}
			}
		}
		else if (i > 2 * daysAmount - 1)
		{
			if (hasDMMSMA)
			{
				negativeDI.push_back(negativeDI.back());
			}
			if (hasDMPSMA)
			{
				positiveDI.push_back(positiveDI.back());
			}
			if (hasDXSMA)
			{
				ADX.push_back(ADX.back());
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
			if (i == daysAmount- 1)
			{
				prevHigh = firstHigh;
				prevLow = firstLow;
			}
		}
	}
}

void calcWilliamR(std::vector<double> const& prices, int period, std::vector<double> & R)
{

	for (int i = 0; i < prices.size(); i++)
	{
		if (i >= period)
		{
			double high = findMax(prices, i - period + 1, i + 1);

			double low = findMin(prices, i - period + 1, i + 1);

			double curR = ((high - prices[i]) / (high - low)) * -100;

			R.push_back(curR);
		}
	}
}

void calcStochastic(std::vector<double> const& prices, int period, std::vector<double> & K, std::vector<double> & D)
{
	int dSmoothPeriod = 3;

	double sum = 0;

	for (int i = 0; i < prices.size(); i++)
	{
		if (i >= period)
		{
			double low = findMin(prices, i - period + 1, i + 1);
			double high = findMax(prices, i - period + 1, i + 1);

			double curK = 100 * (prices[i] - low) / (high - low);

			K.push_back(curK);

			if (i > period + dSmoothPeriod - 1)
			{
				sum += curK;
				sum -= K[i - dSmoothPeriod - period];

				double curD = sum / dSmoothPeriod;

				D.push_back(curD);
			}

			else
			{
				sum += curK;
				if (i == period + dSmoothPeriod - 1)
				{
					double curD = sum / dSmoothPeriod;
					D.push_back(curD);
				}
			}
		}
	}
}