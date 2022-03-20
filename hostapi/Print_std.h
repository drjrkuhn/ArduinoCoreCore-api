#pragma once

#include <cstdint>
#include <streambuf>
#include <stdio.h>
#include <string>
#include <limits>
#include <ios>
#include <sstream>
#include <type_traits>
#include <Print.h>

namespace arduino {

	/**
	 * Adapts a OSTREAM for printing.
	 *
	 * This version overrides both the single-byte 'write' and the
	 * multi-byte 'write' functions of Print to write to the OSTREAM.
	 */
	template <class OSTREAM>
	class Print_stdostream : public Print {
	public:
		Print_stdostream(OSTREAM& os) : _ostream(os) {}
		OSTREAM& ostream() { return _ostream; }

		virtual size_t write(const uint8_t c) override {
			char cc = static_cast<char>(c);
			return _ostream.rdbuf()->sputc(cc) == cc ? 1 : 0;
		}

		virtual size_t write(const uint8_t* str, size_t n) override {
			return _ostream.rdbuf()->sputn(reinterpret_cast<const char*>(str), n);
		}

		virtual int availableForWrite() override {
			return std::numeric_limits<int>::max();
		}

		template<typename T>
		OSTREAM& operator<< (T t) { return ostream() << t; }
	protected:
		OSTREAM& _ostream;
	};

	/**
	 * Adapts a std::ostringstream for printing to strings.
	 *
	 * This version keeps an internal stringstream/stringbuf
	 * as its stream. Access the current contents with @see str().
	 * Clear the buffer with @see clear().
	 */
	class Print_stdstring : public Print_stdostream<std::ostringstream>, public Printable {
	public:
		Print_stdstring(const std::string str)
			: Print_stdostream(_oss), _oss(str, std::ios_base::out | std::ios_base::app) {
			// NOTE: open in append mode so we don't overwrite the intiial value
		}
		Print_stdstring()
			: Print_stdostream(_oss), _oss(std::ios_base::out | std::ios_base::app) {
		}

		std::string str() const { return _oss.str(); }
		void str(const std::string s) { _oss.str(s); }
		void clear() { _oss.str(""); }

		/// number of bytes available in write buffer. 
		/// For stringstream it will return 0 just before reallocating more buffer space
		virtual int availableForWrite() override {
			return static_cast<int>(str().capacity() - str().length());
		}
		virtual size_t printTo(Print& p) const override {
			return p.write(_oss.str());
		}
	protected:
		std::ostringstream _oss;
	};


} // namespace arduino