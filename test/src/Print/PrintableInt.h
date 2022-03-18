#pragma once

#include <Printable.h>

class PrintableInt : public Printable {
public:
	int _i;
	PrintableInt(int i) : _i(i) {}
	PrintableInt() : _i(0) { }

    virtual size_t printTo(arduino::Print& p) const override
    {
        return p.print("PrintableInt i = ") + p.print(_i);
    }
};
