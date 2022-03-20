#include <ostream>
#include "Printable.h"
#include "Print.h"
#include "Print_std.h"

std::ostream& operator<<(std::ostream& os, arduino::Printable const& that) {
    arduino::Print_stdstring buf;
    that.printTo(buf);
    os << buf.str();
    return os;
}

