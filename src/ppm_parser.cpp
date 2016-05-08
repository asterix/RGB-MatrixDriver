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

#include "ppm_parser.h"
#include <regex>
#include <cstdlib>



void ppm_parser::parse(std::string filename, color_buffer *buf)
{
   std::string line;

   // Clear buffer if something already exists
   buf->clear();

   // Try to open file
   fl_.open(filename.c_str(), std::ios::in | std::ios::binary);
   flh_.open(filename.c_str(), std::ifstream::in);

   if(flh_.is_open() && fl_.is_open())
   {
      // Get magic string - format
      std::getline(fl_, line);

      if(line == "P6")
      {
         // Single byte color - binary file
         this->parse_p6(buf);
      }
      else if(line == "P3")
      {
         // Possible multi-byte color - ASCII
         this->parse_p3(buf);
      }
      else
      {
         LOG_ERROR("Unknown PPM format. Check input file.");
      }
   }
   else
   {
      LOG_ERROR("File "+ filename+ " could not be opened :(");
   }

   fl_.close();
   flh_.close();
}


// Parse a P6 binary formatted PPM image
void ppm_parser::parse_p6(color_buffer *buf)
{
   bool hh = false, ll = false, max = false;
   std::string line, subl;
   std::regex rgxnum("[0-9]+");

   while(!(hh && ll && max))
   {
      std::getline(fl_, line);
      
      // Truncate and throw away comment part
      size_t posc = line.find('#');
      line = line.substr(0, (posc != std::string::npos)? posc : std::string::npos);

      // If the line still has something
      if(line.size() != 0)
      {
         do
         {
            // Any space?
            size_t poss = line.find(' ');

            // Subl = useful piece (?) until a space
            subl = line.substr(0, poss);

            // Line = remaining line
            line.erase(0, poss == std::string::npos? std::string::npos : poss + 1);
            
            if(std::regex_match(subl, rgxnum))
            {
               if(!ll)
               {
                  buf->length = std::stoi(subl);
                  ll = true;
               }
               else if(!hh)
               {
                  buf->height = std::stoi(subl);
                  hh = true;
               }
               else if(!max)
               {
                  buf->depth = std::stoi(subl);
                  max = true;
               }
               else
               {
                  // Shouldn't reach
                  break;
               }
            }

         } while(line.size() != 0);
      }     
   }

   // Now seek the binary reader to the point after header
   //LOG_DEBUG("P6 image length = "+ std::to_string(buf->length)
   //               + ", height = "+ std::to_string(buf->height)
   //               + ", depth = "+ std::to_string(buf->depth));

   uint32_t i, num_pixs = buf->length * buf->height;

   if(num_pixs != 0)
   {
      buf->pix = new pixel[num_pixs];
      for(i = 0; i < num_pixs; i++)
      {
         if(fl_.eof())
         {
            break;
         }
         buf->pix[i].r = static_cast<uint16_t>(fl_.get());
         buf->pix[i].g = static_cast<uint16_t>(fl_.get());
         buf->pix[i].b = static_cast<uint16_t>(fl_.get());
         buf->pix[i].a = DEFAULT_ALPHA;
      }

      if(i < num_pixs)
      {
         LOG_ERROR("P6 image had fewer pixels than what size says?");
      }
   }
   else
   {
      LOG_ERROR("P6 image had zero pixels?");
   }
}


void ppm_parser::parse_p3(color_buffer *buf)
{
   LOG_ERROR("P3 format not supported yet");
}

