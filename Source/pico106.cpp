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
#include "MTL/rp2040/Uart.h"

#include "STB/MIDIInterface.h"

#include "Synth.h"


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

   Synth   synth;
   MidiIn0 midi_in0{synth};
   MidiIn1 midi_in1{synth};

   while(true)
   {
      midi_in0.tick();
      midi_in1.tick();
   }
}
