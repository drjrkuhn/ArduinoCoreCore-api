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

#include <ostream>
#include <iostream>

void dummy() {
    int i = 12;
    std::cout << i;
}

using namespace arduino;

// Public Methods //////////////////////////////////////////////////////////////

// Private Methods /////////////////////////////////////////////////////////////

// REFERENCE IMPLEMENTATION FOR ULL
// size_t Print::printULLNumber(unsigned long long n, uint8_t base)
// {
  // // if limited to base 10 and 16 the bufsize can be smaller
  // char buf[65];
  // char *str = &buf[64];

  // *str = '\0';

  // // prevent crash if called with base == 1
  // if (base < 2) base = 10;

    // do {
      // unsigned long long t = n / base;
      // char c = n - t * base;  // faster than c = n%base;
      // n = t;
      // *--str = c < 10 ? c + '0' : c + 'A' - 10;
  // } while(n);

  // return write(str);
// }

