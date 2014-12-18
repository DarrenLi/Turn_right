//********************************************************************
//	created:	17:6:2013   9:48
//	filename: 	randomhelper.h
//	author:		tiamo
//	purpose:	random helper
//********************************************************************

#pragma once

namespace myutils
{
	//
	// random helper
	//
	class Random
	{
	public:
		//
		// constructor
		//
		Random()															{mSeed = 0;}

		//
		// destructor
		//
		~Random()															{}

		//
		// initialize
		//
		void initialize(int64_t seed)										{mSeed = seed;}

		//
		// check rate value
		//
		bool checkRate(int32_t rateValue, int32_t maxValue = 999999, int32_t minValue = 0) {return rateValue > getRandomValue(minValue, maxValue);}

		//
		// get random value
		//
		int32_t getRandomValue(int32_t maxValue = 999999, int32_t minValue = 0) {return static_cast<int32_t>(minValue + fixRand(maxValue - minValue + 1));}

	private:
		//
		// fix rand
		//
		int64_t fixRand(int32_t n);

		//
		// rand in 32bits
		//
		int64_t rand32();

	private:
		//
		// seed
		//
		int64_t																mSeed;

		//
		// rand max
		//
		static int64_t const												mRandMax;
	};
}
