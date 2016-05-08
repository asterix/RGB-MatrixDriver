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
Function:  Fills pixels for a UTF code using a loaded font
Created:   07-May-2016
---------------------------------------------------------------------------*/


#include "fontizer.h"


void fontizer::get_font_properties(int& w_fbox, int& h_fbox)
{
   int dummy;
   glyfer_.get_fbox_params(w_fbox, h_fbox, dummy, dummy);
}


void fontizer::get_font_properties(int& w_fbox, int& h_fbox, int& w_off, int& h_off)
{
   glyfer_.get_fbox_params(w_fbox, h_fbox, w_off, h_off);
}


// Default black-white contrast
int fontizer::draw(color_buffer *clrbuf, uint32_t xpos, uint32_t ypos, uint32_t codept)
{
   pixel black;
   black.r = black.g = black.b = 0;
   black.a = DEFAULT_ALPHA;

   pixel white;
   white.r = white.g = white.b = 255;
   white.a = DEFAULT_ALPHA;

   return (this->draw(clrbuf, xpos, ypos, &white, &black, codept));
}


// Full free color contrast
int fontizer::draw(color_buffer *clrbuf, uint32_t xpos, uint32_t ypos,
                   pixel *foreclr, pixel *backclr, uint32_t codept)
{
   // Get glyph for code-point
   const glyph* glyf = glyfer_.get_glyph(codept);

   // Let's draw!
   for(int y = 0; y < glyf->height; y++)
   {
      if((ypos + y) < clrbuf->height)
      {
         // Pre-calculate column jump
         uint32_t col = (ypos + y) * clrbuf->length;
         
         // Starting point for each row
         pixel *row = clrbuf->pix + col + xpos;         

         for(int x = 0; x < glyf->width; x++)
         {
            if((xpos + x) < clrbuf->length)
            {
               if((glyf->bitmap[y] << x) & BMAPMASK)
               {
                  row[x].r = foreclr->r;
                  row[x].g = foreclr->g;
                  row[x].b = foreclr->b;
                  row[x].a = foreclr->a;
               }
               else
               {
                  row[x].r = backclr->r;
                  row[x].g = backclr->g;
                  row[x].b = backclr->b;
                  row[x].a = backclr->a;
               }
            }
         }
      }
   }

   return glyf->width;
}

