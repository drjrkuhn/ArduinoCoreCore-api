
#if TEMPLATE
#include <iostream>
int main(int argc, char* argv[])
{
	std::string name(argv[0]);
	std::cout << "running " << name.substr(name.find_last_of("\\/") + 1) << std::endl;

	return 0;
}
#endif

#if 1
#include <iostream>
#include <sstream>
#include <Stream.h>

using namespace std;

class Stream_buf : public Stream {
public:
	Stream_buf(stringstream& ios) : _ios(ios) {}

	virtual size_t write(const uint8_t byte) override {
		char cc = static_cast<char>(byte);
		return checkio(_ios.rdbuf()->sputc(cc) == cc ? 1 : 0);
	}
	virtual size_t write(const uint8_t* str, size_t n) override {
		return checkio(_ios.rdbuf()->sputn(reinterpret_cast<const char*>(str), n));
	}
	virtual int availableForWrite() override {
		return std::numeric_limits<int>::max();
	}

	//using Print::write;
	//using Print::print;
	//using Print::println;
	//using Print::flush;

	virtual int available() override {
		return _ios.rdbuf()->in_avail();
	}

	virtual int read() override {
		return checkio(_ios.rdbuf()->sbumpc());
	}
	virtual int peek() override {
		return checkio(_ios.rdbuf()->sgetc());
	}
	virtual size_t readBytes(char* buffer, size_t length) override {
		return checkio(_ios.rdbuf()->sgetn(buffer, length));
	}

protected:
	virtual size_t checkio(size_t input) {
		if (_ios.tellg() == _ios.tellp()) {
			//_ios.str("");
			_ios.sync();
			_ios.flush();
		}
		return input;
	}
	stringstream& _ios;
};

pair<string, string> ssformat(stringstream& ss)
{
	string str = ss.str();
	size_t len = str.length();
	size_t g = ss.tellg();
	size_t p = ss.tellp();
	int nvalid = int(p - g) - 1;
	string ptrs;
	if (g > 0) ptrs.append(g, '.');
	ptrs.append(1, g==p ? '&' : 'g');
	if (nvalid > 0) ptrs.append(nvalid, '^');
	if (g != p ) ptrs.append(1, 'p');
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

	stringstream ss("", ios_base::in | ios_base::out);

	Stream_buf stream(ss);
	stream.setTimeout(10);

	stream.print("a 123 Hello ");
	printss(ss);
	stream.print("world");
	printss(ss);
	cout << "first char: " << char(stream.read()) << endl;
	printss(ss);
	cout << "first int: " << stream.parseInt() << endl;
	printss(ss);
	cout << "first word: '" << stream.readStdString() << "'" << endl;
	printss(ss);
	cout << "next word: '" << stream.readStdString() << "'" << endl;
	//ss.str("");
	printss(ss);
	stream.print(" goodbye!");
	printss(ss);
	cout << "next word: '" << stream.readStdString() << "'" << endl;
	printss(ss);

	return 0;
}
#endif

