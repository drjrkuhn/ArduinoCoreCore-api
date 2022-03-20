#include <iostream>
#include <string>
#include <sstream>
#include <cassert>
#include <algorithm>
#include <type_traits>

#if !defined(__PRETTY_FUNCTION__) && !defined(__GNUC__)
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

#if TEMPLATE
int main(int argc, char* argv[])
{
	return 0;
}
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

#if 0
#include "new_time.h"

int main(int argc, char* argv[])
{
	using namespace std;

	unsigned long start = systime::millis();
	systime::delay(50);
	unsigned long end = systime::millis();
	cout << "start ms " << start << endl;
	cout << "end ms " << end << endl;
	cout << "difference " << end - start << endl;

	start = systime::micros();
	systime::delayMicroseconds(50000);
	end = systime::micros();
	cout << "start us " << start << endl;
	cout << "end us " << end << endl;
	cout << "difference " << end - start << endl;

	const int N = 100;
	const unsigned long delayms = 50;
	start = systime::millis();
	for (int i = 0; i < N; i++) {
		systime::delay(delayms);
	}
	end = systime::millis();
	double avg = (end - start) / double(N);
	cout << N << " delays of " << delayms << " ms = " << (N * delayms) << " total ms" << endl;
	cout << "actual " << (end - start) << " ms total" << endl;
	cout << "average of " << avg << " ms per delay" << endl;

}
#endif

#if 0

#include <string>

using namespace std;

class mystr {

public:
	using iterator = string::iterator;
	using const_iterator = string::const_iterator;
	using reverse_iterator = string::reverse_iterator;
	using const_reverse_iterator = string::const_reverse_iterator;

	mystr() : buffer_() {}
	mystr(const char* c_str) : buffer_(c_str ? c_str : "") {	}
	mystr(const string& other) : buffer_(other) {}
	mystr(string&& other) : buffer_(std::move(other)) {}

	template<typename T>
	friend bool operator==(const mystr& lhs, T rhs) { return lhs.buffer_ == rhs; }
	template<typename T>
	friend bool operator==(T lhs, const mystr& rhs) { return lhs == rhs.buffer_; }

	string& as_str() { return buffer_; }
	explicit operator string() const { return buffer_; }

protected:
	string buffer_;
};

int main(int argc, char* argv[])
{
	mystr hello("hello");
	mystr failstr(NULL);

	cout << string(hello) << endl;
	cout << (hello == "hello") << endl;
	cout << ("hello" == hello) << endl;
	cout << "NULL initialized: " << string(failstr) << endl;

	cout << "== comparison tests ==" << endl;
	string empty1 = "", empty2 = "", aaa = "aaa", bbb = "bbb";
	cout << "aaa.compare bbb: " << aaa.compare(bbb) << endl;
	cout << "bbb.compare aaa: " << bbb.compare(aaa) << endl;
	cout << "empty1.compare empty2: " << empty1.compare(empty2) << endl;
	cout << "aaa.compare empty2: " << aaa.compare(empty2) << endl;
	cout << "empty1.compare aaa: " << empty1.compare(aaa) << endl;
	cout << "aaa.compare "": " << aaa.compare("") << endl;

	cout << "== operator[] tests ==" << endl;
	cout << "get 1: " << aaa[1] << endl;
	cout << "get 10: " << aaa[2] << endl;

	cout << "== casting ==" << endl;

	long lmax = numeric_limits<long>::max();
	short smax = short(min(numeric_limits<short>::max(), lmax));
	cout << "long max: " << lmax << "  convrted to short max: " << smax << endl;

	return 0;
}

#endif

#if 0

template <class D>
struct basic_Base {

	const D& self() const { return static_cast<const D&>(*this); }
	D& self() { return static_cast<D&>(*this); }

	int geti() {
		return self().geti__();
	}
	int geti__() { return 0; }
};

struct Base : basic_Base<Base> {
};

struct Derived : basic_Base<Derived> {
	int geti__() { return 10; }
};

int main(int argc, char* argv[])
{
	using namespace std;

	//Base<void> base;
	Base simple;
	Derived derived;

	//cout << "base direct: " << base.geti__() << endl;
	cout << "simple direct: " << simple.geti__() << endl;
	cout << "derived direct: " << derived.geti__() << endl;

	//cout << "base indirect: " << base.geti() << endl;
	cout << "simple indirect: " << simple.geti() << endl;
	cout << "derived indirect: " << derived.geti() << endl;
	return 0;
}
#endif


#if 0

#include <vector>

using namespace std;

//// the partial specialization of A is enabled via a template parameter
//template<class T, class Enable = void>
//struct is_char_iterator : false_type {}; // primary template
//
//template<class T>
//struct is_char_iterator<T, typename std::enable_if<std::is_same<std::iterator_traits<T>::value_type, char>::value>::type> : true_type {
//}; // specialization for floating point types


template <typename T>
constexpr bool is_char_iterator_v = std::is_same<typename std::iterator_traits<T>::value_type, char>::value;

void print(const char* buf, size_t length) {
	cout.write(buf, length);
}

template <typename IT, typename std::enable_if<
	is_char_iterator_v<IT>, bool>::type = false>
	void print(IT begin, IT end) {
	//typename iterator_traits<IT>::pointer buf = &(*begin);
	//typename iterator_traits<IT>::difference_type size = end - begin;
	//print(buf, size);
	print(&begin[0], end - begin);
}

int main(int argc, char* argv[])
{
	char hello[] = "hello";

	print(hello, strlen(hello));
	cout << endl;

	string world("world");
	print(world.cbegin(), world.cend());
	cout << endl;

	vector<char> cvec{ 'a','b','c','d','e' };
	print(cvec.cbegin(), cvec.cend());
	cout << endl;

	//vector<long> lvec{ 10, 20, 30, 40, 50, 60 };
	//print(lvec.begin(), lvec.end());
	//cout << endl;

	return 0;
}
#endif

#if 1

#include <sstream>
#include <algorithm>

using namespace std;

std::istream_iterator<char> read_until(std::istream_iterator<char> begin, std::istream_iterator<char> end, std::string needle) {
	if (needle.size() == 0) return begin;
	std::string buff;
	buff.reserve(needle.size());
	std::istream_iterator<char> it = std::find_if(begin, end, [&](char c) {
		buff.push_back(c);
		if (buff.size() < needle.size()) {
			return false;
		}
		if (buff == needle)
			return true;
		buff.erase(buff.begin());
		return false;
		});
	return it;
}


std::istream_iterator<char> read_until(std::istream_iterator<char> begin, std::string needle) {
	if (needle.size() == 0) return begin;
	std::string buff;
	buff.reserve(needle.size());
	std::istream_iterator<char> eof;
	std::istream_iterator<char> it = std::find_if(begin, eof, [&](char c) {
		buff.push_back(c);
		if (buff.size() < needle.size()) {
			return false;
		}
		if (buff == needle)
			return true;
		buff.erase(buff.begin());
		return false;
		});
	return it;
}

int main(int argc, char* argv[])
{
	stringstream mock("a test string");
	noskipws(mock);
	string tofind("tes");
	istream_iterator<char> eof;
	istream_iterator<char> begin(mock);
	istream_iterator<char> pos = read_until(begin, tofind);
	//cout << "pos before " << mock.tellg();
	//for (int i=0; i<3; i++)	mock.get();
	//cout << " after " << mock.tellg() << endl;

	stringstream remain;
	remain << mock.rdbuf();
	mock.sync();
	cout << "found: " << bool(pos != eof) << endl;
	cout << "remain " << remain.str() << endl;
	cout << "mock.str " << mock.str() << endl;
	cout << "mock.substr " << mock.str().substr(mock.tellg()) << endl;
	cout << "mock.tellg: " << mock.tellg() << endl;
	cout << "mock.rdbuf: " << mock.rdbuf() << endl;
	cout << "mock.rdbuf->str " << mock.rdbuf()->str() << endl;
	//cout << "remaining: ";
	//std::copy(istream_iterator<char>(mock,, eof, ostream_iterator<char>(cout));
	//cout << endl;

	return 0;
}
#endif

