//#define _WIN32_WINNT 0x0400

//#include "rest_client.hpp"

#include <vector>
#include <fstream>
#include <string>
#include <algorithm>

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

void main_()
{
	rewriteWithCommas("pricesUSD.txt", "pricesUSDComma.txt");
}

void main() 
{
	//std::string res = getData("EUR", "20180424", true);
	//std::string fileName = "pricesComma.txt";
	//std::string fileName = "pricesUSDComma.txt";
	std::string fileName = "prices.txt";
	std::vector<double> data = getData(fileName);

	//std::vector<double> firstEma, allEMA10, allEMA20, RSI_SMA, RSI_EMA, RSI_WSM;
	//calculateAll(data, allEMA10, allEMA20, RSI_SMA, RSI_EMA, RSI_WSM);
	//std::vector<double> EMA3, EMA21, AroonUp, AroonDown;
	//calc(data, EMA3, EMA21, AroonUp, AroonDown);
	//save(AroonUp, "AroonUp13.txt");
	//save(AroonDown, "AroonDown13.txt");

	std::vector<double> EMA;
	calcSmoothEMA_1_1(data, EMA, 21, 5);
	save(EMA, "EMA21smooth_v1_1.txt");

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


size_t findMaxIndex(std::vector<double> const& data, size_t from, size_t to)
{
	return std::distance(data.begin(), std::max_element(data.begin() + from, data.begin() + to));
}

size_t findMinIndex(std::vector<double> const& data, size_t from, size_t to)
{
	return std::distance(data.begin(), std::min_element(data.begin() + from, data.begin() + to));
}

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

		else if(i >= aroonPeriod)
		{
			double curAroonUp = ((aroonPeriod - (i - findMaxIndex(data, i - aroonPeriod, i))) / aroonPeriod) * 100;
			double curAroonDown = ((aroonPeriod - (i - findMinIndex(data, i - aroonPeriod, i))) / aroonPeriod) * 100;

			AroonUp.push_back(curAroonUp);
			AroonDown.push_back(curAroonDown);
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
	double sum = 0;
	std::vector<double> SMA(daysAmount, 0);
	//double SMA;
	std::vector<double> prevEMA(daysAmount, 0);
	//double prevEMA = 0;
	std::vector<int> hadSMA(daysAmount, 0);
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

	for (int i = 0; i < data.size(); i++)
	{
		if (i > daysAmount - 1)
		{

			int count = i % daysAmount;
			sum += data[i];
			sum -= data[i - daysAmount];

			double avg = sum / double(daysAmount);

			if (!hadSMA[count]/*&& (i + 1) % daysAmount == 0*/)
			{
				SMA[count] += avg;


				if ((i+1) / daysAmount == period)
				{
					SMA[count] /= double(period);
					hadSMA[count] = 1;

					prevEMA[count] = SMA[count];

					EMA.push_back(SMA[count]);
				}
				else
				{
					EMA.push_back(data[i]);
				}
			}
			else
			{
				double curEMA = calculateEMA(data[i], period, prevEMA[count]);
				prevEMA[count] = curEMA;
				EMA.push_back(curEMA);
			}
		}
		else
		{
			sum += data[i];
			EMA.push_back(data[i]);

			if (i == daysAmount - 1)
			{
				SMA[i] += sum / double(daysAmount);
			}
		}
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

			if (!hadSMA/*[count]*/&& (i + 1) % daysAmount == 0)
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
			else if(hadSMA)
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