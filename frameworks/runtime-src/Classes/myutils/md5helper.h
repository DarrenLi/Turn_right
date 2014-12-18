//********************************************************************
//	created:	9:4:2012   17:57
//	filename: 	md5helper.h
//	author:		tiamo
//	purpose:	md5 helper
//********************************************************************

#pragma once

/* UINT4 defines a four byte word */
typedef unsigned int UINT4;

/* MD5 context. */
struct md5_ctx {
    UINT4 state[4];                                   /* state (ABCD) */
    UINT4 count[2];        /* number of bits, modulo 2^64 (lsb first) */
    unsigned char buffer[64];                         /* input buffer */
};

typedef struct md5_ctx MD5_CTX;

namespace myutils
{
	//
	// md5 helper
	//
	class MD5Helper
	{
	public:
		//
		// constructor
		//
		MD5Helper();

		//
		// hash data
		//
		void hashData(void const* inputBuffer, size_t inputLength, void* outputBuffer);

		//
		// re-initialize
		//
		void reinitialize();

		//
		// append data
		//
		void appendData(void const* inputBuffer, size_t inputLength);

		//
		// get hash result
		//
		void getHashResult(void* outputBuffer);

	private:
		//
		// context
		//
		MD5_CTX																mContext;
	};
}
