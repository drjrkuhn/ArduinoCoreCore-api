
#define TEMPLATE 0
#if TEMPLATE
#include <iostream>
int main(int argc, char* argv[])
{
	std::string name(argv[0]);
	std::cout << "running " << name.substr(name.find_last_of("\\/") + 1) << std::endl;

	return 0;
}
#endif

#if 0
#include <iostream>
#include <sstream>
#include <Stream.h>

using namespace std;

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
		return _canget ? _ios.rdbuf()->in_avail() : 0;
	}

	virtual int read() override {
		return _canget ? checkget(_ios.rdbuf()->sbumpc()) : -1;
	}
	virtual int peek() override {
		return _canget ? checkget(_ios.rdbuf()->sgetc()) : -1;
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

class Stream_stdstring : public Stream_stdstream<stringstream> {
public:
	Stream_stdstring(
		const std::string str, ios_base::openmode which = std::ios_base::in | std::ios_base::out | std::ios_base::app)
		: _ss(str, which), Stream_stdstream(_ss, (no_init_tag())) {
		init(_ss);
		// NOTE: open in append mode so we don't overwrite the intiial value
	}
	Stream_stdstring(ios_base::openmode which = std::ios_base::in | std::ios_base::out)
		: _ss(which), Stream_stdstream(_ss, (no_init_tag())) {
		init(_ss);
		// NOTE: open in append mode so we don't overwrite the intiial value
	}
	std::string str() const { return _ss.str(); }
	void str(const std::string s) { _ss.str(s); }
	void clear() { _ss.str(""); }

	std::string bufferStr() const {
		string buf = _ss.str();
		int len = buf.length(), g = _ios.tellg(), p = _ios.tellp();
		if (p < 0) p = len;
		bool samegp = g == p;
		if (g < 0) g = 0;
		int headlen = g;
		int taillen = len - p;
		string ptrs;
		if (headlen - 1 > 0) ptrs.append(headlen - 1, '.');
		ptrs.append(1, samegp ? '@' : '^');
		ptrs.append(buf.substr(g, p - g));
		if (!samegp) ptrs.append(1, 'v');
		return ptrs;
	}

protected:
	virtual size_t checkget(size_t input) override {
		std::streampos g = _ios.tellg(), p = _ios.tellp();
		if (p < 0) p = _ss.str().length();
		if (g > 0 && g == p) clear();
		return input;
	}
	stringstream _ss;
};

pair<string, string> ssformat(stringstream& ss)
{
	string str = ss.str();
	int len = str.length();
	int g = ss.tellg();
	int p = ss.tellp();
	int nvalid = int(p - g) - 1;
	string ptrs;
	if (g > 0) {
		ptrs.append(g, '.');
		ptrs.append(1, g == p ? '&' : 'g');
	}
	if (nvalid > 0) ptrs.append(nvalid, '^');
	if (g != p) ptrs.append(1, 'p');
	return pair<string, string>(str, ptrs);
}

void printss(stringstream& ss)
{
	pair<string, string> lines = ssformat(ss);
	cout << "\t\t\t" << lines.first << endl;
	cout << "\t\t\t" << lines.second << endl;
}

int main(int argc, char* argv[])
{
	std::string name(argv[0]);
	std::cout << "running " << name.substr(name.find_last_of("\\/") + 1) << std::endl;

	ios_base::openmode which = 0;
	which |= ios_base::in;
	//which |= ios_base::out;
	Stream_stdstring stream("c123", which);
	stream.setTimeout(10);

	stream.print("a 123 Hello ");
	cout << "\t\t\t" << stream.bufferStr() << endl;
	stream.print("world");
	cout << "\t\t\t" << stream.bufferStr() << endl;
	cout << "first char: " << char(stream.read()) << endl;
	cout << "\t\t\t" << stream.bufferStr() << endl;
	cout << "first int: " << stream.parseInt() << endl;
	cout << "\t\t\t" << stream.bufferStr() << endl;
	cout << "first word: '" << stream.readStdString() << "'" << endl;
	cout << "\t\t\t" << stream.bufferStr() << endl;
	cout << "next word: '" << stream.readStdString() << "'" << endl;
	//ss.str("");
	cout << "\t\t\t" << stream.bufferStr() << endl;
	stream.print(" goodbye!");
	cout << "\t\t\t" << stream.bufferStr() << endl;
	cout << "next word: '" << stream.readStdString() << "'" << endl;
	cout << "\t\t\t" << stream.bufferStr() << endl;

	return 0;
}
#endif


#if 1
#include <iostream>
#include <sstream>

class SillyInt {
public:
	SillyInt(const int n) : _silly(n) {}

	friend std::ostream& operator<<(std::ostream& os, SillyInt const& myObject) {
		os << "Silly integer = ";
		os << myObject._silly;
		return os;
	}

protected:
	int _silly;
};

int main(int, char* argv[])
{
	std::string name(argv[0]);
	std::cout << "running " << name.substr(name.find_last_of("\\/") + 1) << std::endl;

	using namespace std;
	SillyInt baz(10);
	cout << baz << endl;
	return 0;
}
#endif
