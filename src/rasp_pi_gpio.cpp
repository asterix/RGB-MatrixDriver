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
Function:  Raspberry Pi GPIO access class
Created:   23-Apr-2016
---------------------------------------------------------------------------*/


#include "rasp_pi_gpio.h"


// Initialize GPIO
bool rasp_pi_gpio::init()
{
   bool result = true;
   void *gpio = nullptr;

   gpio_fd_ = open("/dev/mem", O_RDWR|O_SYNC);
   if(gpio_fd_ < 0)
   {
      LOG_ERROR("Could not open /dev/mem for I/O access. Try sudo'ing");
      result = false;
   }

   if(result)
   {
      // Map memory to CPU virtual address
      gpio = mmap(NULL,                // Generic address space
                  REG_BLOCK_LEN,       // IO registers' space length
                  PROT_READ|PROT_WRITE,// R/W access
                  MAP_SHARED,          // Allow others to reuse
                  gpio_fd_,            // File descr of mem map
                  GPIO_BASE);          // Memory start address

      if(gpio == MAP_FAILED)
      {
         LOG_ERROR("Memory mapping to virtual space failed");
         result = false;
      }

      close(gpio_fd_);
   }

   if(result)
   {
      io_port_ = (volatile uint32_t*)gpio;
      io_gpset_ = io_port_ + GPSET_OFF;
      io_gpclr_ = io_port_ + GPCLR_OFF;
      io_gplev_ = io_port_ + GPLEV_OFF;
   }

   return result;
}


// Set pin mode to input
void rasp_pi_gpio::set_mode_in(uint16_t p)
{
   *(io_port_ + (p/10)) &= ~(7 << ((p%10)*3));
}


// Set pin mode to output
void rasp_pi_gpio::set_mode_out(uint16_t p)
{
   this->set_mode_in(p);
   *(io_port_ + (p/10)) |= (1 << ((p%10)*3));
}


// Set pin mode to an alternative function
void rasp_pi_gpio::set_mode_alt(uint16_t p, rasp_pi_gpio::mode alt)
{
   this->set_mode_in(p);
   *(io_port_ + (p/10)) |= (alt << ((p%10)*3));
}


// Set a GPIO pin
void rasp_pi_gpio::set_pin(uint16_t p)
{
   *(io_gpset_ + (p/32)) = (1 << (p%32));
}


// Reset a GPIO pin
void rasp_pi_gpio::clr_pin(uint16_t p)
{
   *(io_gpclr_ + (p/32)) = (1 << (p%32));
}


// Read a GPIO pin
bool rasp_pi_gpio::read_pin(uint16_t p)
{
   return (*(io_gplev_ + (p/32)) & (1 << (p%32)));
}

