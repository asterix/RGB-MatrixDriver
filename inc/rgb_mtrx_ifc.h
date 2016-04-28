/*---------------------------------------------------------------------------
Copyright (c) 2016, Vaibhav Desai

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.
THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
-----------------------------------------------------------------------------
Function:  RGB Matrix interface to GPIO
Created:   24-Apr-2016
---------------------------------------------------------------------------*/

#ifndef _RGB_MTRX_IFC_H_
#define _RGB_MTRX_IFC_H_

#include "rasp_pi_gpio.h"


class rgb_mtrx_ifc
{
public:
   enum gpio_map
   {
      R1 = 11,
      G1 = 27,
      B1 = 7,
      R2 = 8,
      G2 = 9,
      B2 = 10,
      A = 22,
      B = 23,
      C = 24,
      D = 25,
      CLK = 17,
      LAT = 4,
      OE = 18     
   };
   
private:
   uint32_t bits;

public:
   rgb_mtrx_ifc(): bits(0)
   {

   }
   
   ~rgb_mtrx_ifc()
   {

   }

   inline void set_all()
   {
      bits = 0xFFFFFFFF;
   }

   inline void clr_all()
   {
      bits = 0;
   }

   inline void set_rgb1(uint8_t rgb)
   {
      // -----RGB (least 3 bits)
      (rgb & 0x04) ? (bits |= (1 << R1)) : (bits &= ~(1 << R1));
      (rgb & 0x02) ? (bits |= (1 << G1)) : (bits &= ~(1 << G1));
      (rgb & 0x01) ? (bits |= (1 << B1)) : (bits &= ~(1 << B1));
   }

   inline void set_rgb2(uint8_t rgb)
   {
      // -----RGB (least 3 bits)
      (rgb & 0x04) ? (bits |= (1 << R2)) : (bits &= ~(1 << R2));
      (rgb & 0x02) ? (bits |= (1 << G2)) : (bits &= ~(1 << G2));
      (rgb & 0x01) ? (bits |= (1 << B2)) : (bits &= ~(1 << B2));
   }

   inline void set_row(uint8_t abcd)
   {
      // ----ABCD (least 4 bits)
      (abcd & 0x08) ? (bits |= (1 << A)) : (bits &= ~(1 << A));
      (abcd & 0x04) ? (bits |= (1 << B)) : (bits &= ~(1 << B));
      (abcd & 0x02) ? (bits |= (1 << C)) : (bits &= ~(1 << C));
      (abcd & 0x01) ? (bits |= (1 << D)) : (bits &= ~(1 << D));
   }

   inline void set_clk(uint8_t clk)
   {
      // CLK = 0/1
      clk ? (bits |= (1 << CLK)) : (bits &= ~(1 << CLK));
   }

   inline void set_lat(uint8_t lat)
   {
      // LAT/STROBE = 0/1
      lat ? (bits |= (1 << LAT)) : (bits &= ~(1 << LAT));
   }

   inline void set_oe(uint8_t oe)
   {
      // OE = 0/1
      oe ? (bits |= (1 << OE)) : (bits &= ~(1 << OE));
   }

   inline uint32_t get_bits()
   {
      return bits;
   }
};


#endif // _RGB_MTRX_IFC_H_

