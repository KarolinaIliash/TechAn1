#pragma once

#include "all.hpp"

enum RSI_States
{
	OverSold,
	OverBought,
	Wait,
	FirstRiseBeforeBottomFailureSwing,
	FallBeforeBottomFailureSwing,
	SecondRiseBeforeBottomFailureSwing,
	FirstFallBeforeTopFailureSwing,
	RiseBeforeTopFailureSwing,
	SecondFallBeforeTopFailureSwing,
};


class Manager
{

public:
	Manager() : rsi1(14), aroon(25) {}


public:
	void start(std::vector<double> const& prices) 
	{
		for (int i = 0; i < prices.size(); i++)
		{
			//manageRSI(prices, i);
			calcAroon(prices, i);
		}
	}

	void calcAroon(std::vector<double> const& prices, int i)
	{
		std::pair<double, double> aroon_res = aroon.calculate(prices, i);
		aroonUp.push_back(aroon_res.first);
		aroonDown.push_back(aroon_res.second);
	}


	bool buySignal(int i) 
	{
		signals[i] = 1;
		buySignalsDays.push_back(i);
		return false;
	}
	bool selSignal(int i) 
	{
		signals[i] = -1;
		sellSignalsDays.push_back(i);
		return false;
	}

	void setState(double value, RSI_States & state)
	{
		if (value <= 30)
		{
			state = OverSold;
		}
		else if (value >= 70)
		{
			state = OverBought;
		}
		else
		{
			state = Wait;
		}
	}

	void manageRSI(std::vector<double> const& prices, int i)
	{
		if (signals.size() != prices.size()) 
		{
			signals.assign(prices.size(), 0);
		}

		static RSI_States state;

		static double levelToBreak = -1;

		static bool started = false;
		static bool stateSet = false;

		double curRsi = rsi1.calculate(prices, i);

		if (!started && curRsi != 0)
		{
			started = true;
		}

		if (started && !stateSet)
		{
			setState(curRsi, state);
			stateSet = true;
		}

		if (i == 180)
		{
			int a = 1;
		}

		else if(stateSet)
		{
			switch (state)
			{
			case OverSold:
				if (curRsi > 30 /*&& curRsi <= 50*/)
				{
					if (buySignal(i)) state = Wait;
					else if (curRsi <= 50)
						state = FirstRiseBeforeBottomFailureSwing;
					else
						state = Wait;
				}
				//else if (curRsi > 50 && curRsi < 70) state = Wait; // Need think about this checkings
				//else if(curRsi >= 70) state = OverBought;
				if (curRsi >= 70) state = OverBought;
				break;
			case OverBought:
				if (curRsi < 70 /*&& curRsi >= 50*/)
				{
					if (selSignal(i)) state = Wait;
					else if(curRsi >= 50) state = FirstFallBeforeTopFailureSwing;
					else state = Wait;
				}
				//else if (curRsi < 50 && curRsi > 30) state = Wait;
				/*else */if(curRsi <= 30) state = OverSold;
				break;
			case Wait:
				if (curRsi <= 30) state = OverSold;
				else if (curRsi >= 70) state = OverBought;
				break;
			case FirstRiseBeforeBottomFailureSwing:
				if (curRsi > 50) state = Wait;
				else if (curRsi < rsi_res.back() && curRsi > 30)
				{
					state = FallBeforeBottomFailureSwing; 
					levelToBreak = rsi_res.back();
				}
				else if (curRsi <= 30) state = OverSold;
				break;
			case FallBeforeBottomFailureSwing:
				if (curRsi <= 30) state = OverSold;
				else if (curRsi > rsi_res.back()) state = SecondRiseBeforeBottomFailureSwing;
				break;
			case SecondRiseBeforeBottomFailureSwing:
				if (curRsi >= levelToBreak) 
				{
					buySignal(i);
					state = Wait;
				}
				else if (curRsi <= 30) state = OverSold;
				else if (curRsi >= 70) state = OverBought;
				else if (curRsi < rsi_res.back()) state = Wait; // think about it
				break;
			case FirstFallBeforeTopFailureSwing:
				if (curRsi < 50) state = Wait;
				else if (curRsi > rsi_res.back() && curRsi < 70)
				{
					state = RiseBeforeTopFailureSwing;
					levelToBreak = rsi_res.back();
				}
				else if (curRsi >= 70) state = OverBought;
				break;
			case RiseBeforeTopFailureSwing:
				if (curRsi >= 70) state = OverBought;
				else if (curRsi > rsi_res.back()) state = SecondFallBeforeTopFailureSwing;
				break;
			case SecondFallBeforeTopFailureSwing:
				if (curRsi <= levelToBreak)
				{
					selSignal(i);
					state = Wait;
				}
				else if (curRsi <= 30) state = OverSold;
				else if (curRsi >= 70) state = OverBought;
				else if (curRsi > rsi_res.back()) state = Wait; // think about it
				break;
			}
		}

		rsi_res.push_back(curRsi);
	}

	std::vector<int> getSignals() { return signals; }

	std::vector<double> getRSIRes() { return rsi_res; }


	std::vector<int> sellSignalsDays;
	std::vector<int> buySignalsDays;


	std::vector<double> getAroonUp() { return aroonUp; }
	std::vector<double> getAroonDown() { return aroonDown; }
private:

	std::vector<double> aroonUp;
	std::vector<double> aroonDown;

	//CustomTrendDirection custom(1, NON, 1, 15, 5, true, );
	//ADX adx;
	Aroon aroon;

	RSI_EMA rsi1;
	std::vector<double> rsi_res;

	std::vector<int> signals;

	//WilliamR williamR;
	//Stochastic stochastic;
};