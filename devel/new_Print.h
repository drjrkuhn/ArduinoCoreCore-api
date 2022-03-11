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

#include "String_helpers.h"
#include "new_String.h"
#include "new_Printable.h"

namespace stduino {

	template <typename charT, typename traits = std::char_traits<charT> >
	class basic_Printstream : public std::basic_streambuf<chartT, traits> {

	};

	/**
	 * Character based output class with an internal streambuf class.
	 * Derived classes may back
	 */
	template <typename charT, typename traits = std::char_traits<charT> >
	class basic_Print
	{
	protected:
		void setWriteError(int err = 1) { write_error_ = err; }
		
		std::streambuf& streambuf_;
		using STDSTR = typename std::basic_string<charT, traits>;
		using str_const_iterator = typename STDSTR::const_iterator;
	public:

		basic_Print(std::basic_streambuf<charT, traits> buf)
			: write_error_(0), streambuf_(buf) {}
		basic_Print() : write_error_(0), streambuf_(NOBUF) {}

		int getWriteError() { return write_error_; }
		void clearWriteError() { setWriteError(0); }

		/** Write a single byte. Must override in implementing class. */
		virtual size_t write(const uint8_t c) {
			streambuf_.sputc(c);
		}

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

	using Print = basic_Print<char>;

} // namespace

#if 1 || defined(DOCTEST_LIBRARY_INCLUDED) && defined(DOCTEST_NEW_PRINT)
#include <iostream>
#include <doctest/doctest.h>
#include "new_String.h"

TEST_SUITE("[new_Print.h]") {
	using namespace stduino;

	struct StringPrint : public Print, public String {
		StringPrint() : String("") {}
		StringPrint(const char* c_str) : String(c_str) {}
		virtual size_t write(const uint8_t c) { return String::concat(c) ? 1 : 0; }
		using Print::write;
		//using String::operator=;
	};

	TEST_CASE_TEMPLATE("write chars", C, char, unsigned char) {
		StringPrint p;
		CHECK(p.write(C('c')) == 1);
		CHECK(p == "c");
	}
	TEST_CASE("write") {
		String world("world");
		std::basic_string<char> stdworld("world");
		StringPrint lhs = "hello ";
		lhs.write(reinterpret_cast<uint8_t*>("world"), 5);
		CHECK(lhs == "hello world");
		lhs = "hello ";
		lhs.write("world", 5);
		CHECK(lhs == "hello world");
		lhs = "hello ";
		lhs.write(stdworld.begin(), stdworld.end());
		CHECK(lhs == "hello world");
		lhs = "hello ";
		lhs.write(world.c_str(), world.length());
		CHECK(lhs == "hello world");

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