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
Function:  RGB Matrix interface to GPIO
Created:   24-Apr-2016
---------------------------------------------------------------------------*/

#include "rgb_mtrx_ifc.h"


bool rgb_mtrx_ifc::startup_gpio()
{
   if(!io_.init())
   {
      LOG_ERROR("GPIO init failed :(");
      return false;
   }

   for(int i = 0; i < 28; i++)
   {
      if(full_mask_ & (1 << i))
      {
         io_.set_mode_out(i);
      }
   }

   LOG_DEBUG("Successful setup of rgb_mtrx_ifc");
   return true;
}


// Startup PWM Module
bool rgb_mtrx_ifc::startup_pwm(uint32_t pwm_bit)
{
   // t is cycle time needed in nano-seconds
   uint32_t div = ((CM_PWM_FREQ * pwm_bit) / (1000));
   
   LOG_DEBUG("PWM clock divider = "+ std::to_string(div));

   if(!pwm_.pwm_module_setup(div))
   {
      LOG_ERROR("PWM init failed :(");
      return false;
   }

   return true;
}


void rgb_mtrx_ifc::update_gpio(uint32_t mask)
{
   // Write straight to GPSET0 register
   io_.set_gpset0(bits_ & mask);

   // Write inverted values to GPCLR0 register
   io_.set_gpclr0(~bits_ & mask);
}

