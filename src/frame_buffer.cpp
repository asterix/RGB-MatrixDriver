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
   uint8_t num_bufs = static_cast<uint8_t>(fbuf_.size());
   register uint8_t temp_active = active_buf_.load() + 1;

   temp_active = temp_active % num_bufs;
   active_buf_.store(temp_active);

   taken_.store(false);

   // Wait until the new active fbuffer is read by driver
   while(!taken_.load());
}


pixel* frame_buffer::get_active_fbuffer()
{
   pixel *res = fbuf_.at(active_buf_.load());

   // Say taken to unblock frame updater
   taken_.store(true);
   return res;
}


uint8_t frame_buffer::next_idle_buf()
{
   int num_bufs = fbuf_.size();
   return ((active_buf_.load() + 1) % num_bufs);
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
pixel* frame_buffer::get_pixel(pixel *fbuf, uint32_t x, uint32_t y)
{
   return (fbuf + y * length_ + x);
}


// Gets a pointer to the row head
pixel* frame_buffer::get_row(pixel *fbuf, uint32_t row)
{
   return (fbuf + row * length_);
}


void frame_buffer::clear_frame(pixel *frame_ptr)
{
   for(uint32_t i = 0; i < height_ * length_; i++)
   {
      frame_ptr->clear();
      frame_ptr++;
   }
}

