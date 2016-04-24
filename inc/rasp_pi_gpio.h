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

#ifndef _RASP_PI_GPIO_H_
#define _RASP_PI_GPIO_H_

// Set the correct peripheral addresses
// Ref: BCM2835 ARM Peripherals
#define RASPBERRY_PI_2     1

#ifdef RASPBERRY_PI_2
#define RASP_PI_X_BASE   0x3F000000
#else
#define RASP_PI_X_BASE   0x20000000
#endif

// GPxxx register offsets
#define GPIO_BASE        (RASP_PI_X_BASE + 0x200000)
#define GPSET_OFF        0x07
#define GPCLR_OFF        0x0A
#define GPLEV_OFF        0x0D


#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>


// Logging macros
#define LOG_DEBUG(x) (std::cout << "DEBUG: " << x << std::endl)
#define LOG_ERROR(x) (std::cout << "ERROR: " << x << std::endl)

// Mem mapped IO space
#define REG_BLOCK_LEN    (4*1024)


// GPIO accessor
class rasp_pi_gpio
{
public:
   enum mode
   {
      INPT = 0,
      OUPT = 1,
      ALT0 = 4,
      ALT1 = 5,
      ALT2 = 6,
      ALT3 = 7,
      ALT4 = 3,
      ALT5 = 2
   };

private:
   int gpio_fd_;
   volatile uint32_t *io_port_;
   volatile uint32_t *io_gpset_;
   volatile uint32_t *io_gpclr_;
   volatile uint32_t *io_gplev_;

public:
   rasp_pi_gpio(): gpio_fd_(-1),
                   io_port_(nullptr),
                   io_gpset_(nullptr),
                   io_gpclr_(nullptr),
                   io_gplev_(nullptr)
   {
      // Nothing here
   }

   ~rasp_pi_gpio()
   {
      if(gpio_fd_ > 0)
      {
         munmap((void *)io_port_, REG_BLOCK_LEN);
      }
   }

   bool init();
   void set_mode_in(uint16_t pin);
   void set_mode_out(uint16_t pin);
   void set_mode_alt(uint16_t pin, mode alt);

   void set_pin(uint16_t pin);
   void clr_pin(uint16_t pin);

   bool read_pin(uint16_t pin);

};


#endif // _RASP_PI_GPIO_H_

