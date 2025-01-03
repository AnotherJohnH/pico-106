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


template <unsigned NUM_VOICES>
class Synth : public MIDI::Instrument<NUM_VOICES>
{
public:
   Synth()
   {
      voice[0].setDCO(dco1);
      voice[1].setDCO(dco2);
      voice[2].setDCO(dco3);
      voice[3].setDCO(dco4);
      voice[4].setDCO(dco5);
      voice[5].setDCO(dco6);
   }

private:
   void voiceOn(unsigned index_, uint8_t note_, uint8_t velocity_) override
   {
      voice[index_].noteOn(note_);
   }

   void voiceOff(unsigned index_, uint8_t velocity_) override
   {
      voice[index_].noteOff();
   }

   // Provision for 6 DCOs
   static const unsigned MAX_VOICES = 6;

   Voice voice[MAX_VOICES];

   DCO_PWM<MTL::Pio0, MTL::PIN_4>  dco1;
   DCO_PWM<MTL::Pio0, MTL::PIN_6>  dco2;
   DCO_PWM<MTL::Pio0, MTL::PIN_9>  dco3;
   DCO_PWM<MTL::Pio1, MTL::PIN_11> dco4;
   DCO_PWM<MTL::Pio1, MTL::PIN_14> dco5;
   DCO_PWM<MTL::Pio1, MTL::PIN_16> dco6;
};
