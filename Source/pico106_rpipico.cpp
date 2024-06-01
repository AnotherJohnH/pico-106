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
#include "MTL/Config.h"
#include "MTL/rp2040/Pwm.h"

#include "Table_exp_24.h"


//! DCO control
template <unsigned PIN>
class DCO
{
public:
   void setPeriod(unsigned period_)
   {
      period = period_;
   }

   void setLevel(unsigned level_)
   {
      pwm     = level_ * period / 100;
      pwm_alt = level_ * period / 100;
   }

   void setNote(unsigned midi_note_)
   {
      const unsigned A4_MIDI = 69;
      const unsigned A4_FREQ = 440;

      printf("MIDI %03u ", midi_note_);
      signed   midi_rel_a4 = midi_note_ - A4_MIDI;
      signed   note_16     = midi_rel_a4 * 4096 / 12;
      printf("=> note16 0x%4x ", note_16 + 0x8000);
      unsigned freq_16 = table_exp_24[note_16 + 0x8000] * A4_FREQ;
      printf("=> freq16 0x%8x ", freq_16);
      unsigned count   = 0x10000 * (MIN_FREQ << 12) / (freq_16 >> 4);
      printf("=> count 0x%x", count);

      pwm.setPeriod(count);

      unsigned level = 0x20;
      printf("=> level 0x%x\n", level);

      pwm     = level;
      pwm_alt = level;
   }

private:
   const unsigned MIN_FREQ = 8;
   const unsigned PWM_FREQ = MIN_FREQ * 0x10000;
   const unsigned CLK_DIV  = CLOCK_FREQ * 16 / PWM_FREQ;

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

   dco.setNote(69);

   getchar();

   while(true)
   {
#if 0
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
#endif

      for(unsigned midi_note = 0; midi_note < 128; midi_note++)
      {
         usleep(500000);
         dco.setNote(midi_note);
      }
   }

   return 0;
}
