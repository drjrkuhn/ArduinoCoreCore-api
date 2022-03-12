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
#include <string> // for char_traits
#include <iterator>	// for char iterators
#include <limits>

#include "String_helpers.h"
#include "new_String.h"
#include "new_Printable.h"

namespace stduino {

	/**
	 * Character based output class with an internal streambuf class.
	 * Derived classes may back
	 */
	template <typename charT, typename traits = std::char_traits<charT> >
	class basic_Print
	{
	protected:
		void setWriteError(int err = 1) { write_error_ = err; }
		
		using STDSTR = typename std::basic_string<charT, traits>;
		using str_const_iterator = typename STDSTR::const_iterator;
	public:

		basic_Print() : write_error_(0) {}

		int getWriteError() { return write_error_; }
		void clearWriteError() { setWriteError(0); }

		/** Write a single byte. Must override in implementing class. */
		virtual size_t write(const uint8_t c) = 0;

		/** Write multiple bytes. May override in implementing class. */
		virtual size_t write(const uint8_t* buffer, size_t size) {
			if (buffer == NULL)	return 0;
			size_t n = 0;
			while (size--) {
				if (write(*buffer++)) n++;
				else break;
			}
			return n;
		}

		virtual size_t write(str_const_iterator begin, str_const_iterator end) {
			size_t n = 0; 
			auto it = begin;
			while (it != end) {
				if (write(static_cast<const uint8_t>(*it++))) n++;
				else break;
			}
			return n;
		}

		size_t write(const charT* buffer, size_t size) { 
			return buffer == NULL ? 0 : write(reinterpret_cast<const uint8_t*>(buffer), size); 
		}
		//size_t write(const charT* c_str) { 
		//	return write(c_str, strlen(c_str)); 
		//}
		//size_t write(const STDSTR std_str) { 
		//	return write(std_str.begin(), std_str.end()); 
		//}

		/** default to zero, meaning "a single write may block"
		 * should be overridden by subclasses with buffering */
		virtual int availableForWrite() { return 0; }

		size_t print(charT c) { return write(c); }
		size_t print(const charT* buffer, size_t size) { return write(buffer, size); }

		size_t print(str_const_iterator begin, str_const_iterator end()) { return print(begin, end - begin); }
		size_t print(const STDSTR std_str) { return print(std_str.begin(), std_str.end()); }
		size_t print(const String& s) { return print(s.c_str(), s.length()); }
		size_t print(const charT* c_str) { return c_str == NULL ? 0 : print(c_str, strlen(c_str)); }
		size_t print(const __FlashStringHelper* ifsh) { return print(reinterpret_cast<const charT*>(ifsh)); }
		size_t print(unsigned char b, int base = DEC) { return print((unsigned int)b, base); }
		template<class numT, typename std::enable_if_t<std::is_arithmetic_v<numT>, bool> = true>
		size_t print(numT number, unsigned char baseOrPlaces = baseOrPlaces<numT>()) { return print(to_stdsstring(number, baseOrPlaces)); }
		size_t print(const Printable& x) { return x.printTo(*this); }

		size_t println(void) { return write("\r\n"); }
		size_t println(charT c) { return write(c) + println(); }
		size_t println(const charT* buffer, size_t size) { return print(buffer, size) + println(); }

		size_t println(str_const_iterator begin, str_const_iterator) { return print(begin, end) + println(); }
		size_t println(const STDSTR std_str) { return print(std_str) + println(); }
		size_t println(const String& s) { return print(s) + println(); }
		size_t println(const charT* c_str) { return print(c_str) + println(); }
		size_t println(const __FlashStringHelper* ifsh) { return print(ifsh) + println(); }
		size_t println(unsigned char b, int base = DEC) { return print(b, base) + println(); }
		template<class numT, typename std::enable_if_t<std::is_arithmetic_v<numT>, bool> = true>
		size_t println(numT number, unsigned char baseOrPlaces = baseOrPlaces<numT>()) { return print(number, baseOrPlaces) + println(); }
		size_t println(const Printable& x) { return print(x) + println(); }

		virtual void flush() { /* Empty implementation for backward compatibility */ }
	private:
		int write_error_;
	};

	/**
	 * Adapts a std::ostream for printing single characters at a time.
	 * 
	 * This version only uses the single-byte 'write' function of Print
	 * to write to the std::ostream.
	 */
	template <typename charT,typename traits = std::char_traits<charT> >
	class basic_Print_ocharstream : public basic_Print<charT, traits> {
	public:
		using stdostream = std::basic_ostream<charT, traits>;
		basic_Print_ocharstream(stdostream& os) : ostream_(os) {}
		stdostream& ostream() { return ostream_; }

		/// write a single byte
		virtual size_t write(const uint8_t c) override {
			ostream_.put(static_cast<const char>(c));
			return ostream_.good() ? 1 : 0;
		}
		/// import other versions of write
		using basic_Print<charT, traits>::write;

		/// number of bytes available in write buffer - "infinite" in theory for ostreams
		virtual int availableForWrite() override {	return std::numeric_limits<int>::max(); }

		template<typename T>
		stdostream& operator<< (T t) { return ostream() << t; }
	protected:
		stdostream& ostream_;
	};

	/**
	 * Adapts a std::ostream for printing.
	 * 
	 * This version overrides both the single-byte 'write' and the
	 * multi-byte 'write' functions of Print to write to the std::ostream.
	 */
	template <typename charT, typename traits = std::char_traits<charT> >
	class basic_Print_ostream : public basic_Print<charT, traits> {
	public:
		using stdostream = std::basic_ostream<charT, traits>;
		basic_Print_ostream(stdostream& os) : ostream_(os) {}
		stdostream& ostream() { return ostream_; }

		/// write a single byte
		virtual size_t write(const uint8_t c) override {
			ostream_.put(static_cast<const char>(c));
			return ostream_.good() ? 1 : 0;
		}
		/// write multible bytes
		virtual size_t write(const uint8_t* str, size_t n) override {
			auto first = ostream_.tellp();
			ostream_.write(reinterpret_cast<const char*>(str), n);
			return ostream_.good() ? n : ostream_.tellp() - first;
		}
		/// import other versions of write
		using basic_Print<charT, traits>::write;

		/// number of bytes available in write buffer - "infinite" in theory for ostreams
		virtual int availableForWrite() override { return std::numeric_limits<int>::max(); }

		template<typename T>
		stdostream& operator<< (T t) { return ostream() << t; }
	protected:
		stdostream& ostream_;
	};

	/**
	 * Adapts a std::ostringstream for printing to strings.
	 *
	 * This version keeps an internal stringstream/stringbuf
	 * as its stream. Access the current contents with @see str().
	 * Clear the buffer with @see clear().
	 */
	template <typename charT, typename traits = std::char_traits<charT> >
	class basic_Print_string : public basic_Print_ostream<charT, traits> {
	public:
		basic_Print_string(const std::string str) 
			: oss_(str, std::ios_base::out | std::ios_base::app), basic_Print_ostream(oss_) {
			// NOTE: open in append mode so we don't overwrite the intiial value
		}
		basic_Print_string() 
			: oss_(std::ios_base::out | std::ios_base::app), basic_Print_ostream(oss_) {
			// NOTE: open in append mode so we don't overwrite current contents
		}
		std::string str() const { return oss_.str(); }
		void str(const std::string s) { oss_.str(s); }
		void clear() { oss_.str(""); oss_.clear(); }
	protected:
		std::ostringstream oss_;
	};

	using Print = basic_Print<char>;
	using Print_ocharstream = basic_Print_ocharstream<char>;
	using Print_ostream = basic_Print_ostream<char>;
	using Print_string = basic_Print_string<char>;

} // namespace

#if 1 || defined(DOCTEST_LIBRARY_INCLUDED) && defined(DOCTEST_NEW_PRINT)
#include <iostream>
#include <doctest/doctest.h>
#include "new_String.h"

TEST_SUITE("[new_Print.h]") {
	using namespace stduino;

	TEST_CASE("Print_ocharstream") {
		std::ostringstream ss;
		Print_ocharstream lhs(ss);
		ss << "hello";
		CHECK(lhs.write(' ') == 1);
		CHECK(lhs.write("world", 5) == 5);
		CHECK(ss.str() == "hello world");
	}

	TEST_CASE("Print_ostream") {
		std::ostringstream ss;
		ss << "hello";
		Print_ostream lhs(ss);
		CHECK(lhs.write(' ') == 1);
		CHECK(lhs.write("world", 5) == 5);
		CHECK(ss.str() == "hello world");
	}

	TEST_CASE("Print_string") {
		Print_string lhs("hello");
		CHECK(lhs.print(' ') == 1);
		CHECK(lhs.print("world", 5) == 5);
		CHECK(lhs.str() == "hello world");
		lhs.str("goodbye ");
		CHECK(lhs.str() == "goodbye ");
		CHECK(lhs.print("world") == 5);
		CHECK(lhs.str() == "goodbye world");
		lhs.clear();
		CHECK(lhs.str().empty());
		lhs << "aaa ";
		CHECK(lhs.str() == "aaa ");
		lhs << 100;
		CHECK(lhs.str() == "aaa 100");
	}

	TEST_CASE_TEMPLATE("write chars", C, char, unsigned char) {
		Print_string p;
		CHECK(p.write(C('c')) == 1);
		CHECK(p.str() == "c");
	}
	TEST_CASE("write") {
		Print_string lhs;
		CHECK(lhs.write(static_cast<unsigned char>('h')) == 1);
		CHECK(lhs.write('e') == 1);
		CHECK(lhs.write(reinterpret_cast<uint8_t*>("llo "), 4) == 4);
		CHECK(lhs.write("world", 5) == 5);
		CHECK(lhs.str() == "hello world");
		lhs.str("goodbye ");
		CHECK(lhs.str() == "goodbye ");
		std::string world("world");
		CHECK(lhs.write(world.begin(), world.end()) == 5);
		CHECK(lhs.str() == "goodbye world");
	}
	TEST_CASE("assignment") {
		//lhs.clear();
		//CHECK((lhs = other) == "other");
		//CHECK((lhs = stdother) == "stdother");
		//CHECK((lhs = "hello") == "hello");
		//CHECK((lhs = F("hello")) == "hello");
		//// copy assignment
		//CHECK((lhs = other) == "other");
		//CHECK(other == "other");
		//// move assignment
		//CHECK((lhs = std::move(other)) == "other");
		//CHECK(other == "");
		//lhs.clear();
		//CHECK(lhs == "");
	}
}

#endif // DOCTEST_LIBRARY_INCLUDED