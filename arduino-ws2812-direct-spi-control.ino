#include<SPI.h>

/**
MCU pin D11 MOSI = LED STRIP DATA pin
Tested on Arduino Nano

8Mhz SPI_CLOCK_DIV2
one 0xF8
zero 0xC0

4Mhz SPI_CLOCK_DIV4
one 1 or 4
zero 2 or 6

you can use hex dec binary converter online
one
0b1 1
0b10 2
0b11 3
0b100 4
0b110 6
0b1000 8
0b1110 14
0b11000000 192 0xC0
0b11111000 248 0xF8
**/

#define number_of_LEDs                          9
#define number_of_bits                          24


#define one                                     0xF8
#define zero                                    0xC0

void setup (void) {
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE1);
  SPI.setClockDivider(SPI_CLOCK_DIV2);    //Sets clock for SPI communication at 2 (16/2=8Mhz)
  delay(10);
  WS2812_reset();
}

void loop(void) {
  //All RED
  WS2812_set_common_colour_on_all_LEDs(200, 0, 0);
  delay(600);

  //All GREEN
  WS2812_set_common_colour_on_all_LEDs(0, 200, 0);
  delay(600);

  //All BLUE
  WS2812_set_common_colour_on_all_LEDs(0, 0, 200);
  delay(600);
}

void WS2812_send_data(unsigned char r, unsigned char g, unsigned char b) {
  unsigned char s = number_of_bits;

  unsigned long value = 0x00000000;

  value = (((unsigned long)g << 16) | ((unsigned long)r << 8) | ((unsigned long)b));

  while (s > 0) {
    if ((value & 0x800000) != LOW) {
      SPI.transfer(one);
      asm("nop");
      asm("nop");
    } else {
      SPI.transfer(zero);
    }
    value <<= 1;
    s--;
  }
}

void WS2812_set_common_colour_on_all_LEDs(unsigned char r, unsigned char g, unsigned char b) {
  unsigned int n = 0x0000;

  for (n = 0; n < number_of_LEDs; n++) {
    WS2812_send_data(r, g, b);
  }

  WS2812_update();
}

void WS2812_update() {
  delayMicroseconds(60);
}

void WS2812_reset() {
  WS2812_set_common_colour_on_all_LEDs(0, 0, 0);
}
