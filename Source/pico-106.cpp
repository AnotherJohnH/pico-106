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

// \brief Synth for Raspberry Pi Pico (based around DCOs like the Juno 106)

#include <cstdio>

#include "STB/MIDIInterface.h"

#include "MTL/MTL.h"
#include "MTL/Digital.h"
#include "MTL/chip/Uart.h"

#include "Synth.h"


// --- Target Hardware Configuration -------------------------------------------

#define HW_MIDI_UART1
#define HW_MIDI_USB_DEVICE
#define HW_LED
#define HW_VOICES   3   // Number of external DCO circuits, max 6


// -----------------------------------------------------------------------------

static Synth<HW_VOICES> synth {};


// --- MIDI-IN -----------------------------------------------------------------

#if defined(HW_MIDI_UART1)

// pico pin 27 : IN

//! Physical MIDI at 31250 baud
class MidiPhys : public MIDI::Interface
{
public:
   MidiPhys(MIDI::Instrument& instrument)
      : MIDI::Interface(instrument)
   {}

   bool empty() const override { return uart.empty(); }

   uint8_t rx() override { return uart.rx(); }

   void tx(uint8_t byte) { return uart.tx(byte); }

private:
   MTL::Uart1_P26_P27 uart{31250, 8, MTL::UART::NONE, 1};
};

static MidiPhys midi_in {synth};

#endif


// --- USB MIDI ----------------------------------------------------------------

#if defined(HW_MIDI_USB_DEVICE)

// pico micro USB : MIDI in

#include "MTL/USBMidiDevice.h"

class MidiUSB : public MIDI::Interface
{
public:
   MidiUSB(MIDI::Instrument& instrument)
      : MIDI::Interface(instrument)
   {
      setDebug(true);
   }

   bool empty() const override { return device.empty(); }

   uint8_t rx() override { return device.rx(); }

   MTL::USBMidiDevice device{"https://github.com/AnotherJohnH",
                             0x9106, PLT_BCD_VERSION, "pico-106",
                             PLT_COMMIT};

   MTL::Usb usb{device};
};

static MidiUSB midi_usb {synth};

extern "C" void IRQ_USBCTRL() { midi_usb.usb.irq(); }

#endif


// --- Key press LED -----------------------------------------------------------

#if defined(HW_LED)

MTL::Digital::Out<MTL::PIN_LED1> led;

#endif


// -----------------------------------------------------------------------------

int MTL_main()
{
   // Clear screen and cursor to home
   printf("\e[2J");
   printf("\e[1,1H");

   printf("\n");
   printf("Program  : pico-106\n");
   printf("Author   : Copyright (c) 2024 John D. Haughton\n");
   printf("Version  : %s\n", PLT_VERSION);
   printf("Commit   : %s\n", PLT_COMMIT);
   printf("Built    : %s %s\n", __TIME__, __DATE__);
   printf("Compiler : %s\n", __VERSION__);
   printf("\n");

   while(true)
   {
#if defined(HW_MIDI_UART1)
      midi_in.tick();
#endif

#if defined(HW_MIDI_USB_DEVICE)
      midi_usb.tick();
#endif

#if defined(HW_LED)
      led = synth.isAnyVoiceOn();
#endif
   }

   return 0;
}
