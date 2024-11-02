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

#pragma once

#include "MTL/Config.h"
#include "MTL/chip/Pwm.h"
#include "MTL/chip/PioClkLoFreq.h"

#include "Table_exp_24.h"


class DCO
{
public:
   virtual void noteOn(unsigned midi_note_) = 0;
   virtual void noteOff() = 0;
};


//! DCO implement via PWM
template <typename PIO, unsigned PIN>
class DCO_PWM : public DCO
{
public:
   DCO_PWM()
   {
      noteOff();
   }

   void noteOn(unsigned midi_note_) override
   {
      const unsigned A4_MIDI = 69;
      const unsigned A4_FREQ = 440;

      signed   midi_rel_a4 = midi_note_ - A4_MIDI;
      signed   note_16     = midi_rel_a4 * 4096 / 12;
      printf("=> note16 %4x ", note_16 + 0x8000);

      unsigned freq_16 = table_exp_24[note_16 + 0x8000] * A4_FREQ;
      printf("=> freq16 %8x ", freq_16);

      unsigned count   = 0x10000 * (MIN_FREQ << 12) / (freq_16 >> 4);
      printf("=> count %4x", count);

      dac.setPeriod(count);

      unsigned level = 0x30;

      if (level >= count)
         level = count - 1;

      printf("=> level 0x%x\n", level);

      clk = freq_16 >> 8;
      dac = level;
   }

   void noteOff() override
   {
      clk = 1000000 << 8;
      dac = 0;
   }

private:
   const unsigned MIN_FREQ = 8;
   const unsigned PWM_FREQ = MIN_FREQ * 0x10000;
   const unsigned CLK_DIV  = CLOCK_FREQ * 16 / PWM_FREQ;

   MTL::PioClkLoFreq<PIO,PIN> clk{};
   MTL::Pwm<PIN + 1>          dac{CLK_DIV};
};

