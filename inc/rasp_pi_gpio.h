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


#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include "logging.h"


// Set the correct peripheral addresses
// Ref: BCM2835 ARM Peripherals
#define RASP_PI_2        1

#ifdef RASP_PI_2
#define RASP_PI_X_BASE   0x3F000000
#else
#define RASP_PI_X_BASE   0x20000000
#endif


// GPxxx register offsets
#define GPIO_BASE        (RASP_PI_X_BASE + 0x200000) // GPFSEL0
#define GPSET_OFF        0x07 // GPSET0
#define GPCLR_OFF        0x0A // GPCLR0
#define GPLEV_OFF        0x0D // GPLEV0


// Mem mapped access space
#define GIO_BLOCK_LEN    0xCE
#define CLK_BLOCK_LEN    0x1C8
#define PWM_BLOCK_LEN    0x48


// PWM register offsets
#define PWM_BASE         (RASP_PI_X_BASE + 0x20C000)
#define PWM_CTL_OFF      0x00
#define PWM_STA_OFF      0x01
#define PWM_DMAC_OFF     0x02
#define PWM_RNG1_OFF     0x04
#define PWM_DAT1_OFF     0x05
#define PWM_FIF1_OFF     0x06
#define PWM_RNG2_OFF     0x08
#define PWM_DAT2_OFF     0x09


// Clock management register offsets
#define CLK_BASE         (RASP_PI_X_BASE + 0x101000)
#define CM_PWMCTL_OFF    0x28
#define CM_PWMDIV_OFF    0x29


// CLK CTL bits
#define CM_CTL_SRC(x)    ((x) << 0)
#define CM_CTL_ENAB      (1 << 4)
#define CM_CTL_KILL      (1 << 5)
#define CM_CTL_BUSY      (1 << 7)
#define CM_CTL_FLIP      (1 << 8)
#define CM_CTL_MASH(x)   ((x) << 9)
#define CLK_PASSWD       (0x5A << 24)
#define CM_SRC_OSC       1
#define CM_SRC_PLLD      6


// CLK DIV bits
#define CM_DIV_DIVF(x)   ((x) << 0)
#define CM_DIV_DIVI(x)   ((x) << 12)



//---------
//  GPIO
//---------

// GPIO + Peripheral accessor
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

protected:   
   void* mmap_peripheral(uint32_t base, uint32_t len);

public:
   rasp_pi_gpio(): gpio_fd_(-1),
                   io_port_(nullptr),
                   io_gpset_(nullptr),
                   io_gpclr_(nullptr),
                   io_gplev_(nullptr)
   {
      LOG_DEBUG("Constructor of rasp_pi_gpio");
   }

   virtual ~rasp_pi_gpio()
   {
      LOG_DEBUG("Destructor of rasp_pi_gpio");
      if(io_port_ != nullptr)
      {
         munmap((void *)io_port_, GIO_BLOCK_LEN);
      }
   }

   bool init();
   void set_mode_in(uint16_t pin);
   void set_mode_out(uint16_t pin);
   void set_mode_alt(uint16_t pin, mode alt);

   void set_pin(uint16_t pin);
   void clr_pin(uint16_t pin);

   bool read_pin(uint16_t pin);
   
   void set_gpset0(uint32_t w);
   void set_gpclr0(uint32_t w);
};



//---------
//  PWM
//---------

#define CM_PLLD_FREQ     500 // MHz
#define CM_PWM_FREQ      CM_PLLD_FREQ

// PWM accessor
class rasp_pi_pwm : private rasp_pi_gpio
{
public:
   enum ctl_bits
   {
      PWMEN1 = 0,
      MODE1 = 1,
      RPTL1 = 2,
      SBIT1 = 3,
      POLA1 = 4,
      USEF1 = 5,
      CLRF1 = 6,
      MSEN1 = 7,
      PWMEN2 = 8,
      MODE2 = 9,
      RPTL2 = 10,
      SBIT2 = 11,
      POLA2 = 12,
      USEF2 = 13,
      MSEN2 = 15
   };

   enum sta_bits
   {
      FULL1 = 0,
      EMPT1 = 1,
      WERR1 = 2,
      RERR1 = 3,
      GAPO1 = 4,
      GAPO2 = 5,
      GAPO3 = 6,
      GAPO4 = 7,
      BERR  = 8,
      STA1  = 9,
      STA2  = 10,
      STA3  = 11,
      STA4  = 12
   };

   enum dmac_bits
   {
      ENAB = 31
   };

protected:
   volatile uint32_t *clkm_;
   volatile uint32_t *pwm_;
   volatile uint32_t *pwm_fifo_;

private:
   uint32_t divider_;
   uint32_t clkfreq_;

   void pwm_setup(uint32_t divider);
   void pwm_config();
   bool pwm_init();

public:
   rasp_pi_pwm()
      : clkm_(nullptr), pwm_(nullptr), divider_(0), clkfreq_(0)
   {
      LOG_DEBUG("Constructor of rasp_pi_pwm");
   }

   virtual ~rasp_pi_pwm()
   {
      LOG_DEBUG("Destructor of rasp_pi_pwm");
      if(clkm_ != nullptr)
      {
         // Stop clocks
         *(clkm_ + CM_PWMCTL_OFF) = (CLK_PASSWD | CM_CTL_KILL);
         munmap((void *)clkm_, CLK_BLOCK_LEN);
      }

      if(pwm_ != nullptr)
      {
         *(pwm_ + PWM_CTL_OFF) = 0;
         munmap((void *)pwm_, PWM_BLOCK_LEN);
      }
   }

   bool pwm_module_setup(uint32_t divider);
   void pwm_pulse(uint32_t t);
   void pwm_wait_fifo_empty();

};


#endif // _RASP_PI_GPIO_H_

