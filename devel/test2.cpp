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

#if 0

using namespace std;

class print_base {
public:
	virtual size_t write(char c) = 0;
	virtual size_t write(const char* str, size_t n) {
		//if (str == nullptr) return 0;
		//size_t nout = 0;
		//while (n--) {
		//	if (write(*str++)) nout++;
		//	else break;
		//}
		//return nout;
		auto last = find_if(str, str + n, [this](char c) { return write(c) != 1; });
		return last - str;
	}
	size_t print(const char c) {
		return write(c);
	}
	size_t print(const char* str) {
		return write(str, strlen(str));
	}
};

class print_putstream : public print_base {
public:
	print_putstream(ostream& os) : ostream_(os) {}
protected:
	virtual size_t write(char c) override {
		ostream_.put(c);
		return ostream_.good() ? 1 : 0;
	}
	ostream& ostream_;
};

class print_writestream : public print_base {
public:
	print_writestream(ostream& os) : ostream_(os) {}
protected:
	virtual size_t write(char c) override {
		ostream_.put(c);	
		return ostream_.good() ? 1 : 0;
	}
	virtual size_t write(const char* str, size_t n) {
		auto first = ostream_.tellp();
		ostream_.write(str, n);
		auto last = ostream_.tellp();
		size_t written = last - first;
		return written;
		//return os_.good() ? n : written;
	}
	ostream& ostream_;
};

int main(int argc, char* argv[])
{
	using namespace std;


	ostringstream ssout("");

	print_putstream pprint(ssout);
	assert(pprint.print('h') == 1);
	assert(pprint.print("ello") == 4);

	cout << ssout.str() << endl;

	ssout.str("");
	ssout.clear();

	print_writestream wprint(ssout);
	assert(wprint.print('w') == 1);
	assert(wprint.print("orld") == 4);
	assert(wprint.print("\r\n") == 2);
	char* bigmsg = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ullamcorper eget nulla facilisi etiam. Viverra orci sagittis eu volutpat. Pulvinar neque laoreet suspendisse interdum consectetur libero id faucibus nisl. Ac turpis egestas sed tempus urna et pharetra pharetra. Leo in vitae turpis massa sed elementum. At risus viverra adipiscing at in tellus. Tempor commodo ullamcorper a lacus vestibulum sed arcu non odio. Vulputate dignissim suspendisse in est ante in. Est pellentesque elit ullamcorper dignissim cras. Commodo elit at imperdiet dui accumsan. Aliquam nulla facilisi cras fermentum odio eu. In egestas erat imperdiet sed euismod nisi. Cursus in hac habitasse platea dictumst quisque sagittis purus sit.";
	size_t biglen = strlen(bigmsg);
	assert(wprint.print(bigmsg) == biglen);

	cout << ssout.str() << endl;

	return 0;
}
#endif

#if 1
#include "new_time.h"

int main(int argc, char* argv[])
{
	using namespace std;

	unsigned long start = millis();
	delay(100);
	unsigned long end = millis();
	cout << "start ms " << start << endl;
	cout << "end ms " << end << endl;
	cout << "difference " << end - start << endl;
}
#endif
