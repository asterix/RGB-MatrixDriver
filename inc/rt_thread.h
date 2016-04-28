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


#ifndef _RT_THREAD_H_
#define _RT_THREAD_H_


#include <pthread.h>
#include "logging.h"


class rt_thread
{
public:
   enum
   {
      // Change scheduling policy SCHED_FIFO or SCHED_RR
      // 1-99, 99 being the highest priority
      NORMAL = 0,
      HPRIO = 10, 
      MPRIO = 9, 
      LPRIO = 8,
      OTHER = 3
   };

private:
   int prio_;
   int policy_;
   pthread_t th_;

   static void (*thread_launch_fptr)(void *fn);
   
protected:

   // Implemented in the derived class
   virtual void run();

public:
   rt_thread(int pri = LPRIO, int sch = SCHED_FIFO)
      : prio_(pri), policy_(sch)
   {

   }

   ~rt_thread()
   {

   }

   bool run_as_thread();

};


#endif // _RT_THREAD_H_

