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
Function:  What? => what is shown on the display
Created:   25-Apr-2016
---------------------------------------------------------------------------*/

#include "what.h"
#include <unistd.h>


// Start running playground
void what::run()
{
   LOG_DEBUG("run() of what");
   run_.store(true);

   while(run_.load())
   {
      sleep(1);
      this->playground();
   }

   LOG_DEBUG("exit from run() of what");
}


// Terminate running
void what::stop()
{
   LOG_DEBUG("stop() of what");
   run_.store(false);
}

