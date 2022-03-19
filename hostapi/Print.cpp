/*
  Copyright (c) 2014 Arduino.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "Print.h"
#include "StringHelpers.h"

using namespace arduino;

// Public Methods //////////////////////////////////////////////////////////////

/* default implementation: may be overridden */
size_t Print::write(const uint8_t *buffer, size_t size)
{
  size_t n = 0;
  while (size--) {
    if (write(*buffer++)) n++;
    else break;
  }
  return n;
}


size_t Print::print(const __FlashStringHelper *ifsh)
{
  return print(reinterpret_cast<const char *>(ifsh));
}

size_t Print::print(const String &s)
{
  return write(s.c_str(), s.length());
}

size_t Print::print(const std::string& str)
{
    return write_from(str.begin(), str.end());
}

size_t Print::print(const char str[])
{
  return write(str);
}

size_t Print::print(char c)
{
  return write(c);
}

size_t Print::print(unsigned char n, int base)
{
    if (base == 0) return write(n);
    else return write(to_string(n, base));
}

size_t Print::print(short n, int base)
{
    return write(to_string(n, base));
}

size_t Print::print(unsigned short n, int base)
{
    return write(to_string(n, base));
}

size_t Print::print(int n, int base)
{
    return write(to_string(n, base));
}

size_t Print::print(unsigned int n, int base)
{
    return write(to_string(n, base));
}

size_t Print::print(long n, int base)
{
    return write(to_string(n, base));
}

size_t Print::print(unsigned long n, int base)
{
    return write(to_string(n, base));
}

size_t Print::print(long long n, int base)
{
    return write(to_string(n, base));
}

size_t Print::print(unsigned long long n, int base)
{
    return write(to_string(n, base));
}

size_t Print::print(float n, int digits)
{
    return write(to_string(n, digits));
}

size_t Print::print(double n, int digits)
{
    return write(to_string(n, digits));
}

size_t Print::println(const __FlashStringHelper *ifsh)
{
  size_t n = print(ifsh);
  n += println();
  return n;
}

size_t Print::print(const Printable& x)
{
  return x.printTo(*this);
}

size_t Print::println(void)
{
  return write("\r\n");
}

size_t Print::println(const String &s)
{
  size_t n = print(s);
  n += println();
  return n;
}

size_t Print::println(const std::string& str)
{
    size_t n = print(str);
    n += println();
    return n;
}

size_t Print::println(const char c[])
{
  size_t n = print(c);
  n += println();
  return n;
}

size_t Print::println(char c)
{
  size_t n = print(c);
  n += println();
  return n;
}

size_t Print::println(unsigned char b, int base)
{
  size_t n = print(b, base);
  n += println();
  return n;
}

size_t Print::println(short num, int base)
{
    size_t n = print(num, base);
    n += println();
    return n;
}

size_t Print::println(unsigned short num, int base)
{
    size_t n = print(num, base);
    n += println();
    return n;
}

size_t Print::println(int num, int base)
{
  size_t n = print(num, base);
  n += println();
  return n;
}

size_t Print::println(unsigned int num, int base)
{
  size_t n = print(num, base);
  n += println();
  return n;
}

size_t Print::println(long num, int base)
{
  size_t n = print(num, base);
  n += println();
  return n;
}

size_t Print::println(unsigned long num, int base)
{
  size_t n = print(num, base);
  n += println();
  return n;
}

size_t Print::println(long long num, int base)
{
  size_t n = print(num, base);
  n += println();
  return n;
}

size_t Print::println(unsigned long long num, int base)
{
  size_t n = print(num, base);
  n += println();
  return n;
}

size_t Print::println(float num, int digits)
{
    size_t n = print(num, digits);
    n += println();
    return n;
}

size_t Print::println(double num, int digits)
{
  size_t n = print(num, digits);
  n += println();
  return n;
}

size_t Print::println(const Printable& x)
{
  size_t n = print(x);
  n += println();
  return n;
}

