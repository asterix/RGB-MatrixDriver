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
Function:  RGB Matrix abstraction
Created:   24-Apr-2016
---------------------------------------------------------------------------*/

#ifndef _RGB_MATRIX_H_
#define _RGB_MATRIX_H_


#include <thread>
#include <memory>
#include "rasp_pi_gpio.h"
#include "frame_buffer.h"



class rgb_matrix
{
private:
   uint16_t length_;
   uint16_t height_;
   uint8_t depth_;
   uint16_t num_in_chain_;

   frame_buffer *frame_buf_;

   void startup();

public:
   rgb_matrix()
      : length_(32), height_(16), depth_(8),
        num_in_chain_(1), frame_buf_(nullptr)
   {
      this->startup();
   }

   rgb_matrix(uint16_t l, uint16_t h, uint8_t d, uint16_t n)
      : length_(l), height_(h), depth_(d),
        num_in_chain_(n), frame_buf_(nullptr)
   {
      this->startup();
   }
   
   ~rgb_matrix()
   {
      delete frame_buf_;
   }

   void run();

};


#endif // _RGB_MATRIX_H_

