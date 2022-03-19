#pragma once
/*
 * Copyright (c) 2020 Arduino.  All rights reserved.
 */

#ifndef STREAM_MOCK_H_
#define STREAM_MOCK_H_

 /**************************************************************************************
  * INCLUDE
  **************************************************************************************/

#include <deque>
#include <Stream.h>

  /**************************************************************************************
   * CLASS DECLARATION
   **************************************************************************************/

#if 0

template <class STREAM>
class Stream_stdstream : public Stream {
protected:
	//virtual int timedRead() = 0;
	//virtual int timedPeek() = 0;
public:

	Stream_stdstream(STREAM& s) : _stream(s) {}
	STREAM& stream() { return _stream; }

	virtual size_t write(const uint8_t c) override {
		_stream.put(static_cast<const char>(c));
		return _stream.good() ? 1 : 0;
	}
	virtual size_t write(const uint8_t* str, size_t n) override {
		auto first = _stream.tellp();
		_stream.write(reinterpret_cast<const char*>(str), n);		return _stream.good() ? n : _stream.tellp() - first;
	}
	using Print::write;
	using Print::print;
	using Print::println;

	virtual int availableForWrite() override { return std::numeric_limits<int>::max(); }

	virtual void flush() override {		_stream.flush();	}
	virtual int available() {	return _stream.rdbuf()->in_avail();	}
	virtual int read() ;
	virtual int peek() = 0;
	virtual bool find(const char* target, size_t length) = 0;
	virtual bool find(char target) = 0;
	virtual bool findUntil(const char* target, size_t targetLen, const char* terminator, size_t termLen) = 0;
	virtual long parseInt(LookaheadMode lookahead = SKIP_ALL, char ignore = NO_IGNORE_CHAR) = 0;
	virtual float parseFloat(LookaheadMode lookahead = SKIP_ALL, char ignore = NO_IGNORE_CHAR) = 0;
	virtual size_t readBytes(char* buffer, size_t length) = 0;
	virtual size_t readBytesUntil(char terminator, char* buffer, size_t length) = 0;
	virtual std::string readStdString() = 0;
	virtual std::string readStdStringUntil(char terminator) = 0;
protected:
	STREAM& stream;
};

#else

#include <iostream>

class StreamMock : public arduino::Stream
{
public:
	void operator << (char const* str) {
		for (size_t c = 0; c < strlen(str); c++)
			_stream.push_back(str[c]);
	}

	virtual size_t write(uint8_t ch) override {
		_stream.push_back(static_cast<char>(ch));
		return 1;
	}
	virtual int available() override {
		return _stream.size();
	}
	virtual int read() override {
		if (available() == 0)
			return -1;
		/* Extract first/oldest element. */
		char const c = _stream.at(0);
		/* Erase first/oldest element. */
		_stream.pop_front();

		return c;
	}
	virtual int peek() override {
		if (available() == 0)
			return -1;

		return _stream.at(0);
	}

private:
	std::deque<char> _stream;

};

#endif

#endif /* STREAM_MOCK_H_ */
