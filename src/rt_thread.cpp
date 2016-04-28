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
Function:  pthread wrapper for Real-Time threads (portable? may be not!)
Created:   26-Apr-2016
---------------------------------------------------------------------------*/

#include "rt_thread.h"


void *rt_thread::thread_launch_fptr(void *fn)
{
   (static_cast<rt_thread>(fn))->run();
}


bool rt_thread::run_as_thread()
{
   int result = pthread_create(&th_, NULL, thread_launch_fptr, this);

   if(result != 0)
   {
      LOG_ERROR("Thread could not be created :( Failed with error code: "
            + std::to_string(result));
   }
   else
   {
      // Bump up to Real-Time priority
      if(prio_ > 0)
      {
         struct sched_param sch;
         sch.p = prio_;
         pthread_setschedparam(th_, policy_, &sch);
      }
   }

   return (result == 0);
}

