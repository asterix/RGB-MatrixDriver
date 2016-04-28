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

#include "rgb_mtrx_ifc.h"


bool rgb_mtrx_ifc::startup_gpio()
{
   if(!io_.init())
   {
      LOG_ERROR("GPIO init failed :(");
      return false;
   }

   // Set used GPIO to output
   uint32_t mask = ((1 << R1) | (1 << B1) | (1 << G1) |
                    (1 << R2) | (1 << B2) | (1 << G2) |
                    (1 << A) | (1 << B) | (1 << C) | (1 << D) |
                    (1 << CLK) | (1 << LAT) | (1 << OE));
   for(int i = 0; i < 28; i++)
   {
      if(mask & (1 << i))
      {
         io_.set_mode_out(i);
      }
   }

   LOG_DEBUG("Successful setup of rgb_mtrx_ifc");
   return true;
}


void rgb_mtrx_ifc::update_gpio(uint32_t mask)
{
   // Write straight to GPSET0 register
   io_.set_gpset0(bits_ & mask);

   // Write inverted values to GPCLR0 register
   io_.set_gpclr0(~bits_ & mask);
}

