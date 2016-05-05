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
#include "ppm_parser.h"
#include "what.h"



int main(int argc, char *arg[])
{
   rgb_matrix mtrx;
   mtrx.run();

   // Wait for user input
   std::cout << "Press any key to exit" << std::endl;
   std::cin.get();

   mtrx.stop();
   return 0;
}


// Your coloring algorithm goes here
bool what::playground()
{
   //static uint32_t x = 3, y = 8;
   // uint32_t l = 2;

   usleep(300 * 1000);

   // Moving ball
   /*if(x + l >= length_)
   {
      x = 3;
   }

   if(x + l < length_ && y + l < height_)
   {
      for(uint32_t i = x - l; i <= x + l; i++)
      {
         for(uint32_t j = y - l; j <= y + l; j++)
         {
            if(!((i == x + l || i == x - l) && (j == y + l || j == y - l)))
            {
               set_pixel(i, j, 255, 0, 255);
            }
         }
      }
      x++;
   }*/

   // Random colors
   /*for(uint32_t x = 0; x < length_; x++)
   {
      for(uint32_t y = 0; y < height_; y++)
      {
         set_pixel(x, y, rand()%255, rand()%255, rand()%255);
      }
   }*/

   // Smiley face
   ppm_parser pr;
   color_buffer cbuf1, cbuf2;

   pr.parse("smiley16none.ppm", &cbuf1);
   pr.parse("frownyw.ppm", &cbuf2);

   for(uint32_t x = 0; x < length_; x++)
   {
      for(uint32_t y = 0; y < height_; y++)
      {
         if(x < cbuf1.length)
         {
            set_pixel(x, y, cbuf1.pix[x + cbuf1.length * y].r,
                            cbuf1.pix[x + cbuf1.length * y].g,
                            cbuf1.pix[x + cbuf1.length * y].b);
         }
         else
         {
            set_pixel(x, y, cbuf2.pix[x - cbuf1.length + cbuf2.length * y].r,
                            cbuf2.pix[x - cbuf1.length + cbuf2.length * y].g,
                            cbuf2.pix[x - cbuf1.length + cbuf2.length * y].b);
         }
      }
   }

   return true;
}

