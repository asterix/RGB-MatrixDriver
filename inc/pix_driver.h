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
Function:  High prio RT thread, updates RGB Matrix from frame buffer
Created:   25-Apr-2016
---------------------------------------------------------------------------*/

#ifndef _PIX_DRIVER_H_
#define _PIX_DRIVER_H_


#include "frame_buffer.h"
#include "rt_thread.h"
#include "rgb_mtrx_ifc.h"
#include <cmath>

// Time weight for bitplane 0 of colors (nano-secs) (scaled by 2 internally)
// 124 ns -> Lesser consumes less power, increases frame rate, but runs dimmer
#define BIT0_COLOR_TIME 124
#define ROW_WAIT        1250


class pix_driver final: public rt_thread, public pixel
{
private:
   frame_buffer* fbuf_;
   pixel* curr_fbuf_;
   std::atomic<bool> run_;

   // Use GPIO + Peripheral interface
   rgb_mtrx_ifc ifc_;

   // Timing constants for each bitplane
   uint32_t bit_plane_wait_;
   std::vector<uint32_t> bit_plane_times_;

   // Frame params
   uint32_t height_, length_;
   uint8_t depth_;
   uint32_t rows_;

   void refresh_matrx();
   void update_fbuffer();
   void sleep_hold_row(int ns);

   // Run as thread helpers
   void set_run(bool r);
   bool get_run();
   void run();

public:

   pix_driver() = delete;

   pix_driver(frame_buffer* f)
      : rt_thread(HPRIO, (1 << CPU_CORE3)), fbuf_(f), run_(false),
        ifc_(), bit_plane_wait_(ROW_WAIT)
   {
      LOG_DEBUG("Constructor of pix_driver");
      fbuf_->get_frame_params(length_, height_, depth_);
      curr_fbuf_ = fbuf_->get_active_fbuffer();

      // rows_ represents the two rows used at once
      // 16x32 matrix => row 0 and 8 are operated at once
      // 32x32 matrix => row 0 and 16 are operated at once
      // 64x32 matrix => row 0 and 32 are operated at once
      rows_ = height_ / 2;

      // Pre-fill time weights for each bitplane
      for(uint32_t i = 0; i < depth_; i++)
      {
         // Weights = [2, 4, 8, 16, 32, 64, 128, 256]
         //         LSB^                        MSB^
         bit_plane_times_.push_back(pow(2, i+1));
      }

      // PWM startup
      ifc_.startup_pwm(BIT0_COLOR_TIME);
   }

   ~pix_driver()
   {
      LOG_DEBUG("Destructor of pix_driver");
   }

   void stop();

};

#endif // _PIX_DRIVER_H_

