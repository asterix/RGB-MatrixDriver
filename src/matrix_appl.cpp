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
Function:  RGB LED Matrix main application
Created:   23-Apr-2016
---------------------------------------------------------------------------*/

#include "matrix_appl.h"
#include "rgb_matrix.h"
#include "what.h"



int main(int argc, char *arg[])
{
   rgb_matrix mtrx;
   mtrx.run();

   return 0;
}


// Your coloring algorithm goes here
bool what::playground()
{
   static bool done = false;
   uint8_t r, g, b;
   // Use frame buffer to modify
   for(uint32_t j = 0; j < height_; j++)
   {
      r = g = b = 0;

      if(j % 3 == 0) r = 255;
      else if(j % 3 == 1) g = 255;
      else if(j % 3 == 2) b = 255;

      for(uint32_t i = 0; i < length_; i++)
      {
         new_fbuf_->r = r;
         new_fbuf_->g = g;
         new_fbuf_->b = b;
         new_fbuf_++;
      }
   }


   // Stop running
   if(!done)
   {
      done = true;
      return true;
   }
   else
   {
      sleep(5);
      stop();
      return false;
   }
}

