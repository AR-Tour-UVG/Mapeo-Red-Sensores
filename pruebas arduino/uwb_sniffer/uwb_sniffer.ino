#include <Wire.h>
#include <U8g2lib.h>
#include <DW3000.h>

#define LED_PIN 2
#define PIN_RST 27
#define PIN_IRQ 34
#define PIN_SS 5

U8G2_SH1106_128X64_NONAME_F_SW_I2C oled(U8G2_R0, /* clock=*/9, /* data=*/8, /* reset=*/U8X8_PIN_NONE);
DW3000Class DW;

// Logo UVG (64x64 px aprox)
const unsigned char uvg_logo [] PROGMEM = {
  0xff, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xff, 
	0xff, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xff, 
	0xff, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xff, 
	0xff, 0x1f, 0x00, 0x00, 0x38, 0x00, 0x9c, 0x07, 0x80, 0x07, 0xfc, 0x03, 0x00, 0x00, 0xf8, 0xff, 
	0xff, 0x1f, 0x00, 0x00, 0x7c, 0x00, 0xfe, 0x07, 0xc0, 0x07, 0xff, 0x0f, 0x00, 0x00, 0xf8, 0xff, 
	0xff, 0x1f, 0x00, 0x00, 0x7c, 0x00, 0x9e, 0x0f, 0xc0, 0xc7, 0xff, 0x1f, 0x00, 0x00, 0xf8, 0xff, 
	0xff, 0x1f, 0x00, 0x00, 0x7c, 0x00, 0x9e, 0x0f, 0xc0, 0xe3, 0xff, 0x1f, 0x00, 0x00, 0xf8, 0xff, 
	0xff, 0x1f, 0x00, 0x00, 0x7c, 0x00, 0x1e, 0x0f, 0xe0, 0xf3, 0x0f, 0x1f, 0x00, 0x00, 0xf8, 0xff, 
	0xff, 0x1f, 0x00, 0x00, 0x7c, 0x00, 0x1e, 0x1f, 0xe0, 0xf3, 0x03, 0x08, 0x00, 0x00, 0xf8, 0xff, 
	0xff, 0x1f, 0x00, 0x00, 0x7c, 0x00, 0x1e, 0x1f, 0xe0, 0xf9, 0x01, 0x00, 0x00, 0x00, 0xf8, 0xff, 
	0xff, 0x1f, 0x00, 0x00, 0x7c, 0x00, 0x1e, 0x1e, 0xf0, 0xf9, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xff, 
	0xff, 0x1f, 0x00, 0x00, 0x7c, 0x00, 0x1e, 0x3e, 0xf0, 0x7c, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xff, 
	0xff, 0x1f, 0x00, 0x00, 0x7c, 0x00, 0x1e, 0x3e, 0xf0, 0x7c, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xff, 
	0xff, 0x1f, 0x00, 0x00, 0x7c, 0x00, 0x1e, 0x3c, 0xf8, 0x7c, 0xc0, 0x1f, 0x00, 0x00, 0xf8, 0xff, 
	0xff, 0x1f, 0x00, 0x00, 0x7c, 0x00, 0x1e, 0x7c, 0xf8, 0x7c, 0xc0, 0x1f, 0x00, 0x00, 0xf8, 0xff, 
	0xff, 0x1f, 0x00, 0x00, 0x7c, 0x00, 0x1e, 0x7c, 0x7c, 0x7c, 0xc0, 0x1f, 0x00, 0x00, 0xf8, 0xff, 
	0xff, 0x1f, 0x00, 0x00, 0x7c, 0x00, 0x1e, 0x78, 0x7c, 0x7c, 0xc0, 0x1f, 0x00, 0x00, 0xf8, 0xff, 
	0xff, 0x1f, 0x00, 0x00, 0x7c, 0x00, 0x1e, 0xf8, 0x3c, 0x7c, 0x00, 0x1e, 0x00, 0x00, 0xf8, 0xff, 
	0xff, 0x1f, 0x00, 0x00, 0x7c, 0x00, 0x1e, 0xf0, 0x3c, 0x7c, 0x00, 0x1e, 0x00, 0x00, 0xf8, 0xff, 
	0xff, 0x1f, 0x00, 0x00, 0x7c, 0x00, 0x1f, 0xf0, 0x1f, 0xf8, 0x00, 0x1e, 0x00, 0x00, 0xf8, 0xff, 
	0xff, 0x1f, 0x00, 0x00, 0xf8, 0x00, 0x1f, 0xf0, 0x1f, 0xf8, 0x01, 0x1e, 0x00, 0x00, 0xf8, 0xff, 
	0xff, 0x1f, 0x00, 0x00, 0xf8, 0x81, 0x1f, 0xe0, 0x1f, 0xf0, 0x03, 0x1e, 0x00, 0x00, 0xf8, 0xff, 
	0xff, 0x1f, 0x00, 0x00, 0xf0, 0xff, 0x0f, 0xe0, 0x1f, 0xf0, 0xff, 0x1f, 0x00, 0x00, 0xf8, 0xff, 
	0xff, 0x1f, 0x00, 0x00, 0xf0, 0xff, 0x07, 0xe0, 0x0f, 0xe0, 0xff, 0x1f, 0x00, 0x00, 0xf8, 0xff, 
	0xff, 0x1f, 0x00, 0x00, 0xe0, 0xff, 0x07, 0xc0, 0x0f, 0xc0, 0xff, 0x1f, 0x00, 0x00, 0xf8, 0xff, 
	0xff, 0x1f, 0x00, 0x00, 0x80, 0xff, 0x01, 0xc0, 0x07, 0x00, 0xff, 0x07, 0x00, 0x00, 0xf8, 0xff, 
	0xff, 0x1f, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0xf8, 0xff, 
	0xff, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xff, 
	0xff, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xff, 
	0xff, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xff, 
	0xff, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xff, 
	0xff, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xff, 
	0xff, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xff, 
	0xff, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xff, 
	0xff, 0x1f, 0x00, 0x00, 0x04, 0x02, 0x00, 0xcf, 0x71, 0x78, 0x00, 0x0f, 0x00, 0x00, 0xf8, 0xff, 
	0xff, 0x1f, 0x00, 0x00, 0x84, 0x46, 0x45, 0x01, 0x08, 0x8a, 0x18, 0x11, 0x00, 0x00, 0xf8, 0xff, 
	0xff, 0x1f, 0x00, 0x00, 0x84, 0x4a, 0x45, 0x01, 0x18, 0x0a, 0x01, 0x21, 0x00, 0x00, 0xf8, 0xff, 
	0xff, 0x1f, 0x00, 0x00, 0x84, 0x62, 0x29, 0x4f, 0x21, 0x0a, 0x25, 0x21, 0x00, 0x00, 0xf8, 0xff, 
	0xff, 0x1f, 0x00, 0x00, 0x84, 0x62, 0x29, 0x01, 0x81, 0x0a, 0x3d, 0x21, 0x00, 0x00, 0xf8, 0xff, 
	0xff, 0x1f, 0x00, 0x00, 0x00, 0x22, 0x11, 0x01, 0x80, 0x8a, 0xc0, 0x11, 0x00, 0x00, 0xf8, 0xff, 
	0xff, 0x1f, 0x00, 0x00, 0x78, 0x20, 0x00, 0x00, 0x78, 0x68, 0x00, 0x0d, 0x00, 0x00, 0xf8, 0xff, 
	0xff, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xff, 
	0xff, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xff, 
	0xff, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xff, 
	0xff, 0x1f, 0x00, 0x00, 0x8c, 0x18, 0x04, 0x08, 0x60, 0x08, 0x00, 0x01, 0x00, 0x00, 0xf8, 0xff, 
	0xff, 0x1f, 0x00, 0x00, 0x04, 0x09, 0x04, 0x10, 0x62, 0x08, 0x00, 0x01, 0x00, 0x00, 0xf8, 0xff, 
	0xff, 0x1f, 0x00, 0x00, 0x04, 0x0a, 0x04, 0x10, 0x80, 0x08, 0x00, 0x01, 0x00, 0x00, 0xf8, 0xff, 
	0xff, 0x1f, 0x00, 0x00, 0x04, 0xfa, 0x04, 0x10, 0x91, 0x08, 0x00, 0x1f, 0x00, 0x00, 0xf8, 0xff, 
	0xff, 0x1f, 0x00, 0x00, 0x04, 0x1a, 0x04, 0x00, 0x91, 0x08, 0x00, 0x01, 0x00, 0x00, 0xf8, 0xff, 
	0xff, 0x1f, 0x00, 0x00, 0x04, 0x0a, 0x04, 0xa0, 0x98, 0x09, 0x00, 0x01, 0x00, 0x00, 0xf8, 0xff, 
	0xff, 0x1f, 0x00, 0x00, 0x04, 0x09, 0x04, 0xc0, 0x08, 0x09, 0x00, 0x01, 0x00, 0x00, 0xf8, 0xff, 
	0xff, 0x1f, 0x00, 0x00, 0xcc, 0x18, 0x04, 0x40, 0x04, 0x0a, 0x04, 0x01, 0x00, 0x00, 0xf8, 0xff, 
	0xff, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xff, 
	0xff, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xff, 
	0xff, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xff, 
	0xff, 0x1f, 0x00, 0x00, 0x3c, 0x0f, 0x07, 0xd0, 0x77, 0x21, 0x02, 0x08, 0x00, 0x00, 0xf8, 0xff, 
	0xff, 0x1f, 0x00, 0x00, 0x44, 0x00, 0x00, 0x18, 0x11, 0x23, 0x02, 0x08, 0x00, 0x00, 0xf8, 0xff, 
	0xff, 0x1f, 0x00, 0x00, 0x04, 0x01, 0x00, 0x08, 0x11, 0x23, 0x01, 0x10, 0x00, 0x00, 0xf8, 0xff, 
	0xff, 0x1f, 0x00, 0x00, 0x04, 0x40, 0x00, 0x3c, 0x11, 0x13, 0x07, 0x1c, 0x00, 0x00, 0xf8, 0xff, 
	0xff, 0x1f, 0x00, 0x00, 0x44, 0x80, 0x00, 0x05, 0x01, 0x8d, 0x08, 0x02, 0x00, 0x00, 0xf8, 0xff, 
	0xff, 0x1f, 0x00, 0x00, 0x24, 0x00, 0x24, 0x01, 0x10, 0x09, 0x08, 0x02, 0x00, 0x00, 0xf8, 0xff, 
	0xff, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xff, 
	0xff, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xff, 
	0xff, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xff
};

const unsigned char jack [] PROGMEM = {
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xcf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x87, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x07, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0xc3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0xe3, 0xf9, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x9f, 0x71, 0xf8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x9f, 0x19, 0xf8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xbf, 0x0c, 0xf8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xbf, 0xf2, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xbf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xf3, 0x7f, 0x17, 0xfe, 0x0f, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xf3, 0x7f, 0x02, 0xfc, 0x07, 0xd8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xe1, 0xbf, 0x01, 0x00, 0x03, 0xc0, 0xf7, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xd1, 0x3f, 0x00, 0xf0, 0x01, 0xc8, 0xf7, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0x31, 0x3f, 0x00, 0xe0, 0xf3, 0xcf, 0xf7, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0x73, 0x00, 0x00, 0xc0, 0x79, 0xc7, 0xf3, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xe3, 0xbf, 0x3f, 0x00, 0x00, 0xc0, 0xf3, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xc3, 0x7f, 0x02, 0x00, 0x70, 0xe0, 0xf3, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xbf, 0x83, 0x3f, 0xe0, 0x0f, 0x7c, 0x00, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0x7f, 0x06, 0x00, 0xf8, 0xdf, 0x64, 0x60, 0xf8, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0x0c, 0x00, 0xf0, 0xdc, 0x2c, 0x20, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0x11, 0x00, 0xf0, 0xdc, 0x3c, 0x90, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0x23, 0x0e, 0xe0, 0xdf, 0x01, 0x88, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xd3, 0x39, 0xc0, 0xc1, 0x03, 0x8c, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0x63, 0xe0, 0x00, 0xc0, 0x07, 0xc6, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xc3, 0x9f, 0x03, 0xa8, 0xef, 0xe3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0x87, 0x3f, 0x0f, 0x00, 0x07, 0xc3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0x07, 0x3f, 0x3c, 0x00, 0x87, 0xc1, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0x0f, 0x1c, 0xf0, 0x00, 0x80, 0xe0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0x3f, 0x00, 0xc0, 0x03, 0x00, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0x7f, 0x00, 0x80, 0x03, 0x00, 0x80, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0x0f, 0x1e, 0x00, 0x40, 0x20, 0x80, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0x1f, 0xff, 0x00, 0x50, 0x20, 0x00, 0xe0, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0x3f, 0x00, 0x00, 0x48, 0x00, 0xc0, 0xcf, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0x7f, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xbf, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0x3c, 0x00, 0x00, 0x00, 0xfb, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0x1e, 0x00, 0x00, 0x00, 0x02, 0xfe, 0xfe, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0x03, 0x02, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xfd, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0x7c, 0x00, 0x00, 0xf8, 0x03, 0x00, 0xc0, 0xf9, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0x3f, 0xff, 0x8f, 0x01, 0xbc, 0x0b, 0x00, 0x80, 0xfb, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0x9f, 0xff, 0x1f, 0x01, 0x70, 0x0f, 0x00, 0x00, 0xf7, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xcf, 0xdf, 0x03, 0x19, 0x50, 0x0f, 0xc0, 0x01, 0xf6, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xef, 0x03, 0x00, 0x08, 0x60, 0x00, 0xe0, 0x03, 0x00, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xf3, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x07, 0x00, 0xfc, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0x39, 0x00, 0x00, 0x00, 0x80, 0x00, 0xfc, 0x07, 0x80, 0xf9, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0x1f, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x03, 0x00, 0xfb, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xc7, 0x03, 0xf0, 0x00, 0x0c, 0x03, 0x00, 0xff, 0x1b, 0x00, 0xf6, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xf7, 0x00, 0xf8, 0x03, 0x0c, 0x07, 0x80, 0xff, 0x09, 0xd8, 0xf4, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0x33, 0x0c, 0xf8, 0x0f, 0x8c, 0x3f, 0x9e, 0xff, 0xcd, 0x99, 0xf4, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0x03, 0x07, 0xc0, 0x1f, 0x0c, 0xe0, 0x9f, 0xff, 0xc3, 0x99, 0xf0, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0x83, 0x03, 0x98, 0x7f, 0x9c, 0xff, 0x9f, 0xff, 0xdb, 0xd9, 0xf0, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0x83, 0x01, 0xb0, 0xff, 0x9c, 0xff, 0x9f, 0xff, 0xd3, 0xcc, 0xfc, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0x8b, 0x00, 0xa0, 0xff, 0x1d, 0xff, 0xbf, 0xff, 0x47, 0x47, 0xfd, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0x93, 0x88, 0x83, 0xff, 0x1d, 0xff, 0xbf, 0xff, 0x3f, 0x30, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0x17, 0x98, 0xc3, 0xff, 0x1d, 0xff, 0x3f, 0xff, 0xff, 0xb3, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0x0f, 0x98, 0xdb, 0xff, 0x3d, 0xff, 0x77, 0xff, 0xff, 0xe3, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0x5f, 0x13, 0xe2, 0xff, 0x3b, 0xfe, 0x7c, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0x82, 0xf8, 0xff, 0x3b, 0x1c, 0x7f, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0x19, 0xfe, 0xff, 0x3b, 0xfe, 0xff, 0xfe, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x73, 0xfc, 0xff, 0x3c, 0x30, 0xfc, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x77, 0xf8, 0xff, 0x8c, 0xff, 0xf1, 0xff, 0xff, 0xff, 0xff
};

void showWelcomeMessage() {
  oled.clearBuffer();
  oled.drawXBMP(0, 0, 128, 64, uvg_logo);  // Mostrar logo UVG
  oled.sendBuffer();
  delay(2000);

  oled.clearBuffer();
  oled.drawXBMP(0, 0, 128, 64, jack);  // Mostrar logo Jack
  oled.sendBuffer();
  delay(2000);

  oled.clearBuffer();
  oled.setFont(u8g2_font_6x10_tr);
  oled.drawStr(0, 20, "Bienvenido al Sniffer");
  oled.drawStr(0, 35, "AR UVG TOUR");
  oled.drawStr(0, 50, "UWB Packet Listener");
  oled.sendBuffer();
  delay(2500);
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.begin(115200);
  delay(1000);

  Wire.begin(8, 9);
  oled.begin();
  oled.setFont(u8g2_font_6x10_tr);

  showWelcomeMessage();

  if (!DW.begin(PIN_SS, PIN_IRQ, PIN_RST)) {
    oled.clearBuffer();
    oled.drawStr(0, 30, "Error al iniciar DW3000");
    oled.sendBuffer();
    while (1);
  }

  DW.setDeviceAddress(1);
  DW.setNetworkId(10);
  DW.setAntennaDelay(16436);
  DW.newReceive();
  DW.receivePermanently(true);  // Sniffer mode

  oled.clearBuffer();
  oled.drawStr(0, 20, "Sniffer UWB Activo");
  oled.drawStr(0, 40, "Esperando paquetes...");
  oled.sendBuffer();
}

void loop() {
  if (DW.isReceiveDone()) {
    int len = DW.getDataLength();
    byte data[len];
    DW.getData(data, len);

    digitalWrite(LED_PIN, HIGH);
    delay(50);
    digitalWrite(LED_PIN, LOW);

    Serial.print("Paquete UWB recibido: ");
    Serial.print(len);
    Serial.println(" bytes");
    Serial.print("Contenido (hex): ");
    for (int i = 0; i < len; i++) {
      Serial.print(data[i], HEX);
      Serial.print(" ");
    }
    Serial.println();

    // Mostrar primeros bytes en OLED
    oled.clearBuffer();
    oled.setFont(u8g2_font_5x7_tr);
    oled.drawStr(0, 10, "Paquete UWB capturado");
    oled.drawStr(0, 25, ("Tam: " + String(len) + " bytes").c_str());
    oled.drawStr(0, 40, "Bytes:");

    for (int i = 0; i < min(len, 8); i++) {
      char hexChar[4];
      sprintf(hexChar, "%02X", data[i]);
      oled.drawStr(5 + (i * 14), 55, hexChar);
    }

    oled.sendBuffer();

    DW.newReceive();  // Listo para el siguiente
  }
}