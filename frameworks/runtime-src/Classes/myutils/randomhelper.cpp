//********************************************************************
//	created:	17:6:2013   9:52
//	filename: 	randomhelper.cpp
//	author:		tiamo
//	purpose:	random helper
//********************************************************************
#include "stdafx.h"
#include "randomhelper.h"

namespace myutils
{
	//
	// rand max
	//
	int64_t const Random::mRandMax											= 2147483647LL;

	//
	// fix rand
	//
	int64_t Random::fixRand(int32_t n)
	{
		int64_t x															= (mRandMax + 1) / n;
		int64_t y															= x * n;
		int64_t r															= 0;
		do
		{
			r																= rand32();
		}while(r >= y);
		return r / x;
	}

	//
	// rand in 32bits
	//
	int64_t Random::rand32()
	{
		if(!mSeed)
			mSeed															= 123459876;

		int64_t hi															= mSeed / 127773;
		int64_t low															= mSeed % 127773;
		int64_t x															= 16807 * low - 2836 * hi;
		if(x < 0)
			x																+= 2147483647LL;

		mSeed																= x;
		return x % (mRandMax + 1);
	}
}
