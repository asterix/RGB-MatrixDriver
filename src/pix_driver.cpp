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
   uint8_t rgbc = 0, abcd;

   // Start with LAT = 0
   ifc_.set_lat(0);

   // For every double row (each address = two rows - twin row)
   for(abcd = 0; abcd < rows_; abcd++)
   {
      // Go to next twin row
      ifc_.set_row(abcd);

      pixel *row1 = curr_fbuf_ + abcd * length_, *r1;
      pixel *row2 = curr_fbuf_ + (abcd + rows_) * length_, *r2;

      // For every bit plane of color (color depth)
      for(int d = 0; d < depth_; d++)
      {
         uint8_t curr_depth = (1 << d);

         r1 = row1;
         r2 = row2;

         // For every horizontal pixel on the selected rows
         for(uint32_t l = 0; l < length_; l++)
         {
            if(r1->r & curr_depth) (rgbc |= MR1);
            if(r1->g & curr_depth) (rgbc |= MG1);
            if(r1->b & curr_depth) (rgbc |= MB1);

            if(r2->r & curr_depth) (rgbc |= MR2);
            if(r2->g & curr_depth) (rgbc |= MG2);
            if(r2->b & curr_depth) (rgbc |= MB2);

            // Clock in two rows
            ifc_.set_rgb12(rgbc); // CLK = 0

            rgbc = 0;
            r1++; r2++;

            ifc_.set_clk(1);      // CLK = 1
         }

         // Wait until the previous bitplane is complete
         ifc_.pwm_pulse_wait_finish();

         // Latch in clocked rows
         ifc_.set_lat(1);
         ifc_.set_lat(0);

         // Turn ON for bitplane weighted time
         ifc_.pwm_pulse(bit_plane_times_.at(d));
      }

      // Wait until the previous bitplane is complete
      ifc_.pwm_pulse_wait_finish();
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

