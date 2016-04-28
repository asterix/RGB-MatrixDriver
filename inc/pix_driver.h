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


class pix_driver : public rt_thread, public pixel
{
private:
   frame_buffer* fbuf_;
   pixel* curr_fbuf_;
   std::atomic<bool> run_;

   void refresh_matrx();
   void update_fbuffer();
   void set_run(bool r);
   bool get_run();

public:

   pix_driver() = delete;

   pix_driver(frame_buffer* f)
      : rt_thread(HPRIO), fbuf_(f), run_(false)
   {
      curr_fbuf_ = fbuf_->get_active_fbuffer();
   }

   ~pix_driver()
   {

   }

   void run();
   void stop();

};

#endif // _PIX_DRIVER_H_

