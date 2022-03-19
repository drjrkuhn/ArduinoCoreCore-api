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

const unsigned long CHAR_TIMEOUT_MS = 10;

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

#endif /* STREAM_MOCK_H_ */
