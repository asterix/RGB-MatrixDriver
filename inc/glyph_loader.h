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
Function:  Loads BDF fonts and holds for access
Created:   06-May-2016
---------------------------------------------------------------------------*/


#ifndef _GLYPH_LOADER_H_
#define _GLYPH_LOADER_H_


#include <string>
#include <map>
#include <iterator>
#include <fstream>
#include "logging.h"



// Placeholder font
#define UNKNOWN_FONT 0xFFFD


// BDF font file keywords
#define STARTFONT "STARTFONT"
#define FONTBOX   "FONTBOUNDINGBOX"
#define ENDFONT   "ENDFONT"

#define STARTCHAR "STARTCHAR"
#define ENDCHAR   "ENDCHAR"
#define ENCODING  "ENCODING"
#define GLYPHBOX  "BBX"
#define BITMAP    "BITMAP"


// Storing and retrieving constants
#define BMAPWIDTH 32
#define MINWIDTH  8
#define BMAPMASK  0x80000000


struct glyph
{
   int width;
   int height;
   int woff;
   int hoff;
   uint32_t *bitmap;

   glyph():
      width(0), height(0), woff(0), hoff(0),
      bitmap(nullptr)
   {
      
   }

   ~glyph()
   {

   }
};


// Map of UTF code-point to corresponding font glyph
typedef std::map<uint32_t, glyph*> font_map_typ;


class glyph_loader
{
private:
   // Global font bounding box params
   int fbox_width_;
   int fbox_height_;
   int fbox_xoff_;
   int fbox_yoff_;

   // Holds a map of UTF code point to its Glyph
   font_map_typ font_;

   // Load font glyphs
   void load_bdf_glyphs(std::string font_file);

public:

   glyph_loader() = delete;


   glyph_loader(std::string font_file):
      fbox_width_(0), fbox_height_(0), fbox_xoff_(0), fbox_yoff_(0)
   {
      // Clear all mapped data
      font_.clear();
      this->load_bdf_glyphs(font_file);
   }

   ~glyph_loader()
   {
      // Release all glyph memory
      for(auto it = font_.begin(); it != font_.end(); it++)
      {
         delete[] it->second->bitmap;
         delete it->second;
      }
   }

   const glyph* get_glyph(uint32_t code_point);
   void print_glyph(uint32_t codepoint);
   void get_fbox_params(int& fw, int& fh, int& xoff, int& yoff);

};


#endif // _GLYPH_LOADER_H_

