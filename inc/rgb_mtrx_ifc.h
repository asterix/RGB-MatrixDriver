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


enum masks
{
   MR1 = 0x04,
   MG1 = 0x02,
   MB1 = 0x01,
   MR2 = 0x20,
   MG2 = 0x10,
   MB2 = 0x08,
   MCL = 0x40
};


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
   uint32_t bits_;
   rasp_pi_gpio io_;
   rasp_pi_pwm pwm_;
   
   uint32_t full_mask_;

   bool startup_gpio();
   void update_gpio(uint32_t mask);

public:
   rgb_mtrx_ifc(): bits_(0), io_(), pwm_(), full_mask_(0)
   {
      LOG_DEBUG("Constructor of rgb_mtrx_ifc");

      // Create all used GPIO mask
      full_mask_ = ((1 << R1) | (1 << B1) | (1 << G1) |
                    (1 << R2) | (1 << B2) | (1 << G2) |
                    (1 << A) | (1 << B) | (1 << C) | (1 << D) |
                    (1 << CLK) | (1 << LAT) | (1 << OE));

      this->startup_gpio();
    }

   ~rgb_mtrx_ifc()
   {
      LOG_DEBUG("Destructor of rgb_mtrx_ifc");
   }

   bool startup_pwm(uint32_t t);

   inline void set_all()
   {
      bits_ = 0xFFFFFFFF;
      this->update_gpio(full_mask_);
   }

   inline void clr_all()
   {
      bits_ = 0;
      this->update_gpio(full_mask_);
   }

   inline void set_rgb1(uint8_t rgb)
   {
      // -----RGB (least 3 bits)
      (rgb & 0x04) ? (bits_ |= (1 << R1)) : (bits_ &= ~(1 << R1));
      (rgb & 0x02) ? (bits_ |= (1 << G1)) : (bits_ &= ~(1 << G1));
      (rgb & 0x01) ? (bits_ |= (1 << B1)) : (bits_ &= ~(1 << B1));
      this->update_gpio((1 << R1)|(1 << G1)|(1 << B1));
   }

   inline void set_rgb2(uint8_t rgb)
   {
      // -----RGB (least 3 bits)
      (rgb & 0x04) ? (bits_ |= (1 << R2)) : (bits_ &= ~(1 << R2));
      (rgb & 0x02) ? (bits_ |= (1 << G2)) : (bits_ &= ~(1 << G2));
      (rgb & 0x01) ? (bits_ |= (1 << B2)) : (bits_ &= ~(1 << B2));
      this->update_gpio((1 << R2)|(1 << G2)|(1 << B2));
   }

   inline void set_rgb12(uint8_t rgbc)
   {
      // -CRGBRGB
      (rgbc & MCL) ? (bits_ |= (1 << CLK)) : (bits_ &= ~(1 << CLK));
      (rgbc & MR1) ? (bits_ |= (1 << R1)) : (bits_ &= ~(1 << R1));
      (rgbc & MG1) ? (bits_ |= (1 << G1)) : (bits_ &= ~(1 << G1));
      (rgbc & MB1) ? (bits_ |= (1 << B1)) : (bits_ &= ~(1 << B1));
      (rgbc & MR2) ? (bits_ |= (1 << R2)) : (bits_ &= ~(1 << R2));
      (rgbc & MG2) ? (bits_ |= (1 << G2)) : (bits_ &= ~(1 << G2));
      (rgbc & MB2) ? (bits_ |= (1 << B2)) : (bits_ &= ~(1 << B2));
      this->update_gpio((1 << CLK)|(1 << R1)|(1 << G1)|(1 << B1)|(1 << R2)|(1 << G2)|(1 << B2));
   }

   inline void set_row(uint8_t dcba)
   {
      // ----DCBA (least 4 bits)
      (dcba & 0x01) ? (bits_ |= (1 << A)) : (bits_ &= ~(1 << A));
      (dcba & 0x02) ? (bits_ |= (1 << B)) : (bits_ &= ~(1 << B));
      (dcba & 0x04) ? (bits_ |= (1 << C)) : (bits_ &= ~(1 << C));
      (dcba & 0x08) ? (bits_ |= (1 << D)) : (bits_ &= ~(1 << D));
      this->update_gpio((1 << A)|(1 << B)|(1 << C)|(1 << D));
   }

   inline void set_clk(uint8_t clk)
   {
      // CLK = 0/1
      clk ? (bits_ |= (1 << CLK)) : (bits_ &= ~(1 << CLK));
      this->update_gpio((1 << CLK));
   }

   inline void set_lat(uint8_t lat)
   {
      // LAT/STROBE = 0/1
      lat ? (bits_ |= (1 << LAT)) : (bits_ &= ~(1 << LAT));
      this->update_gpio((1 << LAT));
   }

   inline void set_oe(uint8_t oe)
   {
      // OE = 0/1
      oe ? (bits_ |= (1 << OE)) : (bits_ &= ~(1 << OE));
      this->update_gpio((1 << OE));
   }

   inline uint32_t get_bits()
   {
      return bits_;
   }

   inline void set_gpio(uint16_t p, bool v)
   {
      if(v)
      {
         io_.set_pin(p);
      }
      else
      {
         io_.clr_pin(p);
      }
   }

   inline void pwm_pulse(uint32_t t)
   {
      // On GPIO 18
      pwm_.pwm_pulse(t);
   }

   inline void pwm_pulse_wait_finish()
   {
      pwm_.pwm_wait_fifo_empty();
   }
};


#endif // _RGB_MTRX_IFC_H_

