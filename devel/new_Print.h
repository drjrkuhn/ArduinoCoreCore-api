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
#include <stdio.h> // for size_t
#include <string> // for char_traits

#include "String_helpers.h"
#include "new_String.h"
#include "new_Printable.h"


namespace stduino {

	template <typename charT, typename traits = std::char_traits<charT> >
	class basic_Print
	{
	protected:
		void setWriteError(int err = 1) { write_error = err; }
	public:
		using BASE = std::basic_string<charT, traits>;
		using charT_ptr = traits::pointer;
		using iterator = BASE::iterator;
		using const_iterator = BASE::const_iterator;

		basic_print(charT_ptr sbuf)
			: basic_ostream(sbuf), write_error(0) {
		}

		int getWriteError() { return write_error; }
		void clearWriteError() { setWriteError(0); }

		/** Write a single byte. Must override in implementing class. */
		virtual size_t write(const uint8_t c) = 0;

		/** Write a single character. May override in implementing class. */
		virtual size_t write(charT c) { return write(static_cast<uint8_t>(c)); }

		/** Write multiple bytes. May override in implementing class. */
		virtual size_t write(const uint8_t* buffer, size_t size) {
			if (buffer == NULL)
				return 0;
			size_t n = 0;
			while (size--) {
				if (write(*buffer++)) n++;
				else break;
			}
			return n;
		}

		size_t write(const charT_ptr buffer, size_t size) { return buffer == NULL ? 0 : write(static_cast<const uint8_t*>(buffer), size); }
		size_t write(const_iterator begin, const_iterator end) { write(begin, end - begin); }
		size_t write(const charT_ptr c_str) { return write(c_str, strlen(c_str)); }
		size_t write(const BASE std_str) { return write(std_str.begin(), std_str.length()); }

		/** default to zero, meaning "a single write may block"
		 * should be overridden by subclasses with buffering */
		virtual int availableForWrite() { return 0; }

		size_t print(charT c) { return write(c); }
		size_t print(const charT_ptr buffer, size_t size) { return write(buffer, size); }

		size_t print(const_iterator begin, const_iterator end()) { return print(begin, end - begin); }
		size_t print(const BASE std_str) { return print(std_str.begin(), std_str.end()); }
		size_t print(const String& s) { return print(s.c_str(), s.length()); }
		size_t print(const charT_ptr c_str) { return c_str == NULL ? 0 : print(c_str, strlen(c_str)); }
		size_t print(const __FlashStringHelper* ifsh) { return print(reinterpret_cast<const charT_ptr>(ifsh)); }
		size_t print(unsigned char b, int base = DEC) { return print((unsigned int)b, base); }
		template<class numT, typename std::enable_if_t<std::is_arithmetic_v<numT>, bool> = true>
		size_t print(numT number, unsigned char baseOrPlaces = baseOrPlaces<numT>()) { return print(to_stdsstring(number, baseOrPlaces)); }
		size_t print(const Printable& x) { return x.printTo(*this); }

		size_t println(void) { return write("\r\n"); }
		size_t println(charT c) { return write(c) + println(); }
		size_t println(const charT_ptr buffer, size_t size) { return print(buffer, size) + println(); }

		size_t println(const_iterator begin, const_iterator end()) { return print(begin, end) + println(); }
		size_t println(const BASE std_str) { return print(std_str) + println(); }
		size_t println(const String& s) { return print(s) + println(); }
		size_t println(const charT_ptr c_str) { return print(c_str) + println(); }
		size_t println(const __FlashStringHelper* ifsh) { return print(ifsh) + println(); }
		size_t println(unsigned char b, int base = DEC) { return print(b, base) + println(); }
		template<class numT, typename std::enable_if_t<std::is_arithmetic_v<numT>, bool> = true>
		size_t println(numT number, unsigned char baseOrPlaces = baseOrPlaces<numT>()) { return print(number, baseOrPlaces) + println(); }
		size_t println(const Printable& x) { return print(x) + println(); }

		virtual void flush() { /* Empty implementation for backward compatibility */ }
	private:
		int write_error;
	};

	using Print = basic_Print<char>;

} // namespace