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


#include "rgb_matrix.h"
#include "pix_driver.h"
#include "what.h"



void rgb_matrix::startup()
{
   // Create/initialize frame buffer
   frame_buf_ = new frame_buffer(length_ * num_in_chain_, height_, depth_);
}


void what::playground()
{
   // Your code goes here
   while(1);
}


// This is merry downtown!
void rgb_matrix::run()
{
   LOG_DEBUG("run() of rgb_matrix");
   
   // Spawn Pixel Driver
   pix_driver pixel_drv(frame_buf_);
   pixel_drv.run_as_thread();

   // Spawn What
   what show_what(frame_buf_);
   show_what.run_as_thread();

   // Wait for signal to terminate
   show_what.wait_on_thread();

   pixel_drv.stop();
   pixel_drv.wait_on_thread();
}

