#pragma once

#include <cstdint>
#include <streambuf>
#include <stdio.h> // for size_t
#include <string>
#include <limits>

namespace arduino {
	/**
	 * Adapts a std::ostream for printing single characters at a time.
	 *
	 * This version only uses the single-byte 'write' function of Print
	 * to write to the std::ostream.
	 */
	class Print_stdostream_put : public Print {
	public:
		Print_stdostream_put(std::ostream& os) : _ostream(os) {}
		std::ostream& ostream() { return _ostream; }

		/// write a single byte
		virtual size_t write(const uint8_t c) override {
			_ostream.put(static_cast<const char>(c));
			return _ostream.good() ? 1 : 0;
		}
		using Print::write;
		using Print::availableForWrite;
		using Print::print;
		using Print::println;
		using Print::flush;

		/// number of bytes available in write buffer - "infinite" in theory for ostreams
		virtual int availableForWrite() override { return std::numeric_limits<int>::max(); }

		template<typename T>
		std::ostream& operator<< (T t) { return ostream() << t; }
	protected:
		std::ostream& _ostream;
	};

	/**
	 * Adapts a std::ostream for printing.
	 *
	 * This version overrides both the single-byte 'write' and the
	 * multi-byte 'write' functions of Print to write to the std::ostream.
	 */
	class Print_stdostream : public Print {
	public:
		Print_stdostream(std::ostream& os) : _ostream(os) {}
		std::ostream& ostream() { return _ostream; }

		/// write a single byte
		virtual size_t write(const uint8_t c) override {
			_ostream.put(static_cast<const char>(c));
			return _ostream.good() ? 1 : 0;
		}
		/// write multible bytes
		virtual size_t write(const uint8_t* str, size_t n) override {
			auto first = _ostream.tellp();
			_ostream.write(reinterpret_cast<const char*>(str), n);
			return _ostream.good() ? n : _ostream.tellp() - first;
		}
		using Print::write;
		using Print::availableForWrite;
		using Print::print;
		using Print::println;
		using Print::flush;

		/// number of bytes available in write buffer - "infinite" in theory for ostreams
		virtual int availableForWrite() override { return std::numeric_limits<int>::max(); }

		template<typename T>
		std::ostream& operator<< (T t) { return ostream() << t; }
	protected:
		std::ostream& _ostream;
	};

	/**
	 * Adapts a std::ostringstream for printing to strings.
	 *
	 * This version keeps an internal stringstream/stringbuf
	 * as its stream. Access the current contents with @see str().
	 * Clear the buffer with @see clear().
	 */
	class Print_stdstring : public Print_std_ostream {
	public:
		Print_stdstring(const std::string str)
			: _oss(str, std::ios_base::out | std::ios_base::app), Print_std_ostream<charT, traits>(_oss) {
			// NOTE: open in append mode so we don't overwrite the intiial value
		}
		Print_std_string()
			: _oss(std::ios_base::out | std::ios_base::app), Print_std_ostream<charT, traits>(_oss) {
			// NOTE: open in append mode so we don't overwrite current contents
		}
		std::string str() const { return _oss.str(); }
		void str(const std::string s) { _oss.str(s); }
		void clear() { _oss.str(""); _oss.clear(); }
	protected:
		std::ostringstream _oss;
	};


} // namespace arduino