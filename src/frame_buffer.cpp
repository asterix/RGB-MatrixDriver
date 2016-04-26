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

#include "frame_buffer.h"


void frame_buffer::init_fbuf()
{
   // Allocate frame buffers are linear pixel arrays
   for(int i = 0; i < FRAME_BUFS; i++)
   {
      fbuf_.push_back(new pixel[length_ * height_]);
   }
}


void frame_buffer::change_active_fbuffer()
{
   int num_bufs = fbuf_.size();
   active_buf_ = ++active_buf_ % num_bufs;
}


pixel* frame_buffer::get_active_fbuffer()
{
   return fbuf_.at(active_buf_);
}


uint8_t frame_buffer::next_idle_buf()
{
   int num_bufs = fbuf_.size();
   return ((active_buf_ + 1) % num_bufs);
}


// Gets a pointer to the next frame buffer (future use when changed())
pixel* frame_buffer::get_idle_fbuffer()
{
   return fbuf_.at(this->next_idle_buf());
}


void frame_buffer::get_frame_params(uint32_t& l, uint32_t& h, uint8_t& d)
{
   l = length_;
   h = height_;
   d = depth_;
}


// Gets a pointer to the pixel at (x,y)
pixel* frame_buffer::get_pixel(uint32_t x, uint32_t y)
{
   return (fbuf_.at(active_buf_) + y * length_ + x);
}


// Gets a pointer to the row head
pixel* frame_buffer::get_row(uint32_t row)
{
   return (fbuf_.at(active_buf_) + row * length_);
}


void frame_buffer::clear_frame(uint8_t fr)
{
   pixel *frame_ptr = fbuf_.at(fr);
   for(uint32_t i = 0; i < height_ * length_; i++)
   {
      frame_ptr->clear();
      frame_ptr++;
   }
}

