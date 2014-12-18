//*******************************************************************
//	created:	21:4:2011   12:18
//	filename: 	databuffer.h
//	author:		tiamo
//	purpose:	data buffer
//*******************************************************************

#pragma once

namespace myutils
{
	//
	// data buffer
	//
	class DataBuffer
	{
	public:
		//
		// read write helper
		//
		class ReadWriteHelper
		{
		public:
			//
			// constructor
			//
			ReadWriteHelper(DataBuffer& buffer, uint32_t offset = 0);

			//
			// destructor
			//
			~ReadWriteHelper()												{}

			//
			// read value
			//
			template<typename ValueType> 
			void read(ValueType& v, bool net = true)
			{
				//static_assert(std::is_integral<ValueType>::value == true, "need integer"); 

				readBuffer(&v, sizeof(v)); 
				
				if(net)
					swapOrder(&v, sizeof(v));
			}

			//
			// write value
			//
			template<typename ValueType> 
			void write(ValueType v, bool net = true)
			{
				//static_assert(std::is_integral<ValueType>::value == true, "need integer"); 

				if(net) 
					swapOrder(&v, sizeof(v)); 
				
				writeBuffer(&v, sizeof(v));
			}

			//
			// read value
			//
			void read(std::wstring& v, bool networkOrder = true);

			//
			// write value
			//
			void write(std::wstring const& v, bool networkOrder = true);

			//
			// read buffer
			//
			void readBuffer(void* theBuffer, uint32_t length);

			//
			// write buffer
			//
			void writeBuffer(void const* theBuffer, uint32_t length);

			//
			// swap buffer
			//
			void swapOrder(void* theBuffer, uint32_t length);

			//
			// make space
			//
			void makeSpace(uint32_t length);

			//
			// seek
			//
			void seek(int32_t offset);

			//
			// seek to
			//
			void seekTo(uint32_t offset);

			//
			// get buffer
			//
			void const* getBuffer() const									{return static_cast<uint8_t const*>(mDataBuffer.getBuffer()) + mOffset;}

			//
			// get buffer
			//
			void* getBuffer()												{return static_cast<uint8_t*>(mDataBuffer.getBuffer()) + mOffset;}

			//
			// get offset
			//
			uint32_t getOffset() const										{return mOffset;}

			//
			// get left length
			//
			uint32_t getLeftLength() const									{return mDataBuffer.getLength() - mOffset;}

		private:
			//
			// disable copy constructor
			//
			ReadWriteHelper(ReadWriteHelper const& rhs);
			ReadWriteHelper(ReadWriteHelper&& rhs);
			ReadWriteHelper& operator=(ReadWriteHelper const& rhs);
			ReadWriteHelper& operator=(ReadWriteHelper&& rhs);

		private:
			//
			// data buffer
			//
			DataBuffer&														mDataBuffer;

			//
			// offset
			//
			uint32_t														mOffset;
		};

	public:
		//
		// constructor
		//
		explicit DataBuffer(uint32_t bufferSize = 0);

		//
		// constructor
		//
		DataBuffer(void const* attachBuffer, uint32_t bufferSize, bool readOnly);

		//
		// copy constructor
		//
		DataBuffer(DataBuffer const& theOther);

		//
		// copy constructor
		//
		DataBuffer(DataBuffer&& theOther);

		//
		// assignment
		//
		DataBuffer& operator=(DataBuffer const& theOther);

		//
		// assignment
		//
		DataBuffer& operator=(DataBuffer&& theOther);

		//
		// compare
		//
		bool operator==(DataBuffer const& theOther) const;

		//
		// compare
		//
		bool operator!=(DataBuffer const& theOther) const					{return !(*this == theOther);}

		//
		// destructor
		//
		~DataBuffer();

		//
		// reset
		//
		void reset(bool deleteBuffer = true);

		//
		// reserve
		//
		void reserve(uint32_t bufferSize);

		//
		// resize
		//
		void resize(uint32_t bufferLength, bool zeroOut = false);

		//
		// append data
		//
		void appendData(void const* dataBuffer, uint32_t dataLength);

		//
		// consume data
		//
		void consumeData(uint32_t consumeLength);

		//
		// append data
		//
		template<typename Type>
		void appendData(Type const& theValue)								{this->appendData(&theValue, sizeof(theValue));}

		//
		// append data
		//
		void appendData(DataBuffer const& dataBuffer)						{appendData(dataBuffer.getBuffer(), dataBuffer.getLength());}

		//
		// get buffer
		//
		void* getBuffer()													{return mBuffer;}

		//
		// get buffer
		//
		void const* getBuffer() const										{return mBuffer;}

		//
		// get buffer size
		//
		uint32_t getSize() const											{return mBufferSize;}

		//
		// get buffer length
		//
		uint32_t getLength() const											{return mBufferLength;}

		//
		// get left space
		//
		uint32_t getLeftSpace() const										{return mBufferSize - mBufferLength;}

		//
		// is read only
		//
		bool isReadOnly() const												{return mReadOnly;}

	private:
		//
		// buffer
		//
		uint8_t*															mBuffer;

		//
		// buffer size
		//
		uint32_t															mBufferSize;

		//
		// buffer length
		//
		uint32_t															mBufferLength;

		//
		// read only
		//
		bool																mReadOnly;
	};
}
