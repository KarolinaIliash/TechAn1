#pragma once

#include "all.hpp"

//TODO!!! - fix utils and check aroon

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
	Manager() : rsi1(14) {}


public:
	void start() {}

	bool buySignal() {}
	bool selSignal() {}

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
		static RSI_States state;

		int levelToBreak = -1;

		double curRsi = rsi1.calculate(prices, i);
		if (i == 0)
		{
			setState(curRsi, state);
		}
		else
		{
			switch (state)
			{
			case OverSold:
				if (curRsi > 30 && curRsi <= 50)
				{
					if (buySignal) state = Wait;
					else state = FirstRiseBeforeBottomFailureSwing;
				}
				else if (curRsi > 50 && curRsi < 70) state = Wait; // Need think about this checkings
				else state = OverBought;
				break;
			case OverBought:
				if (curRsi < 70 && curRsi >= 50)
				{
					if (selSignal) state = Wait;
					else state = FirstFallBeforeTopFailureSwing;
				}
				else if (curRsi < 50 && curRsi > 30) state = Wait;
				else state = OverSold;
				break;
			case Wait:
				if (curRsi <= 30) state = OverSold;
				else if (curRsi >= 70) state = OverBought;
				break;
			case FirstRiseBeforeBottomFailureSwing:
				if (curRsi > 50) state = Wait;
				else if (curRsi < rsi_res.back())
				{
					state = FallBeforeBottomFailureSwing; 
					levelToBreak = rsi_res.back();
				}
				break;
			case FallBeforeBottomFailureSwing:
				if (curRsi <= 30) state = OverSold;
				else if (curRsi > rsi_res.back()) state = SecondRiseBeforeBottomFailureSwing;
				break;
			case SecondRiseBeforeBottomFailureSwing:
				if (curRsi >= levelToBreak) 
				{
					buySignal();
					state = Wait;
				}
				else if (curRsi <= 30) state = OverSold;
				else if (curRsi >= 70) state = OverBought;
				else if (curRsi < rsi_res.back()) state = Wait; // think about it
				break;
			case FirstFallBeforeTopFailureSwing:
				if (curRsi < 50) state = Wait;
				else if (curRsi > rsi_res.back())
				{
					state = RiseBeforeTopFailureSwing;
					levelToBreak = rsi_res.back();
				}
				break;
			case RiseBeforeTopFailureSwing:
				if (curRsi >= 70) state = OverBought;
				else if (curRsi > rsi_res.back()) state = SecondFallBeforeTopFailureSwing;
				break;
			case SecondFallBeforeTopFailureSwing:
				if (curRsi <= levelToBreak)
				{
					selSignal();
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

private:

	

	//CustomTrendDirection custom(1, NON, 1, 15, 5, true, );
	//ADX adx;
	//Aroon aroon;

	RSI_EMA rsi1;
	std::vector<double> rsi_res;
	//WilliamR williamR;
	//Stochastic stochastic;
};