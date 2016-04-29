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

#include "pix_driver.h"
#include <ctime>
#include <cmath>



void pix_driver::run()
{
   LOG_DEBUG("run() of pix_driver");
   this->set_run(true);

   while(this->get_run())
   {
      this->update_fbuffer();
      this->refresh_matrx();
   }

   // Clear all pixels before exiting
   curr_fbuf_->clear();
   this->refresh_matrx();

   // Set all control bits to low
   ifc_.clr_all();

   // Disable display
   ifc_.set_oe(1);

   LOG_DEBUG("exit from run() of pix_driver");
}


void pix_driver::set_run(bool r)
{
   run_.store(r);
}


bool pix_driver::get_run()
{
   return run_.load();
}


// Sleep or busy wait
void pix_driver::sleep_hold_row(int ns)
{
   if(ns > 30000)
   {
      struct timespec wait = {0, static_cast<long>(ns - 20000)};
      nanosleep(&wait, NULL);
   }
   else
   {
      for(int i = 0; i < ns/2; i++)
      { asm(""); }
  }
}


// Update the display using the active frame buffer
void pix_driver::refresh_matrx()
{
   uint8_t rgb1 = 0, rgb2 = 0, abcd;
   uint8_t rmsk = 0x04, gmsk = 0x02, bmsk = 0x01;
   uint8_t nrmsk = 0x03, ngmsk = 0x05, nbmsk = 0x06;

   // For every double row (each address = two rows)
   for(abcd = 0; abcd < rows_; abcd++)
   {
      ifc_.set_row(abcd);
      pixel* row1 = curr_fbuf_ + abcd * length_;
      pixel* row2 = curr_fbuf_ + (abcd + rows_) * length_;

      // For every bit plane of color (color depth)
      for(int d = 0; d < depth_; d++)
      {
         uint8_t curr_depth = (1 << d);
         ifc_.set_lat(0);
         
         // For every horizontal pixel on the selected rows
         for(uint32_t l = 0; l < length_; l++)
         {
            (row1 + l)->r & curr_depth ? (rgb1 |= rmsk) : (rgb1 &= nrmsk);
            (row1 + l)->g & curr_depth ? (rgb1 |= gmsk) : (rgb1 &= ngmsk);
            (row1 + l)->b & curr_depth ? (rgb1 |= bmsk) : (rgb1 &= nbmsk);
            
            (row2 + l)->r & curr_depth ? (rgb2 |= rmsk) : (rgb2 &= nrmsk);
            (row2 + l)->g & curr_depth ? (rgb2 |= gmsk) : (rgb2 &= ngmsk);
            (row2 + l)->b & curr_depth ? (rgb2 |= bmsk) : (rgb2 &= nbmsk);

            // Clock in two rows
            ifc_.set_clk(0);
            ifc_.set_rgb1(rgb1);
            ifc_.set_rgb2(rgb2);
            ifc_.set_clk(1);
         }

         // Turn OFF
         ifc_.set_oe(1);
         
         // Dummy - remove?
         ifc_.set_row(abcd);
         
         // Latch in clocked rows
         ifc_.set_lat(1);
         
         // Turn ON
         ifc_.set_oe(0);
         ifc_.set_lat(0);

         // Wait! How long? - Depends on the weight of bitplane
         // MSB - longest, LSB - shortest (weights 2^d)
         this->sleep_hold_row(pow(2, d) * row_wait_ - row_wait_);
      }
   }
}


void pix_driver::update_fbuffer()
{
   curr_fbuf_ = fbuf_->get_active_fbuffer();
}


void pix_driver::stop()
{
   LOG_DEBUG("stop() of pix_driver");
   this->set_run(false);
}

