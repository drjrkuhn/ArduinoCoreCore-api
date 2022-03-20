#pragma once

#include <iostream>
#include <sstream>

namespace arduino {

	template <class IOSTREAM>
	class Stream_stdstream : public Stream {
	public:
		Stream_stdstream(IOSTREAM& ios) : _ios(ios) {
			init(_ios);
		}
		IOSTREAM& ios() { return _ios; }

		virtual size_t write(const uint8_t byte) override {
			char cc = static_cast<char>(byte);
			return _canput && _ios.rdbuf()->sputc(cc) == cc ? 1 : 0;
		}
		virtual size_t write(const uint8_t* str, size_t n) override {
			return _canput ? _ios.rdbuf()->sputn(reinterpret_cast<const char*>(str), n) : 0;
		}
		virtual int availableForWrite() override {
			return _canput ? std::numeric_limits<int>::max() : 0;
		}

		virtual int available() override {
			return static_cast<int>(_canget ? _ios.rdbuf()->in_avail() : 0);
		}

		virtual int read() override {
			return static_cast<int>(_canget ? checkget(_ios.rdbuf()->sbumpc()) : -1);
		}
		virtual int peek() override {
			return static_cast<int>(_canget ? checkget(_ios.rdbuf()->sgetc()) : -1);
		}
		virtual size_t readBytes(char* buffer, size_t length) override {
			return _canget ? checkget(_ios.rdbuf()->sgetn(buffer, length)) : 0;
		}

	protected:
		virtual size_t checkget(size_t input) {
			return input;
		}
		void init(IOSTREAM& ios) { _canget = ios.tellg() >= 0; _canput = ios.tellp() >= 0; }
		// protected default constructor for derived
		struct no_init_tag {};
		Stream_stdstream(IOSTREAM& ios, no_init_tag) : _ios(ios) {}

		IOSTREAM& _ios;
		bool _canget, _canput;
	};

	class Stream_stdstring : public Stream_stdstream<std::stringstream> {
	public:
		Stream_stdstring(
			const std::string str, std::ios_base::openmode which = std::ios_base::in | std::ios_base::out | std::ios_base::app)
			: Stream_stdstream(_ss, (no_init_tag())), _ss(str, which) {
			init(_ss);
			// NOTE: open in append mode so we don't overwrite the intiial value
		}
		Stream_stdstring(std::ios_base::openmode which = std::ios_base::in | std::ios_base::out)
			: Stream_stdstream(_ss, (no_init_tag())), _ss(which) {
			init(_ss);
			// NOTE: open in append mode so we don't overwrite the intiial value
		}
		std::string str() const { return _ss.str(); }
		void str(const std::string s) { _ss.str(s); }
		void clear() { _ss.str(""); }

		std::string bufferStr() const {
			std::string buf = _ss.str();
			long len = static_cast<long>(buf.length());
			long g = static_cast<long>(_ios.tellg());
			long p = static_cast<long>(_ios.tellp());
			if (p < 0)
				p = len;
			bool samegp = g == p;
			if (g < 0)
				g = 0;
			long headlen = g;
			long taillen = len - p;
			std::string ptrs;
			if (headlen - 1 > 0)
				ptrs.append((headlen - 1), '.');
			ptrs.append(1, samegp ? '@' : '^');
			ptrs.append(buf.substr(g, p - g));
			if (!samegp)
				ptrs.append(1, 'v');
			return ptrs;
		}

	protected:
		virtual size_t checkget(size_t input) override {
			std::streampos g = _ios.tellg(), p = _ios.tellp();
			if (p < 0) p = _ss.str().length();
			if (g > 0 && g == p) clear();
			return input;
		}
		std::stringstream _ss;
	};

} // namespace arduino