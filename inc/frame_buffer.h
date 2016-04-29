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
Function:  Manages the underlying pixels' frame (double buffer)
Created:   25-Apr-2016
---------------------------------------------------------------------------*/

#ifndef _FRAME_BUFFER_H_
#define _FRAME_BUFFER_H_

#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <memory>
#include <iterator>
#include "logging.h"

class pixel
{
public:
   uint8_t r;
   uint8_t g;
   uint8_t b;
   uint8_t a;

   pixel(): r(0), g(0),
            b(0), a(0)
   { }

   virtual ~pixel()
   { }

   void clear()
   {
      r = g = b = 0;
      a = 100;
   }
};


class frame_buffer : public pixel
{
   enum generic
   {
      COLOR_DEPTH = 8,
      MATRX_LENGTH = 32,
      MATRX_HEIGHT = 16,
      FRAME_BUFS = 2
   };

private:
   std::atomic<uint8_t> active_buf_;
   std::vector<pixel*> fbuf_;
   uint8_t depth_;
   uint32_t length_;
   uint32_t height_;
   std::atomic<bool> taken_;

   // Data guards
   std::mutex fbuf_mtx_;

   // Private methods
   void init_fbuf();
   uint8_t next_idle_buf();

public:
   frame_buffer()
      : active_buf_(0), depth_(COLOR_DEPTH),
        length_(MATRX_LENGTH), height_(MATRX_HEIGHT), taken_(false)
   {
      LOG_DEBUG("Constructor of frame_buffer");
      this->init_fbuf();
   }

   frame_buffer(uint32_t l, uint32_t h, uint8_t d = COLOR_DEPTH)
      : active_buf_(0), depth_(d), length_(l), height_(h), taken_(false)
   {
      LOG_DEBUG("Constructor of frame_buffer");
      this->init_fbuf();
   }

   ~frame_buffer()
   {
      LOG_DEBUG("Destructor of frame_buffer");
      // Release frame buffers
      for(auto it = fbuf_.begin(); it != fbuf_.end(); it++)
      {
         delete[] *it;
      }
   }
   
   // Frame buffer management
   void change_active_fbuffer();
   pixel* get_active_fbuffer();
   pixel* get_idle_fbuffer();
   void get_frame_params(uint32_t& l, uint32_t& h, uint8_t& d);
   void clear_frame(pixel *frame_ptr);

   // Frame accessors
   pixel* get_pixel(pixel *frame_ptr, uint32_t x, uint32_t y);
   pixel* get_row(pixel *frame_ptr, uint32_t row);

};


#endif // _FRAME_BUFFER_H_

