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
Function:  What? => what is shown on the display
Created:   25-Apr-2016
---------------------------------------------------------------------------*/

#ifndef _WHAT_H_
#define _WHAT_H_


#include "frame_buffer.h"
#include "rt_thread.h"


class what final: public rt_thread, public pixel
{
private:
   frame_buffer* fbuf_;
   pixel* new_fbuf_;
   volatile std::atomic<bool> run_;

   // Frame params
   uint32_t height_, length_;
   uint8_t depth_;

public:

   what() = delete;

   what(frame_buffer* f)
      : rt_thread(), fbuf_(f), run_(false)
   {
      LOG_DEBUG("Constructor of what");
      new_fbuf_ = fbuf_->get_idle_fbuffer();
      fbuf_->get_frame_params(length_, height_, depth_);
   }

   ~what()
   {
      LOG_DEBUG("Destructor of what");
   }

   void run();
   virtual bool playground();
   void stop();
   void get_frame_params(uint32_t& l, uint32_t& h, uint8_t& d);
   void set_pixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 100);

};



#endif // _WHAT_H_

