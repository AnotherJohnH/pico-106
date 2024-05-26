//------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//------------------------------------------------------------------------------

#include <cstdio>
#include <unistd.h>

#include "MTL/MTL.h"
#include "MTL/rp2040/Pwm.h"

#include "Table_gamma_8.h"


//! DCO control
template <unsigned PIN>
class DCO
{
public:
   void setPeriod(unsigned period_)
   {
      period = period_;

      pwm.setPeriod(period_);
   }

   void setLevel(unsigned level_)
   {
      pwm     = level_ * period / 100;
      pwm_alt = level_ * period / 100;
   }

   void setNote(unsigned midi_note_)
   {
   }

private:
   const unsigned CLK_DIV = (36<<4) + 14;

   unsigned        period;
   MTL::Pwm<PIN>   pwm{CLK_DIV};
   MTL::Pwm<PIN+1> pwm_alt{CLK_DIV};
};


DCO<MTL::PIN_6> dco;


int MTL_main()
{
   // Clear screen and cursor to home
   printf("\e[2J");
   printf("\e[1,1H");

   printf("\n");
   printf("Program  : pico106\n");
   printf("Author   : Copyright (c) 2024 John D. Haughton\n");
   printf("Version  : %s\n", MTL_VERSION);
   printf("Commit   : %s\n", MTL_COMMIT);
   printf("Built    : %s %s\n", __TIME__, __DATE__);
   printf("Compiler : %s\n", __VERSION__);
   printf("\n");

   dco.setPeriod(0x1000);

   while(true)
   {
       unsigned level = 1;

       while(level < 99)
       {
          dco.setLevel(level++);
          usleep(20000);
       }

       while(level > 1)
       {
          dco.setLevel(level--);
          usleep(20000);
       }
   }

   return 0;
}
