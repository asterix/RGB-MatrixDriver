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


#include "glyph_loader.h"
#include <cmath>


void glyph_loader::load_bdf_glyphs(std::string font_file)
{
   // Some error checking
   bool fbox = false, fstart = false, fend = false;
   uint32_t numchars = 0;

   // Find & open font file
   std::ifstream fontfile(font_file.c_str(), std::ifstream::in);

   if(fontfile.is_open())
   {
      std::string keyword;

      // First find STARTFONT
      while(fontfile >> keyword)
      {  
         if(keyword == STARTFONT)
         {
            fstart = true;
            break;
         }
      }
      
      // Next find FONTBOUNDINGBOX
      if(fstart)
      {
         while(fontfile >> keyword)
         {
            if(keyword == FONTBOX)
            {
               fbox = true;
               break;
            }
         }
      }

      if(fbox)
      {
         fontfile >> fbox_width_ >> fbox_height_ >> fbox_xoff_ >> fbox_yoff_;

         // Start reading font bitmaps
         while(!fontfile.eof())
         {
            bool enc = false, bbx = false, bmap = false;
            uint32_t codepoint;
            glyph* glyf = new glyph;

            // Find ENCODING - UTF code-point
            while(fontfile >> keyword)
            {
               if(keyword == ENCODING)
               {
                  fontfile >> codepoint;
                  enc = true;
                  break;
               }
            }

            // Find BBX - glyph box params
            if(enc)
            {
               while(fontfile >> keyword)
               {
                  if(keyword == GLYPHBOX)
                  {
                     fontfile >> glyf->width >> glyf->height >> glyf->woff >> glyf->hoff;
                     bbx = true;
                     break;
                  }
               }
            }
            
            // Find BITMAP
            if(bbx)
            {
               while(fontfile >> keyword)
               {
                  if(keyword == BITMAP)
                  {
                     bmap = true;
                     break;
                  }
               }
            }
            
            // Contains only as many as "glyph height" rows
            if(bmap)
            {
               // Need to shift glyph to align it left
               // Ref: Glyph BDF Specification (Adobe_BDF_Spec.pdf)
               int align = BMAPWIDTH - (static_cast<int>
                                        (std::ceil(static_cast<double>
                                        (glyf->width)/MINWIDTH)) * MINWIDTH) + glyf->woff;

               // Allocate memory for bitmap
               glyf->bitmap = new uint32_t[glyf->height];

               // Save stream format flags
               std::ios fstr_flags(nullptr);
               fstr_flags.copyfmt(fontfile);

               for(int i = 0; i < glyf->height; i++)
               {
                  if(fontfile >> std::hex >> glyf->bitmap[i])
                  {
                     glyf->bitmap[i] = glyf->bitmap[i] << align;
                  }
                  else
                  {
                     // Error! The font is not complete
                     break;
                  }
               }

               // Restore stream format flags
               fontfile.copyfmt(fstr_flags);

               // Check for ENDCHAR - hopefully the very next line
               while(fontfile >> keyword)
               {
                  if(keyword == ENDCHAR)
                  {
                     break;
                  }
               }
   
               // Store the glyph
               font_.insert(std::make_pair(codepoint, glyf));
               numchars++;

               // Check for ENDFONT or STARTCHAR
               fontfile >> keyword;
               if(keyword == ENDFONT)
               {
                  fend = true;
               }
            }
         }
      }

      // Found ENDFONT?
      if(!fend)
      {
         LOG_ERROR("Font file: "+ font_file+ " may be erroneous");
      }

      LOG_DEBUG("Loaded "+ std::to_string(numchars)+ " font characters");

      // Don't forget to close!
      fontfile.close();
   }
   else
   {
      LOG_ERROR("Could not open the font file: "+ font_file);
   }
}


const glyph* glyph_loader::get_glyph(uint32_t code_point)
{
   glyph *result = nullptr;
   font_map_typ::iterator it = font_.find(code_point);

   if(it != font_.end())
   {
      // :) font exists!
      result = it->second;
   }
   else
   {
      // :( no font for requested UTF code-point, return placeholder question mark
      it = font_.find(UNKNOWN_FONT);
      result = it->second;
   }

   return (static_cast<const glyph*>(result));
}


// Simple screen print 
void glyph_loader::print_glyph(uint32_t codepoint)
{
   const glyph* glyf = this->get_glyph(codepoint);
   
   for(int y = 0; y < glyf->height; y++)
   {
      for(int x = 0; x < glyf->width; x++)
      {
         if((glyf->bitmap[y] << x) & BMAPMASK)
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
}


void glyph_loader::get_fbox_params(int& fw, int& fh, int& xoff, int& yoff)
{
   fw = fbox_width_;
   fh = fbox_height_;
   xoff = fbox_xoff_;
   yoff = fbox_yoff_;
}

