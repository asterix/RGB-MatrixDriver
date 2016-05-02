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


//---------
//  GPIO
//---------


// Initialize GPIO
bool rasp_pi_gpio::init()
{
   bool result = true;
   void *gpio;

   // Map GPIO
   gpio = mmap_peripheral(GPIO_BASE, GIO_BLOCK_LEN);
   
   if(gpio != MAP_FAILED)
   {
      LOG_DEBUG("Successful map of GPIO");
      io_port_ = (volatile uint32_t*)gpio;
      io_gpset_ = io_port_ + GPSET_OFF;
      io_gpclr_ = io_port_ + GPCLR_OFF;
      io_gplev_ = io_port_ + GPLEV_OFF;
   }
   else
   {
      result = false;
   }

   return result;
}


void* rasp_pi_gpio::mmap_peripheral(uint32_t base, uint32_t len)
{
   bool result = true;
   void *periph = MAP_FAILED;

   gpio_fd_ = open("/dev/mem", O_RDWR|O_SYNC);
   if(gpio_fd_ < 0)
   {
      LOG_ERROR("Could not open /dev/mem for peripheral access. Try sudo'ing");
      result = false;
   }

   if(result)
   {
      // Map memory to CPU virtual address
      periph = mmap(NULL,                // Generic address space
                    len,                 // IO registers' space length
                    PROT_READ|PROT_WRITE,// R/W access
                    MAP_SHARED,          // Allow others to reuse
                    gpio_fd_,            // File descr of mem map
                    base);               // Memory start address

      if(periph == MAP_FAILED)
      {
         LOG_ERROR("Memory mapping to virtual space failed");
      }

      close(gpio_fd_);
   }

   return periph;
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


// Set full registers
void rasp_pi_gpio::set_gpset0(uint32_t w)
{
   *io_gpset_ = w;
}

void rasp_pi_gpio::set_gpclr0(uint32_t w)
{
   *io_gpclr_ = w;
}



//---------
//  PWM
//---------

// Initialize PWM Module
bool rasp_pi_pwm::pwm_module_setup(uint32_t divider)
{
   divider_ = divider;
   clkfreq_ = CM_PLLD_FREQ / divider;
   
   bool result = this->pwm_init();

   if(result)
   {
      this->pwm_config();
      this->pwm_setup(divider);
   }

   return result;
}


bool rasp_pi_pwm::pwm_init()
{
   bool result = true;
   void *clk, *pwm;

   // Map Clock management
   clk  = this->mmap_peripheral(CLK_BASE, CLK_BLOCK_LEN);

   if(clk != MAP_FAILED)
   {
      LOG_DEBUG("Successful map of Clock Management");
      clkm_ = (volatile uint32_t*)clk;
   }
   else
   {
      result = false;
   }

   // Map PWM
   pwm  = this->mmap_peripheral(PWM_BASE, PWM_BLOCK_LEN);

   if(pwm != MAP_FAILED)
   {
      LOG_DEBUG("Successful map of PWM");
      pwm_ = (volatile uint32_t*)pwm;
      pwm_fifo_ = pwm_ + PWM_FIF1_OFF;
   }
   else
   {
      result = false;
   }

   return result;
}


// Configure PWM clocks
void rasp_pi_pwm::pwm_setup(uint32_t divider)
{
   // Only use least 12-bits
   divider &= 0xFFF;

   // Stop clocks
   *(clkm_ + CM_PWMCTL_OFF) = (CLK_PASSWD | CM_CTL_KILL);
   usleep(10);

   // Set clock source - PLLD = 500MHz
   *(clkm_ + CM_PWMCTL_OFF) = (CLK_PASSWD | CM_CTL_SRC(CM_SRC_PLLD));
   usleep(10);

   // Set divider
   *(clkm_ + CM_PWMDIV_OFF) = (CLK_PASSWD | CM_DIV_DIVI(divider) | CM_DIV_DIVF(0));
   usleep(10);

   // Turn on clock
   *(clkm_ + CM_PWMCTL_OFF) = (CLK_PASSWD | CM_CTL_ENAB | CM_CTL_SRC(CM_SRC_PLLD));
   usleep(10);

   // Wait until ON
   while((*(clkm_ + CM_PWMCTL_OFF) & CM_CTL_BUSY) == 0)
   {
      asm("");
   }
}


// Configure PWM module
void rasp_pi_pwm::pwm_config()
{
   // Setup GPIO 18 to alternate 5 function
   this->init();
   this->set_mode_alt(18, ALT5);

   // PWM using FIFO inverted output on GPIO 18
   *(pwm_ + PWM_CTL_OFF) = ((1 << CLRF1) | (1 << USEF1) | (1 << POLA1));
   usleep(10);
}


// Generate a pulse of time t(ns)
void rasp_pi_pwm::pwm_pulse(uint32_t t)
{
   uint32_t range;
   if(t < 32)
   {
      range = t/2;

      // RNG - Contains period of PWM = M
      *(pwm_ + PWM_RNG1_OFF) = 2;

      // FIFO - Contains duty cycle of PWM = N
      for(uint32_t i = 0; i < range; i++)
      {
         *(pwm_ + PWM_FIF1_OFF) = 2;
      }

      // PWM form -> N high cycles out of M cycles, t/2 times (inverted if POLA1 = 1)
   }
   else
   {
      // Long range => long wait periods when sending terminating 0 from FIFO
      // Shorten range as much as possible while keeping it >= 2
      range = t/8;

      // RNG - Contains period of PWM = M
      *(pwm_ + PWM_RNG1_OFF) = range;

      // FIFO - Contains duty cycle of PWM = N
      for(int i = 0; i < 8; i++)
      {
         *(pwm_ + PWM_FIF1_OFF) = range;
      }

      // PWM form -> N high cycles out of M cycles, 8 times (inverted if POLA1 = 1)
   }

   // Termination and return to normal state
   *(pwm_ + PWM_FIF1_OFF) = 0;
   *(pwm_ + PWM_FIF1_OFF) = 0;

   // Turn ON PWM
   *(pwm_ + PWM_CTL_OFF) = ((1 << USEF1) | (1 << POLA1) | (1 << PWMEN1));
}


// Check if all bits are pushed out
void rasp_pi_pwm::pwm_wait_fifo_empty()
{
   while((*(pwm_ + PWM_STA_OFF) & (1 << EMPT1)) == 0)
   {
      asm("");
   }

   // Clear FIFO - This line crashes PWM module?
   //*(pwm_ + PWM_CTL_OFF) = ((1 << CLRF1) | (1 << USEF1) | (1 << POLA1));
}

