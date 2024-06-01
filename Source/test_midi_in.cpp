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

#include "MTL/MTL.h"
#include "MTL/Digital.h"
#include "MTL/rp2040/Uart.h"

#include "STB/MIDIInterface.h"

// --- RP2040 module LED --------------------------------------------------------------

MTL::Digital::Out<MTL::PIN_LED1> led;


// --- MIDI-in -----------------------------------------------------------------

//! Physical MIDI in
class MidiIn1 : public MIDI::Interface
{               
public:         
   MidiIn1(MIDI::Instrument& instrument)
      : MIDI::Interface(instrument)
   {}

private:
   bool empty() const override { return uart.empty(); }
   
   uint8_t rx() override { return uart.rx(); }

   MTL::Uart1_ALT uart{/* baud */   31250,
                       /* bits */   8,
                       /* parity */ MTL::UART::NONE,
                       /* stop */   1};
};


// --- debug MIDI-in -----------------------------------------------------------

//! Slow MIDI in via the console UART
class MidiIn0 : public MIDI::Interface
{
public:
   MidiIn0(MIDI::Instrument& instrument)
      : MIDI::Interface(instrument)
   {}

   bool empty() const override { return MTL_getch_empty(); }

   uint8_t rx() override { return MTL_getch(); }
};


// -----------------------------------------------------------------------------

class Sink : public MIDI::Instrument
{
public:
   Sink()
      : MIDI::Instrument(/* num_voices */ 1)
   {
   }

   signed allocVoice() const override
   {
      return 0;
   }
   
   signed findVoice(uint8_t note_) const override
   {
      return 0;
   }

   void voiceOn(unsigned voice_, uint8_t note_, uint8_t velocity_) override
   {
      led = true;
   }

   void voiceOff(unsigned voice_, uint8_t velocity_) override
   {
      led = false;
   }
};


int MTL_main()
{
   printf("MIDI IN TEST\n");

   Sink    sink;
   MidiIn0 midi_in0{sink};
   MidiIn1 midi_in1{sink};

   midi_in0.setDebug(true);
   midi_in1.setDebug(true);

   while(true)
   {
      midi_in0.tick();
      midi_in1.tick();
   }
}

