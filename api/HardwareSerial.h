/*
  Copyright (c) 2016 Arduino LLC.  All right reserved.

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

  Modified 28 September 2010 by Mark Sproul
  Modified 14 August 2012 by Alarus
  Modified 3 December 2013 by Matthijs Kooijman
*/

#pragma once

#include <inttypes.h>
#include "Stream.h"

namespace arduino {

// XXX: Those constants should be defined as const int / enums?
// XXX: shall we use namespaces too?
#define SERIAL_PARITY_EVEN   (0x1ul)
#define SERIAL_PARITY_ODD    (0x2ul)
#define SERIAL_PARITY_NONE   (0x3ul)
#define SERIAL_PARITY_MARK   (0x4ul)
#define SERIAL_PARITY_SPACE  (0x5ul)
#define SERIAL_PARITY_MASK   (0xFul)

#define SERIAL_STOP_BIT_1    (0x10ul)
#define SERIAL_STOP_BIT_1_5  (0x20ul)
#define SERIAL_STOP_BIT_2    (0x30ul)
#define SERIAL_STOP_BIT_MASK (0xF0ul)

#define SERIAL_DATA_5        (0x100ul)
#define SERIAL_DATA_6        (0x200ul)
#define SERIAL_DATA_7        (0x300ul)
#define SERIAL_DATA_8        (0x400ul)
#define SERIAL_DATA_MASK     (0xF00ul)

#define SERIAL_5N1           (SERIAL_STOP_BIT_1 | SERIAL_PARITY_NONE  | SERIAL_DATA_5)
#define SERIAL_6N1           (SERIAL_STOP_BIT_1 | SERIAL_PARITY_NONE  | SERIAL_DATA_6)
#define SERIAL_7N1           (SERIAL_STOP_BIT_1 | SERIAL_PARITY_NONE  | SERIAL_DATA_7)
#define SERIAL_8N1           (SERIAL_STOP_BIT_1 | SERIAL_PARITY_NONE  | SERIAL_DATA_8)
#define SERIAL_5N2           (SERIAL_STOP_BIT_2 | SERIAL_PARITY_NONE  | SERIAL_DATA_5)
#define SERIAL_6N2           (SERIAL_STOP_BIT_2 | SERIAL_PARITY_NONE  | SERIAL_DATA_6)
#define SERIAL_7N2           (SERIAL_STOP_BIT_2 | SERIAL_PARITY_NONE  | SERIAL_DATA_7)
#define SERIAL_8N2           (SERIAL_STOP_BIT_2 | SERIAL_PARITY_NONE  | SERIAL_DATA_8)
#define SERIAL_5E1           (SERIAL_STOP_BIT_1 | SERIAL_PARITY_EVEN  | SERIAL_DATA_5)
#define SERIAL_6E1           (SERIAL_STOP_BIT_1 | SERIAL_PARITY_EVEN  | SERIAL_DATA_6)
#define SERIAL_7E1           (SERIAL_STOP_BIT_1 | SERIAL_PARITY_EVEN  | SERIAL_DATA_7)
#define SERIAL_8E1           (SERIAL_STOP_BIT_1 | SERIAL_PARITY_EVEN  | SERIAL_DATA_8)
#define SERIAL_5E2           (SERIAL_STOP_BIT_2 | SERIAL_PARITY_EVEN  | SERIAL_DATA_5)
#define SERIAL_6E2           (SERIAL_STOP_BIT_2 | SERIAL_PARITY_EVEN  | SERIAL_DATA_6)
#define SERIAL_7E2           (SERIAL_STOP_BIT_2 | SERIAL_PARITY_EVEN  | SERIAL_DATA_7)
#define SERIAL_8E2           (SERIAL_STOP_BIT_2 | SERIAL_PARITY_EVEN  | SERIAL_DATA_8)
#define SERIAL_5O1           (SERIAL_STOP_BIT_1 | SERIAL_PARITY_ODD   | SERIAL_DATA_5)
#define SERIAL_6O1           (SERIAL_STOP_BIT_1 | SERIAL_PARITY_ODD   | SERIAL_DATA_6)
#define SERIAL_7O1           (SERIAL_STOP_BIT_1 | SERIAL_PARITY_ODD   | SERIAL_DATA_7)
#define SERIAL_8O1           (SERIAL_STOP_BIT_1 | SERIAL_PARITY_ODD   | SERIAL_DATA_8)
#define SERIAL_5O2           (SERIAL_STOP_BIT_2 | SERIAL_PARITY_ODD   | SERIAL_DATA_5)
#define SERIAL_6O2           (SERIAL_STOP_BIT_2 | SERIAL_PARITY_ODD   | SERIAL_DATA_6)
#define SERIAL_7O2           (SERIAL_STOP_BIT_2 | SERIAL_PARITY_ODD   | SERIAL_DATA_7)
#define SERIAL_8O2           (SERIAL_STOP_BIT_2 | SERIAL_PARITY_ODD   | SERIAL_DATA_8)
#define SERIAL_5M1           (SERIAL_STOP_BIT_1 | SERIAL_PARITY_MARK  | SERIAL_DATA_5)
#define SERIAL_6M1           (SERIAL_STOP_BIT_1 | SERIAL_PARITY_MARK  | SERIAL_DATA_6)
#define SERIAL_7M1           (SERIAL_STOP_BIT_1 | SERIAL_PARITY_MARK  | SERIAL_DATA_7)
#define SERIAL_8M1           (SERIAL_STOP_BIT_1 | SERIAL_PARITY_MARK  | SERIAL_DATA_8)
#define SERIAL_5M2           (SERIAL_STOP_BIT_2 | SERIAL_PARITY_MARK  | SERIAL_DATA_5)
#define SERIAL_6M2           (SERIAL_STOP_BIT_2 | SERIAL_PARITY_MARK  | SERIAL_DATA_6)
#define SERIAL_7M2           (SERIAL_STOP_BIT_2 | SERIAL_PARITY_MARK  | SERIAL_DATA_7)
#define SERIAL_8M2           (SERIAL_STOP_BIT_2 | SERIAL_PARITY_MARK  | SERIAL_DATA_8)
#define SERIAL_5S1           (SERIAL_STOP_BIT_1 | SERIAL_PARITY_SPACE | SERIAL_DATA_5)
#define SERIAL_6S1           (SERIAL_STOP_BIT_1 | SERIAL_PARITY_SPACE | SERIAL_DATA_6)
#define SERIAL_7S1           (SERIAL_STOP_BIT_1 | SERIAL_PARITY_SPACE | SERIAL_DATA_7)
#define SERIAL_8S1           (SERIAL_STOP_BIT_1 | SERIAL_PARITY_SPACE | SERIAL_DATA_8)
#define SERIAL_5S2           (SERIAL_STOP_BIT_2 | SERIAL_PARITY_SPACE | SERIAL_DATA_5)
#define SERIAL_6S2           (SERIAL_STOP_BIT_2 | SERIAL_PARITY_SPACE | SERIAL_DATA_6)
#define SERIAL_7S2           (SERIAL_STOP_BIT_2 | SERIAL_PARITY_SPACE | SERIAL_DATA_7)
#define SERIAL_8S2           (SERIAL_STOP_BIT_2 | SERIAL_PARITY_SPACE | SERIAL_DATA_8)

class HardwareSerial : public Stream
{
  protected:
    volatile uint8_t * const _ubrrh;
    volatile uint8_t * const _ubrrl;
    volatile uint8_t * const _ucsra;
    volatile uint8_t * const _ucsrb;
    volatile uint8_t * const _ucsrc;
    volatile uint8_t * const _udr;
    // Has any byte been written to the UART since begin()
    bool _written;

    volatile rx_buffer_index_t _rx_buffer_head;
    volatile rx_buffer_index_t _rx_buffer_tail;
    volatile tx_buffer_index_t _tx_buffer_head;
    volatile tx_buffer_index_t _tx_buffer_tail;

    // Don't put any members after these buffers, since only the first
    // 32 bytes of this struct can be accessed quickly using the ldd
    // instruction.
    unsigned char _rx_buffer[SERIAL_RX_BUFFER_SIZE];
    unsigned char _tx_buffer[SERIAL_TX_BUFFER_SIZE];

  public:
    inline HardwareSerial(
      volatile uint8_t *ubrrh, volatile uint8_t *ubrrl,
      volatile uint8_t *ucsra, volatile uint8_t *ucsrb,
      volatile uint8_t *ucsrc, volatile uint8_t *udr);
    void begin(unsigned long baud) { begin(baud, SERIAL_8N1); }
    void begin(unsigned long, uint8_t);
    void end();
    virtual int available(void);
    virtual int peek(void);
    virtual int read(void);
    virtual int availableForWrite(void);
    virtual void flush(void);
    virtual size_t write(uint8_t);
    inline size_t write(unsigned long n) { return write((uint8_t)n); }
    inline size_t write(long n) { return write((uint8_t)n); }
    inline size_t write(unsigned int n) { return write((uint8_t)n); }
    inline size_t write(int n) { return write((uint8_t)n); }
    using Print::write; // pull in write(str) and write(buf, size) from Print
    operator bool() { return true; }

    // Interrupt handlers - Not intended to be called externally
    inline void _rx_complete_irq(void);
    void _tx_udr_empty_irq(void);
};

#if defined(UBRRH) || defined(UBRR0H)
  extern HardwareSerial Serial;
  #define HAVE_HWSERIAL0
#endif
#if defined(UBRR1H)
  extern HardwareSerial Serial1;
  #define HAVE_HWSERIAL1
#endif
#if defined(UBRR2H)
  extern HardwareSerial Serial2;
  #define HAVE_HWSERIAL2
#endif
#if defined(UBRR3H)
  extern HardwareSerial Serial3;
  #define HAVE_HWSERIAL3
#endif

extern void serialEventRun(void) __attribute__((weak));

}