#pragma once

#include <Print.h>
#include <Printable.h>

class PrintableInt : public arduino::Printable {
public:
	PrintableInt(int i) : _i(i) {}
	PrintableInt() : _i(0) { }

    virtual size_t printTo(arduino::Print& p) const override
    {
        return p.print("PrintableInt i = ") + p.print(_i);
    }
protected:
    int _i;
};

class Print_basic : public Print {
public:
    virtual size_t write(uint8_t) override { return 1; }
    void mock_setWriteError(int err) {
        setWriteError(err);
    }
};
