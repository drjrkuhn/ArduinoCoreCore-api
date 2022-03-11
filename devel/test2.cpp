#include <iostream>
#include <string>
#include <sstream>
#include <cassert>
#include <algorithm>
#include <type_traits>

#if !defined(__PRETTY_FUNCTION__) && !defined(__GNUC__)
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

#if 0

//template <typename T, typename... Ts>
//struct is_any : std::disjunction<std::is_same<T, Ts>...> {};

template <typename T, typename... Ts>
constexpr bool is_any_v = std::disjunction<std::is_same<T, Ts>...>::value;

//template <typename T>
//struct is_mynumber : is_any<T, short, int, long, long long, unsigned short, unsigned int, unsigned long, unsigned long long> {};

template <typename T>
constexpr bool is_mynumber_v = is_any_v<T, short, int, long, long long, unsigned short, unsigned int, unsigned long, unsigned long long>;

//template <typename T>
//struct is_char : is_any<T, char, unsigned char> {};

template <typename T>
constexpr bool is_myarithmetic_v = is_mynumber_v<T> || std::is_floating_point_v<T>;

template <typename T>
constexpr bool is_char_v = is_any_v<T, char, unsigned char>;

template<typename T, std::enable_if_t<is_mynumber_v<T>, bool> = true>
size_t mynumbersize(T t) { return sizeof(t); }

template<typename T, std::enable_if_t<is_char_v<T>, bool> = true>
size_t mynumbersize(T t) { return 0; }


template<typename numT, std::enable_if_t<is_myarithmetic_v<numT>, bool> = true>
std::string numString(numT number) {
	std::string res = "integral ";
	return res.append(std::to_string(number));
}

std::string numString(char c) {
	std::string res = "character only ";
	return res.append(std::to_string(c));
}

int main(int argc, char* argv[])
{
	using namespace std;

	cout << "char size " << mynumbersize(char(100)) << endl;
	cout << "short size " << mynumbersize(short(100)) << endl;
	cout << "int size " << mynumbersize(int(100)) << endl;
	cout << "long size " << mynumbersize(long(100)) << endl;

	cout << "Test2" << endl;
	cout << "char\t\t" << numString(char('c')) << endl;
	cout << "uint8_t\t\t" << numString(uint8_t('c')) << endl;
	cout << "unsigned char\t" << numString(unsigned char('c')) << endl;
	cout << "int\t\t" << numString(int('c')) << endl;
	cout << "unsigned int\t" << numString(unsigned int('c')) << endl;
	cout << "float\t\t" << numString(float(1.23456e3)) << endl;
	cout << "double\t\t" << numString(double(1.23456e8)) << endl;

	return 0;
}
#endif

#if 0
int main(int argc, char* argv[])
{
	using namespace std;
	string A = "hello ";
	string B = "world";
	string C = A + B;
	cout << C << endl;
	string D = A + to_string(100);
	cout << D << endl;
	cout << A + ('w') << endl;

	return 0;
}

#endif

#if 0

//template<typename StdString>
using StdString = std::string;

class mystring : public StdString {
public:
	explicit mystring(const StdString& BASE) : StdString(BASE) { std::cerr << __PRETTY_FUNCTION__ << std::endl; }
	mystring(const mystring& other) : StdString(reinterpret_cast<const StdString&>(other)) { std::cerr << __PRETTY_FUNCTION__ << std::endl; }
	mystring(mystring&& rhs) : StdString(std::move(rhs)) { std::cerr << __PRETTY_FUNCTION__ << std::endl; }

	bool reserve(size_t size) {
		//reinterpret_cast<Stringable*>(this)->reserve(size);
		StdString::reserve(size);
		return true;
	}
	unsigned short capacity() {
		return static_cast<unsigned short>(StdString::capacity());
	}
};


int main(int argc, char* argv[])
{
	using namespace std;

	mystring foo("hello");
	string bar("world");
	cout << " reserving 300 " << boolalpha << foo.reserve(300) << endl;
	cout << " capacity " << foo.capacity() << endl;
	cout << "mystring cap return type " << sizeof(foo.capacity()) << endl;
	cout << "std::string cap return type " << sizeof(bar.capacity()) << endl;
	mystring foo2(foo);
	cout << "copy constructor foo: " << foo << " foo2: " << foo2 << endl;
	mystring foo3(std::move(foo));
	cout << "move constructor foo: " << foo << " foo3: " << foo3 << endl;

	string hello("hello");
	string hw = hello + bar + '!';

	string outstr = "appended:";
	using str_const_iterator = std::basic_string<char, std::char_traits<char>>::iterator;
	str_const_iterator begin = hello.begin();
	str_const_iterator end = hello.end();
	outstr.append(begin, end);
	cout << outstr << endl;

	return 0;
}

#endif

#if 1

using namespace std;

class stream_base {
public:
	virtual size_t write(char c) = 0;
	virtual int available() { return 0; }
	virtual int read() { return 0; }
	virtual int peek() { return 0; }
};

class streambuf_adapter : public streambuf {
public:
	stream_base* printer_;
	streambuf_adapter(stream_base* printer) : printer_(printer) { }
	streambuf_adapter() : printer_(NULL) {}
	void setBase(stream_base* p) { printer_ = p; }

	virtual streamsize xsputn(const char* buffer, streamsize size) {
		if (buffer == NULL) return 0;
		size_t n = 0;
		while (size--) {
			if (printer_->write(*buffer++)) n++;
			else break;
		}
		return n;
	}
	virtual streamsize xsgetn(char* buffer, streamsize size) {
		size_t count = 0;
		while (printer_->available() > 0) {
			int c = printer_->read();
			if (c < 0) break;
			*buffer++ = (char)c;
			count++;
		}
		return count;
	}
};

class print : public stream_base {
protected:
	streambuf* streambuf_;
public:
	print() : streambuf_(NULL) { }
	void setBuffer(streambuf* buf) {
		streambuf_ = buf;
	}

	virtual size_t write(char c) {
		return streambuf_->sputc(c);
	}
	virtual size_t write(char* s, size_t n) {
		if (s == nullptr) return 0;
		size_t nout = 0;
		while (n--) {
			if (write(*s++)) nout++;
			else break;
		}
		return nout;
	}
};

class myprint : public print {
protected:
	stringbuf sbuf_;
	streambuf_adapter adapt_;
public:
	myprint() 
		: print(), sbuf_() {
		adapt_.setBase(this);
		this->setBuffer(&adapt_);
	}
	using print::write;

	std::string str() {
		return sbuf_.str();
	}
};

int main(int argc, char* argv[])
{
	using namespace std;

	myprint pout;
	pout.write('h');
	pout.write("ello", 4);

	cout << pout.str() << endl;

	return 0;
}
#endif
