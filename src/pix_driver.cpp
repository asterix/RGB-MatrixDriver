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



void pix_driver::run()
{
   LOG_DEBUG("run() of pix_driver");

   this->set_run(true);

   while(this->get_run())
   {
      this->update_fbuffer();
      this->refresh_matrx();
   }
}


void pix_driver::set_run(bool r)
{
   run_.store(r);
}


bool pix_driver::get_run()
{
   return run_.load();
}


// Update the display using the active frame buffer
void pix_driver::refresh_matrx()
{

   ifc_.set_clk(1);
   sleep(1);
   ifc_.set_clk(0);
   sleep(1);

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

