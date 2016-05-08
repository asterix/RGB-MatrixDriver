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


#ifndef _FONTIZER_H_
#define _FONTIZER_H_


#include "glyph_loader.h"
#include "ppm_parser.h"
#include "frame_buffer.h"
#include "logging.h"


class fontizer
{
private:
   glyph_loader glyfer_;
   int baseline_;

public:

   fontizer() = delete;

   fontizer(std::string fontfile)
      : glyfer_(fontfile), baseline_(0)
   {
      int h, hoff, dummy;
      this->get_font_properties(dummy, h, dummy, hoff);
      baseline_ = h + hoff;
   }

   ~fontizer()
   {

   }

   // Uses white foreground and black background
   int draw(color_buffer *clrbuf, uint32_t xpos, uint32_t ypos, uint32_t codept);

   // Accepts foreground and background colors
   int draw(color_buffer *clrbuf, uint32_t xpos, uint32_t ypos,
            pixel *foreclr, pixel *backclr, uint32_t codept);

   // Global font properties
   void get_font_properties(int& w_fbox, int& h_fbox);
   void get_font_properties(int& w_fbox, int& h_fbox, int& w_off, int& h_off);

};


#endif // _FONTIZER_H_

