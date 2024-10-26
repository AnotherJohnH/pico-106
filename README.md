# pico106

![pico106](docs/pico106.png)

Hybrid synthesizer inspired by the Roland Juno-106.

DCOs driven by a Raspberry Pi Pico with a USB MIDI interface.

## Status

Usable as a musical instrument but plenty to do...

## Hardware

[schematic](https://github.com/AnotherJohnH/pico106/blob/main/docs/schematic_v0.01.pdf)

## Software

### Dependencies

+ https://github.com/AnotherJohnH/Platform
+ arm-none-eabi-gcc
+ cmake
+ ninja (make works too)

### Checkout

This repo uses git sub-modules, so checkout using --recurse to clone all the
dependent source...

    git clone --recurse https://github.com/AnotherJohnH/pico106.git

or

    git clone --recurse ssh://git@github.com/AnotherJohnH/pico106.git

### Build

Being developed on MacOS but should build on Linux too.

    make

Build directly using cmake...

    mkdir build
    cd build
    cmake -DCMAKE_BUILD_TYPE=Release -DPLT_TARGET=rpipico2 -DCMAKE_TOOLCHAIN_FILE=Platform/MTL/rpipico/toolchain.cmake ..
    make

Flashable image will be found under the build sub directory here...

    .../Source/pico106.uf2

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details
