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

#include "STB/MIDIInstrument.h"

#include "DCO.h"
#include "Voice.h"


class Synth : public MIDI::Instrument
{
public:
   Synth()
      : MIDI::Instrument(NUM_VOICE)
   {
      voice[0].setDCO(dco0);
      voice[1].setDCO(dco1);
      voice[2].setDCO(dco2);
      voice[3].setDCO(dco3);
   }

   bool isAnyVoiceOn() const { return active != 0; }

private:
   signed allocVoice() const override
   {
      for(unsigned i = 0; i < NUM_VOICE; ++i)
      {
         if (not voice[i].isActive()) return i;
      }
      return 0;
   }

   signed findVoice(uint8_t note_) const override
   {
      for(unsigned i = 0; i < NUM_VOICE; ++i)
      {
         if (voice[i].isPlaying(note_)) return i;
      }
      return -1;
   }

   void voiceOn(unsigned index_, uint8_t note_, uint8_t velocity_) override
   {
      voice[index_].noteOn(note_);

      ++active;
   }

   void voiceOff(unsigned index_, uint8_t velocity_) override
   {
      voice[index_].noteOff();
   }

   static const unsigned MAX_VOICE = 4;
   static const unsigned NUM_VOICE = 2;

   Voice                voice[MAX_VOICE];
   unsigned             active{0};
   DCO_PWM<MTL::PIN_4>  dco0;
   DCO_PWM<MTL::PIN_6>  dco1;
   DCO_PWM<MTL::PIN_9>  dco2;
   DCO_PWM<MTL::PIN_11> dco3;
};
