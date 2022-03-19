/*
  Stream.h - base class for character-based streams.
  Copyright (c) 2010 David A. Mellis.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  parsing functions based on TextFinder library by Michael Margolis
*/

#pragma once

#include <inttypes.h>
#include "Common.h"
#include "Print.h"

// compatibility macros for testing
/*
#define   getInt()            parseInt()
#define   getInt(ignore)      parseInt(ignore)
#define   getFloat()          parseFloat()
#define   getFloat(ignore)    parseFloat(ignore)
#define   getString( pre_string, post_string, buffer, length)
readBytesBetween( pre_string, terminator, buffer, length)
*/

namespace arduino {

	const unsigned long PARSE_TIMEOUT = 1000;  // default number of milli-seconds to wait


	// This enumeration provides the lookahead options for parseInt(), parseFloat()
	// The rules set out here are used until either the first valid character is found
	// or a time out occurs due to lack of input.
	enum LookaheadMode{
		SKIP_ALL,       // All invalid characters are ignored.
		SKIP_NONE,      // Nothing is skipped, and the stream is not touched unless the first waiting character is valid.
		SKIP_WHITESPACE // Only tabs, spaces, line feeds & carriage returns are skipped.
	};

#define NO_IGNORE_CHAR  '\x01' // a char not found in a valid ASCII numeric field

	// Base input interface for io streams
	class Stream_base
	{
	protected:
		virtual int timedRead() = 0;
		virtual int timedPeek() = 0;
		virtual int peekNextDigit(LookaheadMode lookahead, bool detectDecimal) = 0;
	public:
		virtual int available() = 0;
		virtual int read() = 0;
		virtual int peek() = 0;
		virtual bool find(const char* target) = 0;
		virtual bool find(const char* target, size_t length) = 0;
		virtual bool find(std::string target) = 0;
		virtual bool find(char target) = 0;
		virtual bool findUntil(const char* target, const char* terminator) = 0;
		virtual bool findUntil(const char* target, size_t targetLen, const char* terminator, size_t termLen) = 0;
		virtual long parseInt(LookaheadMode lookahead = SKIP_ALL, char ignore = NO_IGNORE_CHAR) = 0;
		virtual float parseFloat(LookaheadMode lookahead = SKIP_ALL, char ignore = NO_IGNORE_CHAR) = 0;
		virtual size_t readBytes(char* buffer, size_t length) = 0;
		virtual size_t readBytesUntil(char terminator, char* buffer, size_t length) = 0;
		virtual String readString() = 0;
		virtual String readStringUntil(char terminator) = 0;
		virtual std::string readStdString() = 0;
		virtual std::string readStdStringUntil(char terminator) = 0;
	protected:
		struct MultiTarget {
			const char* str;  // string you're searching for
			size_t len;       // length of string you're searching for
			size_t index;     // index used by the search routine.
		};
		virtual int findMulti(struct MultiTarget* targets, int tCount) = 0;
	};

	class Stream : public Stream_base, public Print
	{
	protected:
		unsigned long _timeout;      // number of milliseconds to wait for the next char before aborting timed read
		unsigned long _startMillis;  // used for timeout measurement
		virtual int timedRead() override;    // private method to read stream with timeout
		virtual int timedPeek() override;    // private method to peek stream with timeout
		virtual int peekNextDigit(LookaheadMode lookahead, bool detectDecimal) override; // returns the next numeric digit in the stream or -1 if timeout

	public:
		virtual int available() = 0;
		virtual int read() = 0;
		virtual int peek() = 0;

		Stream() { _timeout = 1000; }

		// parsing methods

		void setTimeout(unsigned long timeout);  // sets maximum milliseconds to wait for stream data, default is 1 second
		unsigned long getTimeout(void) { return _timeout; }

		virtual bool find(const char* target) override;   // reads data from the stream until the target string is found
		bool find(const uint8_t* target) { return find((const char*)target); }
		// returns true if target string is found, false if timed out (see setTimeout)

		virtual bool find(const char* target, size_t length) override;   // reads data from the stream until the target string of given length is found
		bool find(const uint8_t* target, size_t length) { return find((const char*)target, length); }
		// returns true if target string is found, false if timed out
		virtual bool find(const std::string target) override;

		virtual bool find(char target) override { return find(&target, 1); }

		virtual bool findUntil(const char* target, const char* terminator) override;   // as find but search ends if the terminator string is found
		bool findUntil(const uint8_t* target, const char* terminator) { return findUntil((const char*)target, terminator); }

		virtual bool findUntil(const char* target, size_t targetLen, const char* terminate, size_t termLen) override;   // as above but search ends if the terminate string is found
		bool findUntil(const uint8_t* target, size_t targetLen, const char* terminate, size_t termLen) { return findUntil((const char*)target, targetLen, terminate, termLen); }

		virtual long parseInt(LookaheadMode lookahead = SKIP_ALL, char ignore = NO_IGNORE_CHAR) override;
		// returns the first valid (long) integer value from the current position.
		// lookahead determines how parseInt looks ahead in the stream.
		// See LookaheadMode enumeration at the top of the file.
		// Lookahead is terminated by the first character that is not a valid part of an integer.
		// Once parsing commences, 'ignore' will be skipped in the stream.

		virtual float parseFloat(LookaheadMode lookahead = SKIP_ALL, char ignore = NO_IGNORE_CHAR) override;
		// float version of parseInt

		virtual size_t readBytes(char* buffer, size_t length) override; // read chars from stream into buffer
		size_t readBytes(uint8_t* buffer, size_t length) { return readBytes((char*)buffer, length); }
		// terminates if length characters have been read or timeout (see setTimeout)
		// returns the number of characters placed in the buffer (0 means no valid data found)

		virtual size_t readBytesUntil(char terminator, char* buffer, size_t length) override; // as readBytes with terminator character
		size_t readBytesUntil(char terminator, uint8_t* buffer, size_t length) { return readBytesUntil(terminator, (char*)buffer, length); }
		// terminates if length characters have been read, timeout, or if the terminator character  detected
		// returns the number of characters placed in the buffer (0 means no valid data found)

		template <typename CharIT, typename std::enable_if<is_char_iterator_v<CharIT>, bool>::type = false>
		std::string readBytes_for(CharIT begin, CharIT end) {
			return readBytes(&begin[0], end - begin);
		}

		// Arduino String functions to be added here
		virtual String readString() override;
		virtual String readStringUntil(char terminator) override;
		virtual std::string readStdString() override;
		virtual std::string readStdStringUntil(char terminator) override;

	protected:
		long parseInt(char ignore) { return parseInt(SKIP_ALL, ignore); }
		float parseFloat(char ignore) { return parseFloat(SKIP_ALL, ignore); }
		// These overload exists for compatibility with any class that has derived
		// Stream and used parseFloat/Int with a custom ignore character. To keep
		// the public API simple, these overload remains protected.

		// This allows you to search for an arbitrary number of strings.
		// Returns index of the target that is found first or -1 if timeout occurs.
		virtual int findMulti(struct MultiTarget* targets, int tCount) override;
	};

#undef NO_IGNORE_CHAR

}

using arduino::Stream;