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
Function:  Parses PPM images into a color buffer
Created:   03-May-2016
---------------------------------------------------------------------------*/

#ifndef _PPM_PARSER_H_
#define _PPM_PARSER_H_


#include <fstream>
#include <string>
#include "logging.h"
#include "frame_buffer.h"


#define DEFAULT_ALPHA 100


struct color_buffer
{
   uint32_t height;
   uint32_t length;
   uint32_t depth;
   pixel* pix;

   color_buffer()
   {
      height = length = depth = 0;
      pix = nullptr;
   }

   void clear()
   {
      delete[] pix;
      pix = nullptr;
      height = length = 0;
   }

   ~color_buffer()
   {
      delete[] pix;
   }
};


class ppm_parser
{
private:
   std::ifstream fl_;
   std::ifstream flh_;

   // Parsers
   void parse_p3(color_buffer *buf);
   void parse_p6(color_buffer *buf);

public:

   ppm_parser()
   {

   }

   ~ppm_parser()
   {

   }

   void parse(std::string filename, color_buffer *buf);
};


#endif // _PPM_PARSER_H_

