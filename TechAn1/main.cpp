#include <vector>

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


void calculateAll(std::vector<double> const& data)
{
	//int i = 0;
	
	double sum5 = 0;
	double avg5;

	double prevEMA10;
	double prevEMA20;
	double SMA10 = 0;
	double SMA20 = 0;

	bool hadSMA10 = false;
	bool hadSMA20 = false;

	bool hadPrevEMA10 = false;
	bool hadPrevEMA20 = false;


	std::vector<double> allEMA10;
	std::vector <double> allEMA20;


	for (int i = 0; i < data.size(); i++)
	{
		if (i % 5 == 0)
		{
			sum5 = 0;
			avg5 = sum5 / 5;

			if (!hadSMA10)
			{
				SMA10 += avg5;
				if (i / 5 == 10)
				{
					SMA10 *= 0.1; // SMA10 /= 10;
					hadSMA10 = true;
					prevEMA10 = SMA10;
				}
			}
			else
			{
				double curEMA10 = calculateEMA(avg5, 10, prevEMA10);
				allEMA10.push_back(curEMA10);
			}


			if (!hadSMA20)
			{
				SMA20 += avg5;
				if (i / 5 == 20)
				{
					SMA20 /= 20;
					hadSMA20 = true;
				}
			}
			else
			{
				double curEMA20 = calculateEMA(avg5, 20, prevEMA20);
				allEMA20.push_back(curEMA20);
			}

		}
		sum5 += data[i];



	}
}