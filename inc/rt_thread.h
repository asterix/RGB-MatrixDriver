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
   enum thread_prio
   {
      // Change scheduling policy SCHED_FIFO or SCHED_RR
      // 1-99, 99 being the highest priority
      NORMAL = 0,
      HPRIO = 99, 
      MPRIO = 50, 
      LPRIO = 10,
      OTHER = 3
   };

   enum cpu_core
   {
      CPU_CORE0 = 0,
      CPU_CORE1,
      CPU_CORE2,
      CPU_CORE3
   };

private:
   int prio_;
   uint32_t cpu_aff_;
   int policy_;
   pthread_t th_;

   static void* thread_launch_fptr(void *fn);
   
protected:

   // Implemented in the derived class
   virtual void run() = 0;

public:
   rt_thread()
      : prio_(NORMAL), cpu_aff_(0), policy_(SCHED_OTHER)
   {
      // Not an RT thread
   }

   rt_thread(int pri, uint32_t cpuaff, int sch = SCHED_FIFO)
      : prio_(pri), cpu_aff_(cpuaff), policy_(sch)
   {
      // RT thread
   }

   virtual ~rt_thread()
   {

   }

   bool run_as_thread();
   bool wait_on_thread();

};


#endif // _RT_THREAD_H_

