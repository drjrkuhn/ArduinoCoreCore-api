#pragma once

/*
  Copyright (c) 2016 Arduino LLC.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#pragma once

#include <cstdint>
#include <streambuf>
//#include <stdio.h> // for size_t
#include <string>	// for char_traits
#include <iterator> // for char iterators
#include <limits>
#include <thread>

#include "String_helpers.h"
#include "new_String.h"
#include "new_Print.h"
#include "new_time.h"
#include <chrono>

namespace stduino
{
#if 1
	// compatibility macros for testing
	/*
	#define   getInt()            parseInt()
	#define   getInt(ignore)    parseInt(ignore)
	#define   getFloat()          parseFloat()
	#define   getFloat(ignore)  parseFloat(ignore)
	#define   getString( pre_string, post_string, buffer, length)
	readBytesBetween( pre_string, terminator, buffer, length)
	*/

	// This enumeration provides the lookahead options for parseInt(), parseFloat()
	// The rules set out here are used until either the first valid character is found
	// or a time out occurs due to lack of input.
	enum LookaheadMode
	{
		SKIP_ALL,		// All invalid characters are ignored.
		SKIP_NONE,		// Nothing is skipped, and the stream is not touched unless the first waiting character is valid.
		SKIP_WHITESPACE // Only tabs, spaces, line feeds & carriage returns are skipped.
	};

#define NO_IGNORE_CHAR '\x01' // a char not found in a valid ASCII numeric field

	template <typename charT, typename traits = std::char_traits<charT>>
	class basic_Stream : public basic_Print<charT, traits>
	{
	protected:
		unsigned long _timeout;		///< number of milliseconds to wait for the next char before aborting timed read
		unsigned long _startMillis; ///< used for timeout measurement
		/// read stream with timeout
		int timedRead()
		{
			int c;
			_startMillis = millis();
			do
			{
				c = read();
				if (c >= 0)
					return c;
			} while (millis() - _startMillis < _timeout);
			return -1; // -1 indicates timeout
		}
		/// peek stream with timeout
		int timedPeek()
		{
			int c;
			_startMillis = millis();
			do
			{
				c = peek();
				if (c >= 0)
					return c;
			} while (millis() - _startMillis < _timeout);
			return -1; // -1 indicates timeout
		}
		/// returns the next numeric digit in the stream or -1 if timeout
		int peekNextDigit(LookaheadMode lookahead, bool detectDecimal)
		{
			int c;
			while (1)
			{
				c = timedPeek();
				if (c < 0 ||
					c == '-' ||
					(c >= '0' && c <= '9') ||
					(detectDecimal && c == '.'))
					return c;

				switch (lookahead)
				{
				case SKIP_NONE:
					return -1; // Fail code.
				case SKIP_WHITESPACE:
					switch (c)
					{
					case ' ':
					case '\t':
					case '\r':
					case '\n':
						break;
					default:
						return -1; // Fail code.
					}
				case SKIP_ALL:
					break;
				}
				read(); // discard non-numeric
			}
		}

	public:
		virtual int available() = 0;
		virtual int read() = 0;
		virtual int peek() = 0;

		basic_Stream() : { _timeout = 1000; }

		// parsing methods

		// sets maximum milliseconds to wait for stream data, default is 1 second
		void setTimeout(unsigned long timeout) { _timeout = timeout; }
		unsigned long getTimeout(void) { return _timeout; }

		bool find(char *target); // reads data from the stream until the target string is found
		bool find(uint8_t *target) { return find((char *)target); }
		// returns true if target string is found, false if timed out (see setTimeout)

		// reads data from the stream until the target string of given length is found
		bool find(char *target, size_t length) { return findUntil(target, strlen(target), NULL, 0); }
		bool find(uint8_t *target, size_t length) { return find(reinterpret_cast<char *>(target), length); }
		// returns true if target string is found, false if timed out
		bool find(char target) { return find(&target, 1); }

		// as find but search ends if the terminator string is found
		bool findUntil(char *target, char *terminator) { return findUntil(target, strlen(target), terminator, strlen(terminator)); }

		bool findUntil(uint8_t *target, char *terminator) { return findUntil(static_cast<char *>(target), terminator); }

		// as above but search ends if the terminate string is found
		bool findUntil(char *target, size_t targetLen, char *terminate, size_t termLen)
		{
			if (terminator == NULL)
			{
				MultiTarget t[1] = {{target, targetLen, 0}};
				return findMulti(t, 1) == 0;
			}
			else
			{
				MultiTarget t[2] = {{target, targetLen, 0}, {terminator, termLen, 0}};
				return findMulti(t, 2) == 0;
			}
		}
		bool findUntil(uint8_t *target, size_t targetLen, char *terminate, size_t termLen) { return findUntil(static_cast<char *>(target), targetLen, terminate, termLen); }

		// returns the first valid (long) integer value from the current position.
		// lookahead determines how parseInt looks ahead in the stream.
		// See LookaheadMode enumeration at the top of the file.
		// Lookahead is terminated by the first character that is not a valid part of an integer.
		// Once parsing commences, 'ignore' will be skipped in the stream.
		long parseInt(LookaheadMode lookahead = SKIP_ALL, char ignore = NO_IGNORE_CHAR);

		// float version of parseInt
		float parseFloat(LookaheadMode lookahead = SKIP_ALL, char ignore = NO_IGNORE_CHAR);

		// read chars from stream into buffer
		virtual size_t readBytes(char *buffer, size_t length); 
		// terminates if length characters have been read or timeout (see setTimeout)
		// returns the number of characters placed in the buffer (0 means no valid data found)
		size_t readBytes(uint8_t *buffer, size_t length) { return readBytes((char *)buffer, length); }

		// as readBytes with terminator character
		size_t readBytesUntil(char terminator, char *buffer, size_t length); 
		// terminates if length characters have been read, timeout, or if the terminator character  detected
		// returns the number of characters placed in the buffer (0 means no valid data found)
		size_t readBytesUntil(char terminator, uint8_t *buffer, size_t length) { return readBytesUntil(terminator, (char *)buffer, length); }

		// Arduino String functions to be added here
		String readString();
		String readStringUntil(char terminator);

	protected:
		long parseInt(char ignore) { return parseInt(SKIP_ALL, ignore); }
		float parseFloat(char ignore) { return parseFloat(SKIP_ALL, ignore); }
		// These overload exists for compatibility with any class that has derived
		// Stream and used parseFloat/Int with a custom ignore character. To keep
		// the public API simple, these overload remains protected.

		struct MultiTarget
		{
			const char *str; // string you're searching for
			size_t len;		 // length of string you're searching for
			size_t index;	 // index used by the search routine.
		};

		// This allows you to search for an arbitrary number of strings.
		// Returns index of the target that is found first or -1 if timeout occurs.
		int findMulti(struct MultiTarget *targets, int tCount);
	};
#undef NO_IGNORE_CHAR
#endif
} // namespace

#if 1 || defined(DOCTEST_LIBRARY_INCLUDED) && defined(DOCTEST_NEW_STREAM)
#include <iostream>
#include <doctest/doctest.h>
#include "new_String.h"
#include "new_Printable.h"

TEST_SUITE("[new_Stream.h]")
{
	using namespace stduino;
}
#endif