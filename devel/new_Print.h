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

		using str_const_iterator = typename std::string::const_iterator;
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

		/** Write characters using string iterators */
		virtual size_t write(str_const_iterator begin, str_const_iterator end) {
			size_t n = 0;
			auto it = begin;
			while (it != end) {
				if (write(static_cast<const uint8_t>(*it++))) n++;
				else break;
			}
			return n;
		}

		/** Write character buffer */
		size_t write(const charT* buffer, size_t size) {
			return buffer == NULL ? 0 : write(reinterpret_cast<const uint8_t*>(buffer), size);
		}
		/** Write null-terminated string */
		size_t write(const charT* c_str) {
			return write(c_str, strlen(c_str));
		}

		/** default to zero, meaning "a single write may block"
		 * should be overridden by subclasses with buffering */
		virtual int availableForWrite() { return 0; }

		size_t print(charT c) { return write(c); }
		size_t print(const charT* buffer, size_t size) { return write(buffer, size); }
		size_t print(str_const_iterator begin, str_const_iterator end) { return write(begin, end); }
		size_t print(const std::string& std_str) { return write(std_str.begin(), std_str.end()); }
		size_t print(const String& s) { return print(s.c_str(), s.length()); }
		size_t print(const charT* c_str) { return c_str == NULL ? 0 : print(c_str, strlen(c_str)); }
		size_t print(const __FlashStringHelper* f_str) { return print(reinterpret_cast<const charT*>(f_str)); }
		size_t print(unsigned char b, int base = DEC) { return print<unsigned int>(b, base); }
		template <typename T, std::enable_if_t< is_nonchar_arithmetic_v<T>, bool> = true>
		size_t print(const T number, int baseOrPlaces = ::stduino::baseOrPlaces<T>()) {
			return print(to_stdsstring<charT, T>(number, baseOrPlaces));
		}
		size_t print(const Printable& x) { return x.printTo(*this); }

		size_t println(void) { return write("\r\n"); }
		size_t println(charT c) { return write(c) + println(); }
		size_t println(const charT* buffer, size_t size) { return write(buffer, size) + println(); }
		size_t println(str_const_iterator begin, str_const_iterator end) { return write(begin, end) + println(); }
		size_t println(const std::string& std_str) { return write(std_str.begin(), std_str.end()) + println(); }
		size_t println(const String& s) { return println(s.c_str(), s.length()); }
		size_t println(const charT* c_str) { return c_str == NULL ? 0 : println(c_str, strlen(c_str)); }
		size_t println(const __FlashStringHelper* f_str) { return println(reinterpret_cast<const charT*>(f_str)); }
		size_t println(unsigned char b, int base = DEC) { return println<unsigned int>(b, base); }
		template <typename T, std::enable_if_t< is_nonchar_arithmetic_v<T>, bool> = true>
		size_t println(const T number, int baseOrPlaces = ::stduino::baseOrPlaces<T>()) {
			return print(to_stdsstring<charT, T>(number, baseOrPlaces)) + println();
		}
		size_t println(const Printable& x) { return x.printTo(*this) + println(); }

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
	template <typename charT, typename traits = std::char_traits<charT> >
	class basic_Print_ocharstream : public basic_Print<charT, traits> {
	public:
		using stdostream = std::basic_ostream<charT, traits>;
		using Print = basic_Print<charT, traits>;
		basic_Print_ocharstream(stdostream& os) : ostream_(os) {}
		stdostream& ostream() { return ostream_; }

		/// write a single byte
		virtual size_t write(const uint8_t c) override {
			ostream_.put(static_cast<const char>(c));
			return ostream_.good() ? 1 : 0;
		}
		using Print::write;
		using Print::availableForWrite;
		using Print::print;
		using Print::println;
		using Print::flush;

		/// number of bytes available in write buffer - "infinite" in theory for ostreams
		virtual int availableForWrite() override { return std::numeric_limits<int>::max(); }

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
		using Print = basic_Print<charT, traits>;
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
		using Print::write;
		using Print::availableForWrite;
		using Print::print;
		using Print::println;
		using Print::flush;

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
			: oss_(str, std::ios_base::out | std::ios_base::app), basic_Print_ostream<charT,traits>(oss_) {
			// NOTE: open in append mode so we don't overwrite the intiial value
		}
		basic_Print_string()
			: oss_(std::ios_base::out | std::ios_base::app), basic_Print_ostream<charT,traits>(oss_) {
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

#if defined(DOCTEST_LIBRARY_INCLUDED) && defined(DOCTEST_NEW_PRINT)
#include <iostream>
#include <doctest/doctest.h>
#include "new_String.h"
#include "new_Printable.h"

TEST_SUITE("[new_Print.h]") {
	using namespace stduino;

#define CRLF	"\r\n"
#define nCRLF	2

	struct IntPrintable : Printable {
		int num_;
		IntPrintable(int num) : num_(num) {}
		virtual size_t printTo(Print& p) const override {
			return p.print(num_, DEC);
		}
	};

	TEST_CASE("Print_ocharstream") {
		std::ostringstream ss;
		Print_ocharstream ps(ss);
		ss << "hello";
		CHECK(ps.write(' ') == 1);
		CHECK(ps.write("world", 5) == 5);
		CHECK(ss.str() == "hello world");
	}

	TEST_CASE("Print_ostream") {
		std::ostringstream ss;
		ss << "hello";
		Print_ostream ps(ss);
		CHECK(ps.write(' ') == 1);
		CHECK(ps.write("world", 5) == 5);
		CHECK(ss.str() == "hello world");
	}

	TEST_CASE("Print_string") {
		Print_string ps("hello");
		CHECK(ps.print(' ') == 1);
		CHECK(ps.print("world", 5) == 5);
		CHECK(ps.str() == "hello world");
		ps.str("goodbye ");
		CHECK(ps.str() == "goodbye ");
		CHECK(ps.print("world") == 5);
		CHECK(ps.str() == "goodbye world");
		ps.clear();
		CHECK(ps.str().empty());
		ps << "aaa ";
		CHECK(ps.str() == "aaa ");
		ps << 100;
		CHECK(ps.str() == "aaa 100");
	}

	TEST_CASE_TEMPLATE("write chars", C, char, unsigned char) {
		Print_string ps;
		CHECK(ps.write(C('c')) == 1);
		CHECK(ps.str() == "c");
	}
	TEST_CASE("write") {
		Print_string ps;
		CHECK(ps.availableForWrite() > 0);
		CHECK(ps.write(static_cast<unsigned char>('h')) == 1);
		CHECK(ps.write('e') == 1);
		CHECK(ps.write(reinterpret_cast<const uint8_t*>("llo "), 4) == 4);
		CHECK(ps.write("world", 5) == 5);
		CHECK(ps.str() == "hello world");
		ps.str("goodbye ");
		CHECK(ps.str() == "goodbye ");
		std::string world("world");
		CHECK(ps.write(world.begin(), world.end()) == 5);
		CHECK(ps.str() == "goodbye world");
		ps.str("hello ");
		CHECK(ps.write("world") == 5);
		CHECK(ps.str() == "hello world");
	}
	TEST_CASE("print") {
		Print_string ps;
		std::string stdstring("hello");
		String world("world");
		ps.clear();
		CHECK(ps.print('c') == 1);
		CHECK(ps.str() == "c");
		ps.clear();
		CHECK(ps.print("world") == 5);
		CHECK(ps.str() == "world");
		ps.clear();
		CHECK(ps.print(stdstring.begin(), stdstring.end()) == 5);
		CHECK(ps.str() == "hello");
		ps.clear();
		CHECK(ps.print(stdstring) == 5);
		CHECK(ps.str() == "hello");
		ps.clear();
		CHECK(ps.print("world") == 5);
		CHECK(ps.str() == "world");
		ps.clear();
		CHECK(ps.print(F("world")) == 5);
		CHECK(ps.str() == "world");
		ps.clear();
	}
	TEST_CASE_TEMPLATE("print integrals", N, unsigned char, SU_INTEGRALS) {
		Print_string ps;
		ps.print(N(100));
		CHECK(ps.str() == "100");
	}
	TEST_CASE_TEMPLATE("print integrals HEX", N, unsigned char, SU_INTEGRALS) {
		Print_string ps;
		ps.print(N(100), HEX);
		CHECK(ps.str() == "64");
	}
	TEST_CASE_TEMPLATE("print signed integrals", N, SU_SIGNED_INTEGRALS) {
		Print_string ps;
		ps.print(N(-100));
		CHECK(ps.str() == "-100");
	}
	TEST_CASE_TEMPLATE("print floats", N, SU_FLOATS) {
		Print_string ps;
		ps.print(N(100));
		CHECK(ps.str() == "100.00");
		ps.clear();
		ps.print(N(-100));
		CHECK(ps.str() == "-100.00");
	}
	TEST_CASE_TEMPLATE("print floats 4 places", N, SU_FLOATS) {
		Print_string ps;
		ps.print(N(100), 4);
		CHECK(ps.str() == "100.0000");
	}
	TEST_CASE("print Printable") {
		IntPrintable ip(100);
		Print_string ps;
		ps.print(ip);
		CHECK(ps.str() == "100");
	}
	TEST_CASE("println") {
		Print_string ps;
		CHECK(ps.println() == nCRLF);
		CHECK(ps.str() == CRLF);

		std::string stdstring("hello");
		String world("world");
		ps.clear();
		CHECK(ps.println('c') == 1 + nCRLF);
		CHECK(ps.str() == "c" CRLF);
		ps.clear();
		CHECK(ps.println("world") == 5 + nCRLF);
		CHECK(ps.str() == "world" CRLF);
		ps.clear();
		CHECK(ps.println(stdstring.begin(), stdstring.end()) == 5 + nCRLF);
		CHECK(ps.str() == "hello" CRLF);
		ps.clear();
		CHECK(ps.println(stdstring) == 5 + nCRLF);
		CHECK(ps.str() == "hello" CRLF);
		ps.clear();
		CHECK(ps.println("world") == 5 + nCRLF);
		CHECK(ps.str() == "world" CRLF);
		ps.clear();
		CHECK(ps.println(F("world")) == 5 + nCRLF);
		CHECK(ps.str() == "world" CRLF);
		ps.clear();
	}
	TEST_CASE_TEMPLATE("println integrals", N, unsigned char, SU_INTEGRALS) {
		Print_string ps;
		ps.println(N(100));
		CHECK(ps.str() == "100" CRLF);
	}
	TEST_CASE_TEMPLATE("println integrals HEX", N, unsigned char, SU_INTEGRALS) {
		Print_string ps;
		ps.println(N(100), HEX);
		CHECK(ps.str() == "64" CRLF);
	}
	TEST_CASE_TEMPLATE("println signed integrals", N, SU_SIGNED_INTEGRALS) {
		Print_string ps;
		ps.println(N(-100));
		CHECK(ps.str() == "-100" CRLF);
	}
	TEST_CASE_TEMPLATE("println floats", N, SU_FLOATS) {
		Print_string ps;
		ps.println(N(100));
		CHECK(ps.str() == "100.00" CRLF);
		ps.clear();
		ps.println(N(-100));
		CHECK(ps.str() == "-100.00" CRLF);
	}
	TEST_CASE_TEMPLATE("println floats 4 places", N, SU_FLOATS) {
		Print_string ps;
		ps.println(N(100), 4);
		CHECK(ps.str() == "100.0000" CRLF);
	}
	TEST_CASE("println Printable") {
		IntPrintable ip(100);
		Print_string ps;
		ps.println(ip);
		CHECK(ps.str() == "100" CRLF);
	}
}

#endif // DOCTEST_LIBRARY_INCLUDED