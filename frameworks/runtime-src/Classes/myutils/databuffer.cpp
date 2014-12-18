//*******************************************************************
//	created:	21:4:2011   12:23
//	filename: 	databuffer.cpp
//	author:		tiamo
//	purpose:	data buffer
//*******************************************************************
#include "stdafx.h"
#include "databuffer.h"

namespace myutils
{
	//
	// constructor
	//
	DataBuffer::DataBuffer(uint32_t bufferSize)
	{
		mBufferSize															= bufferSize;
		mBufferLength														= 0;
		mBuffer																= bufferSize ? new uint8_t[bufferSize] : nullptr;
		mReadOnly															= false;
	}

	//
	// constructor
	//
	DataBuffer::DataBuffer(void const* attachBuffer, uint32_t bufferSize, bool readOnly)
	{
		mReadOnly															= readOnly;
		mBuffer																= readOnly ? const_cast<uint8_t*>(static_cast<uint8_t const*>(attachBuffer)) : new uint8_t[bufferSize];
		mBufferSize															= bufferSize;
		mBufferLength														= bufferSize;
		if(!readOnly)
			memcpy(mBuffer, attachBuffer, bufferSize);
	}

	//
	// copy constructor
	//
	DataBuffer::DataBuffer(DataBuffer const& theOther)
	{
		mReadOnly															= false;
		mBuffer																= nullptr;
		*this																= theOther;
	}

	//
	// copy constructor
	//
	DataBuffer::DataBuffer(DataBuffer&& theOther)
	{
		mReadOnly															= false;
		mBuffer																= nullptr;
		*this																= (theOther);
	}

	//
	// assignment
	//
	DataBuffer& DataBuffer::operator=(DataBuffer const& theOther)
	{
		if(this == &theOther)
			return *this;

		reset(true);

		mBuffer																= theOther.mBuffer ? new uint8_t[theOther.mBufferSize] : nullptr;
		mBufferSize															= theOther.mBufferSize;
		mBufferLength														= theOther.mBufferLength;
		mReadOnly															= false;
		memcpy(mBuffer, theOther.mBuffer, theOther.mBufferSize);

		return *this;
	}

	//
	// assignment
	//
	DataBuffer& DataBuffer::operator=(DataBuffer&& theOther)
	{
		if(this == &theOther)
			return *this;

		reset(true);

		mBufferSize															= theOther.mBufferSize;
		mBufferLength														= theOther.mBufferLength;
		mBuffer																= theOther.mBuffer;
		mReadOnly															= theOther.mReadOnly;

		theOther.reset(false);
		return *this;
	}

	//
	// compare
	//
	bool DataBuffer::operator==(DataBuffer const& theOther) const
	{
		return mBufferLength == theOther.mBufferLength && !memcmp(mBuffer, theOther.mBuffer, mBufferLength);
	}

	//
	// destructor
	//
	DataBuffer::~DataBuffer()
	{
		reset();
	}

	//
	// reset
	//
	void DataBuffer::reset(bool deleteBuffer)
	{
		if(!mReadOnly && deleteBuffer)
			delete[] mBuffer;

		mBuffer																= nullptr;
		mReadOnly															= false;
		mBufferLength														= 0;
		mBufferSize															= 0;
	}

	//
	// append data
	//
	void DataBuffer::appendData(void const* dataBuffer, uint32_t dataLength)
	{
		//
		// reserve size
		//
		reserve(mBufferLength + dataLength);

		//
		// copy new data
		//
		memcpy(mBuffer + mBufferLength, dataBuffer, dataLength);
		mBufferLength														+= dataLength;
	}

	//
	// consum data
	//
	void DataBuffer::consumeData(uint32_t consumeLength)
	{
		if(mReadOnly)
			ThrowException(L"DataBuffer(%p): unable to modify a read-only buffer", this);

		if(mBufferLength < consumeLength)
			ThrowException(L"DataBuffer(%p): invalid param (%u > %u)", this, consumeLength, mBufferLength);

		if(mBufferLength > consumeLength)
			memmove(mBuffer, mBuffer + consumeLength, mBufferLength - consumeLength);

		mBufferLength														-= consumeLength;
	}

	//
	// reserve
	//
	void DataBuffer::reserve(uint32_t bufferSize)
	{
		if(mReadOnly)
			ThrowException(L"DataBuffer(%p): unable to modify a read-only buffer", this);

		if(bufferSize <= mBufferSize)
			return;

		mBufferSize															= mBufferSize + mBufferSize + bufferSize;
		uint8_t* newBuffer													= new uint8_t[mBufferSize];
		memcpy(newBuffer, mBuffer, mBufferLength);
		delete[] mBuffer;
		mBuffer																= newBuffer;
	}

	//
	// resize
	//
	void DataBuffer::resize(uint32_t bufferLength, bool zeroOut)
	{
		if(mReadOnly)
			ThrowException(L"DataBuffer(%p): unable to modify a read-only buffer", this);

		reserve(bufferLength);

		if(mBufferLength < bufferLength && zeroOut)
			memset(mBuffer + mBufferLength, 0, bufferLength - mBufferLength);

		mBufferLength														= bufferLength;
	}

	//
	// constructor
	//
	DataBuffer::ReadWriteHelper::ReadWriteHelper(DataBuffer& buffer, uint32_t offset)
		: mDataBuffer(buffer)
	{
		mOffset																= offset;
	}

	//
	// read utf8 string
	//
	void DataBuffer::ReadWriteHelper::read(std::wstring& theValue, bool networkOrder)
	{
		uint16_t length														= 0;
		read(length, networkOrder);
		if(!length)
		{
			theValue														= L"";
		}
		else
		{
			std::string utf8Value;
			utf8Value.resize(length);
			readBuffer(&utf8Value[0], length);
			theValue														= utf8ToWide(utf8Value);
		}
	}

	//
	// write utf8 string
	//
	void DataBuffer::ReadWriteHelper::write(std::wstring const& theString, bool networkOrder)
	{
		std::string utf8Value												= wideToUtf8(theString);
		uint16_t length														= static_cast<uint16_t>(utf8Value.size());
		write(length, networkOrder);
		if(!length)
			return;

		writeBuffer(utf8Value.c_str(), length);
	}

	//
	// read buffer
	//
	void DataBuffer::ReadWriteHelper::readBuffer(void* theBuffer, uint32_t length)
	{
		if(length + mOffset > mDataBuffer.getLength())
			ThrowException(L"DataBuffer::ReadWriteHelper(%p): invalid length(%u + %u > %u)", this, length, mOffset, mDataBuffer.getLength());

		memcpy(theBuffer, getBuffer(), length);
		mOffset																+= length;
	}

	//
	// write buffer
	//
	void DataBuffer::ReadWriteHelper::writeBuffer(void const* theBuffer, uint32_t length)
	{
		mDataBuffer.resize(mOffset + length);
		memcpy(getBuffer(), theBuffer, length);
		mOffset																+= length;
	}

	//
	// swap buffer
	//
	void DataBuffer::ReadWriteHelper::swapOrder(void* theBuffer, uint32_t length)
	{
		uint8_t* byteBuffer													= static_cast<uint8_t*>(theBuffer);
		for(uint32_t i = 0; i < length / 2; i ++)
			std::swap(byteBuffer[i], byteBuffer[length - i - 1]);
	}

	//
	// make space
	//
	void DataBuffer::ReadWriteHelper::makeSpace(uint32_t length)
	{
		mDataBuffer.resize(mOffset + length);
	}

	//
	// seek
	//
	void DataBuffer::ReadWriteHelper::seek(int32_t offset)
	{
		if(offset + mOffset > mDataBuffer.getLength())
			ThrowException(L"myutils::DataBuffer::ReadWriteHelper(%p): invalid length(%u + %d > %u)", this, mOffset, offset, mDataBuffer.getLength());

		mOffset																+= offset;
	}

	//
	// seek to
	//
	void DataBuffer::ReadWriteHelper::seekTo(uint32_t offset)
	{
		if(offset > mDataBuffer.getLength())
			ThrowException(L"myutils::DataBuffer::ReadWriteHelper(%p): invalid length(%u > %u)", this, offset, mDataBuffer.getLength());

		mOffset																= offset;
	}
}
