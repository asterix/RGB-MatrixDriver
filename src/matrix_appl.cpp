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


// Font store
fontizer fz("./bdf-fonts/9x15B.bdf");



int main(int argc, char *arg[])
{
   rgb_matrix mtrx(MATRIX_LEN, MATRIX_HGT, COLOR_DEPTH, NUM_CHAINED);
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
   static bool fresh = true;
   static color_buffer clrbuf;
   static uint32_t xscroll = 0;
   
   // Set text colors
   pixel fore;
   fore.r = 238; fore.g = 154; fore.b = 0;
   pixel back;
   back.r = back.g = back.b = 0;

   std::string sample = "Isn't this so the coolest thing on this planet? :)   ";

   if(fresh)
   {
      // Determine color buffer size
      int fboxw, fboxh;
      fz.get_font_properties(fboxw, fboxh);

      // Clear and free pixels before overwriting
      clrbuf.clear();
   
      // Set color buffer params
      clrbuf.length = fboxw * sample.size();
      clrbuf.height = height_;
      clrbuf.depth = 8;
   
      // Allocate pixel memory
      clrbuf.pix = new pixel[clrbuf.length * clrbuf.height];
   
      // Start at (0,0)
      int xcursor = 0;
   
      for(size_t i = 0; i < sample.size(); i++)
      {
         xcursor += fz.draw(&clrbuf, xcursor, 0, &fore, &back, sample[i]);
      }

      fresh = false;
   }

   pixel* scroll_ptr = clrbuf.pix + xscroll;

   // Load into frame-buffer
   for(uint32_t y = 0; y < height_; y++)
   {
      uint32_t col = y * clrbuf.length;
      for(uint32_t x = 0; x < length_; x++)
      {
         if(x < clrbuf.length)
         {
            set_pixel(x, y, scroll_ptr[col + x].r,
                            scroll_ptr[col + x].g,
                            scroll_ptr[col + x].b);
         }
         else
         {
            set_pixel(x, y, 0, 0, 0);
         }
      }
   }

   // Progressive scrolling
   if(xscroll >= (clrbuf.length - length_))
   {
      xscroll = 0;
   }
   else
   {
      xscroll++;
   }

   usleep(50 * 1000);
   return true;
}





// Usage examples' section
// Copy-paste the sections into relevant places for checkout
#if 0

// (1) Random colors
   // Copy into playground()
   for(uint32_t x = 0; x < length_; x++)
   {
      for(uint32_t y = 0; y < height_; y++)
      {
         set_pixel(x, y, rand()%255, rand()%255, rand()%255);
      }
   }
   return true;



// (2) Moving ball
   // Copy into playground()
   static uint32_t x = 3, y = 8;
   uint32_t l = 2;

   usleep(300 * 1000);

   if(x + l >= length_)
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
   }
   return true;



// (3)
   // Two emoticons :) :(
   ppm_parser pr;
   color_buffer cbuf1, cbuf2;

   pr.parse("./emoticons/smiley16none.ppm", &cbuf1);
   pr.parse("./emoticons/frownyw.ppm", &cbuf2);

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



// (4) Glyph printing - Standard output only!
   /*rgb_matrix mtrx;
   mtrx.run();

   // Wait for user input
   std::cout << "Press any key to exit" << std::endl;
   std::cin.get();

   mtrx.stop();*/

   fontizer fz("./bdf-fonts/9x15.bdf");
   std::string sample = "2,O'Clock";
   color_buffer clrbuf;

   // Determine color buffer size
   int fboxw, fboxh;
   fz.get_font_properties(fboxw, fboxh);

   // Set color buffer params
   clrbuf.length = fboxw * sample.size();
   clrbuf.height = 16; // Let's keep this fixed for now
   clrbuf.depth = 8;   // 8-bit color

   // Allocate pixel memory
   clrbuf.pix = new pixel[clrbuf.length * clrbuf.height];

   // Start at (0,0)
   int xcursor = 0;

   for(size_t i = 0; i < sample.size(); i++)
   {
      xcursor += fz.draw(&clrbuf, xcursor, 0, sample[i]);
   }

   // Screen print
   for(uint32_t y = 0; y < clrbuf.height; y++)
   {
      uint32_t col = y * clrbuf.length;
      for(uint32_t x = 0; x < clrbuf.length; x++)
      {
         if(clrbuf.pix[col + x].r == 255)
         {
            std::cout << "@";
         }
         else
         {
            std::cout << "-";
         }
      }
      std::cout << std::endl;
   }

   std::cin.get();

   return 0;



// (5) Scrolling text
static bool fresh = true;
   static color_buffer clrbuf;
   static uint32_t xscroll = 0;

   // Set text colors
   pixel fore;
   fore.r = 238; fore.g = 154; fore.b = 0;
   pixel back;
   back.r = back.g = back.b = 0;

   std::string sample = "Isn't this so cool? :)   ";

   if(fresh)
   {
      // Determine color buffer size
      int fboxw, fboxh;
      fz.get_font_properties(fboxw, fboxh);

      // Clear and free pixels before overwriting
      clrbuf.clear();

      // Set color buffer params
      clrbuf.length = fboxw * sample.size();
      clrbuf.height = height_;
      clrbuf.depth = 8;

      // Allocate pixel memory
      clrbuf.pix = new pixel[clrbuf.length * clrbuf.height];

      // Start at (0,0)
      int xcursor = 0;

      for(size_t i = 0; i < sample.size(); i++)
      {
         xcursor += fz.draw(&clrbuf, xcursor, 0, &fore, &back, sample[i]);
      }

      fresh = false;
   }

   pixel* scroll_ptr = clrbuf.pix + xscroll;

   // Load into frame-buffer
   for(uint32_t y = 0; y < height_; y++)
   {
      uint32_t col = y * clrbuf.length;
      for(uint32_t x = 0; x < length_; x++)
      {
         if(x < clrbuf.length)
         {
            set_pixel(x, y, scroll_ptr[col + x].r,
                            scroll_ptr[col + x].g,
                            scroll_ptr[col + x].b);
         }
         else
         {
            set_pixel(x, y, 0, 0, 0);
         }
      }
   }

   // Progressive scrolling
   if(xscroll >= (clrbuf.length - length_))
   {
      xscroll = 0;
   }
   else
   {
      xscroll++;
   }

   usleep(50 * 1000);
   return true;


// (6)


#endif

