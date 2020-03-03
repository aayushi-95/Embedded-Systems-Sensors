#include <18f26k20.h>
#device ADC=10
#FUSES NOWDT                   
#FUSES NOBROWNOUT             
#FUSES NOLVP   

#fuses HS,NOPROTECT
#use delay(internal=16000000)
#use i2c(Master,Fast=400000,sda=PIN_C4,scl=PIN_C3,force_sw) // maybe force_hw
#include <string.h>
#define OLED 0x78
#INCLUDE <stdlib.h>
#define LED PIN_A0

unsigned int16 adcValue;
float distance = 0;

const BYTE TEXT[51][5] =
{

   0x00, 0x00, 0x00, 0x00, 0x00, // SPACE
   0x00, 0x00, 0x5F, 0x00, 0x00, // !
   0x00, 0x03, 0x00, 0x03, 0x00, // "
   0x14, 0x3E, 0x14, 0x3E, 0x14, // #
   0x24, 0x2A, 0x7F, 0x2A, 0x12, // $
   0x43, 0x33, 0x08, 0x66, 0x61, // %
   0x36, 0x49, 0x55, 0x22, 0x50, //&
   0x00, 0x05, 0x03, 0x00, 0x00, // '
   0x00, 0x1C, 0x22, 0x41, 0x00, // (
   0x00, 0x41, 0x22, 0x1C, 0x00, //)
   0x14, 0x08, 0x3E, 0x08, 0x14, // *
   0x08, 0x08, 0x3E, 0x08, 0x08, // +
   0x00, 0x50, 0x30, 0x00, 0x00, //,
   0x08, 0x08, 0x08, 0x08, 0x08, // -
   0x00, 0x60, 0x60, 0x00, 0x00, // .
   0x20, 0x10, 0x08, 0x04, 0x02, // /
   0x3E, 0x51, 0x49, 0x45, 0x3E, // 0
   0x04, 0x02, 0x7F, 0x00, 0x00, // 1
   0x42, 0x61, 0x51, 0x49, 0x46, // 2
   0x22, 0x41, 0x49, 0x49, 0x36, // 3
   0x18, 0x14, 0x12, 0x7F, 0x10, // 4
   0x27, 0x45, 0x45, 0x45, 0x39, // 5
   0x3E, 0x49, 0x49, 0x49, 0x32, // 6
   0x01, 0x01, 0x71, 0x09, 0x07, // 7
   0x36, 0x49, 0x49, 0x49, 0x36, // 8
   0x26, 0x49, 0x49, 0x49, 0x3E, // 9
   0x00, 0x36, 0x36, 0x00, 0x00, // :
   0x00, 0x56, 0x36, 0x00, 0x00, //;

   0x08, 0x14, 0x22, 0x41, 0x00, // <
   0x14, 0x14, 0x14, 0x14, 0x14, // =
   0x00, 0x41, 0x22, 0x14, 0x08, // >
   0x02, 0x01, 0x51, 0x09, 0x06, // ?
   0x3E, 0x41, 0x59, 0x55, 0x5E, // @
   0x7E, 0x09, 0x09, 0x09, 0x7E, // A
   0x7F, 0x49, 0x49, 0x49, 0x36, // B
   0x3E, 0x41, 0x41, 0x41, 0x22, // C
   0x7F, 0x41, 0x41, 0x41, 0x3E, // D
   0x7F, 0x49, 0x49, 0x49, 0x41, // E
   0x7F, 0x09, 0x09, 0x09, 0x01, // F
   0x3E, 0x41, 0x41, 0x49, 0x3A, // G
   0x7F, 0x08, 0x08, 0x08, 0x7F, // H
   0x00, 0x41, 0x7F, 0x41, 0x00, // I
   0x30, 0x40, 0x40, 0x40, 0x3F, // J
   0x7F, 0x08, 0x14, 0x22, 0x41, // K
   0x7F, 0x40, 0x40, 0x40, 0x40, // L
   0x7F, 0x02, 0x0C, 0x02, 0x7F, // M
   0x7F, 0x02, 0x04, 0x08, 0x7F, // N
   0x3E, 0x41, 0x41, 0x41, 0x3E, // O
   0x7F, 0x09, 0x09, 0x09, 0x06, // P
   0x1E, 0x21, 0x21, 0x21, 0x5E, // Q
   0x7F, 0x09, 0x09, 0x09, 0x76
}; // R

const BYTE TEXT2[44][5]=
{
   0x26, 0x49, 0x49, 0x49, 0x32, // S
   0x01, 0x01, 0x7F, 0x01, 0x01, // T
   0x3F, 0x40, 0x40, 0x40, 0x3F, // U
   0x1F, 0x20, 0x40, 0x20, 0x1F, // V
   0x7F, 0x20, 0x10, 0x20, 0x7F, // W
   0x41, 0x22, 0x1C, 0x22, 0x41, // X
   0x07, 0x08, 0x70, 0x08, 0x07, // Y
   0x61, 0x51, 0x49, 0x45, 0x43, // Z
   0x00, 0x7F, 0x41, 0x00, 0x00, // [
   0x02, 0x04, 0x08, 0x10, 0x20, // \
   0x00, 0x00, 0x41, 0x7F, 0x00, // ]
   0x04, 0x02, 0x01, 0x02, 0x04, // ^
   0x40, 0x40, 0x40, 0x40, 0x40, // _
   0x00, 0x01, 0x02, 0x04, 0x00, // `
   0x20, 0x54, 0x54, 0x54, 0x78, // a
   0x7F, 0x44, 0x44, 0x44, 0x38, // b
   0x38, 0x44, 0x44, 0x44, 0x44, // c
   0x38, 0x44, 0x44, 0x44, 0x7F, // d
   0x38, 0x54, 0x54, 0x54, 0x18, // e
   0x04, 0x04, 0x7E, 0x05, 0x05, // f
   0x08, 0x54, 0x54, 0x54, 0x3C, // g
   0x7F, 0x08, 0x04, 0x04, 0x78, // h
   0x00, 0x44, 0x7D, 0x40, 0x00, // i
   0x20, 0x40, 0x44, 0x3D, 0x00, // j
   0x7F, 0x10, 0x28, 0x44, 0x00, // k
   0x00, 0x41, 0x7F, 0x40, 0x00, // l
   0x7C, 0x04, 0x78, 0x04, 0x78, // m
   0x7C, 0x08, 0x04, 0x04, 0x78, // n
   0x38, 0x44, 0x44, 0x44, 0x38, // o
   0x7C, 0x14, 0x14, 0x14, 0x08, // p
   0x08, 0x14, 0x14, 0x14, 0x7C, // q
   0x00, 0x7C, 0x08, 0x04, 0x04, // r
   0x48, 0x54, 0x54, 0x54, 0x20, // s
   0x04, 0x04, 0x3F, 0x44, 0x44, // t
   0x3C, 0x40, 0x40, 0x20, 0x7C, // u
   0x1C, 0x20, 0x40, 0x20, 0x1C, // v
   0x3C, 0x40, 0x30, 0x40, 0x3C, // w
   0x44, 0x28, 0x10, 0x28, 0x44, // x
   0x0C, 0x50, 0x50, 0x50, 0x3C, // y
   0x44, 0x64, 0x54, 0x4C, 0x44, // z
   0x00, 0x08, 0x36, 0x41, 0x41, //
   
      0x00, 0x00, 0x7F, 0x00, 0x00, //|
      0x41, 0x41, 0x36, 0x08, 0x00, //
   

   0x02, 0x01, 0x02, 0x04, 0x02
}; // ~

const INT LOGOS [1024] =
{
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xE0, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9C, 0xBE, 0xB6, 0xF6, 0xE6, 0x00, 0x00, 0xFE, 0xFE, 0x80,
   0x80, 0x80, 0xFE, 0xFE, 0x00, 0x00, 0xFE, 0xFE, 0x84, 0x86, 0x86, 0xFE, 0xFC, 0x00, 0x00, 0xFC,
   0xFE, 0x96, 0x96, 0x96, 0x9E, 0xDC, 0x00, 0x00, 0xFE, 0xFE, 0x0C, 0x06, 0x06, 0x00, 0x9C, 0xBE,
   0xB6, 0xF6, 0xE6, 0x00, 0x00, 0xFE, 0xFE, 0x80, 0x80, 0x80, 0xFE, 0xFE, 0x00, 0x00, 0xFE, 0xFE,
   0x84, 0x86, 0x86, 0xFE, 0xFC, 0x00, 0x00, 0xFC, 0xFE, 0x96, 0x96, 0x96, 0x9E, 0xDC, 0x00, 0x00,
   0xFE, 0xFE, 0x0C, 0x06, 0x06, 0x06, 0xFF, 0xFF, 0x06, 0x06, 0x00, 0xFE, 0xFE, 0x0C, 0x06, 0x06,
   0x00, 0xEC, 0xF6, 0x96, 0x96, 0xFE, 0xFC, 0x00, 0x00, 0xFE, 0xFE, 0x0C, 0x06, 0x06, 0xFE, 0xFC,
   0x00, 0x00, 0xC6, 0xE6, 0xB6, 0x9E, 0x8E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01,
   0x01, 0x00, 0x01, 0x01, 0x00, 0x00, 0x0F, 0x0F, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00,
   0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01,
   0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x01, 0x01, 0x00, 0x00, 0x0F, 0x0F,
   0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00,
   0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01,
   0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xE0, 0x30, 0xB0, 0xD8, 0xD8, 0xD8, 0xD8, 0x18,
   0x30, 0xF0, 0xC0, 0x00, 0x00, 0x80, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0x00, 0xC0, 0xC0, 0x80,
   0xC0, 0xC0, 0xC0, 0x80, 0x80, 0xC0, 0xC0, 0xC0, 0x80, 0x00, 0x00, 0x80, 0xC0, 0xC0, 0xC0, 0xC0,
   0x80, 0x00, 0x00, 0xD8, 0xD8, 0x00, 0x00, 0xFC, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x80, 0xC0, 0xC0, 0xC0, 0xC0, 0x80, 0x00, 0x80, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0x80, 0x00, 0x00,
   0xC0, 0xC0, 0x80, 0xC0, 0xC0, 0xC0, 0x80, 0x80, 0xC0, 0xC0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x7F, 0x60, 0xCF, 0xDF, 0xD8, 0xCC, 0xDF, 0xDB, 0x18,
   0x1C, 0x0F, 0x07, 0x00, 0x00, 0xDB, 0xFF, 0xB6, 0xB6, 0xB7, 0xF3, 0xE0, 0x00, 0x3F, 0x3F, 0x01,
   0x00, 0x00, 0x3F, 0x3F, 0x01, 0x00, 0x00, 0x3F, 0x3F, 0x00, 0x00, 0x1D, 0x3E, 0x32, 0x12, 0x3F,
   0x3F, 0x00, 0x00, 0x3F, 0x3F, 0x00, 0x00, 0x3F, 0x3F, 0x00, 0x00, 0x30, 0x30, 0x00, 0x00, 0x00,
   0x1F, 0x3F, 0x30, 0x30, 0x30, 0x19, 0x00, 0x1F, 0x3F, 0x30, 0x30, 0x30, 0x3F, 0x1F, 0x00, 0x00,
   0x3F, 0x3F, 0x01, 0x00, 0x00, 0x3F, 0x3F, 0x01, 0x00, 0x00, 0x3F, 0x3F, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x06, 0x06, 0x06, 0xFE, 0xFE, 0x06, 0x06, 0x06, 0x00, 0xF0, 0xF0, 0x60, 0x30, 0x30, 0x00,
   0xF6, 0xF6, 0x00, 0x00, 0xE0, 0xF0, 0xB0, 0xB0, 0xB0, 0xF0, 0xE0, 0x00, 0x00, 0xE0, 0xF0, 0xB0,
   0xB0, 0x30, 0x00, 0x30, 0xFC, 0xFC, 0x30, 0x30, 0x00, 0x00, 0xE0, 0xF0, 0xB0, 0xB0, 0xB0, 0xF0,
   0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF6, 0xF6, 0x00, 0x30, 0xFC,
   0xFC, 0x30, 0x30, 0x00, 0x00, 0x60, 0xB0, 0xB0, 0xB0, 0xF0, 0xE0, 0x00, 0x00, 0xFF, 0xFF, 0x00,
   0x30, 0xF0, 0xC0, 0x00, 0x00, 0xC0, 0xF0, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x00, 0x00, 0x00, 0x00,
   0x0F, 0x0F, 0x00, 0x00, 0x07, 0x0F, 0x0C, 0x0C, 0x0C, 0x0C, 0x06, 0x00, 0x00, 0x0C, 0x0D, 0x0D,
   0x0F, 0x07, 0x00, 0x00, 0x07, 0x0F, 0x0C, 0x0C, 0x00, 0x00, 0x07, 0x0F, 0x0C, 0x0C, 0x0C, 0x0C,
   0x06, 0x00, 0x20, 0x3C, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x00, 0x00, 0x07,
   0x0F, 0x0C, 0x0C, 0x00, 0x00, 0x07, 0x0F, 0x0C, 0x04, 0x0F, 0x0F, 0x00, 0x00, 0x0F, 0x0F, 0x00,
   0x00, 0x01, 0x67, 0x7E, 0x1E, 0x07, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

int DISPLAY [1024];

void OLED_command(INT ind, int com)
{
   i2c_start ();
   i2c_write (ind);
   i2c_write (0x00);
   i2c_write (com);
   i2c_stop ();
}

void OLED_write(INT ind)
{
   LONG i;
   OLED_command (ind, 0x21) ;
   OLED_command (ind, 0x00);
   OLED_command (ind, 127);
   OLED_command (ind, 0x22) ;
   OLED_command (ind, 0x00);
   OLED_command (ind, 7);
   i2c_start ();
   i2c_write (ind) ;
   i2c_write (0x40) ;
   FOR (i = 0; i < 1024; i++)
   {
     
      i2c_write (DISPLAY[i]) ;
   }

   i2c_stop ();
}

void  OLED_init(INT add)
{
   OLED_command (add, 0xae) ;
   OLED_command (add, 0xa8);
   OLED_command (add, 0x3f);
   OLED_command (add, 0xd3);
   OLED_command (add, 0x00);
   OLED_command (add, 0x40);
   
   OLED_command (add, 0xa0);
   OLED_command (add, 0xa1);
   OLED_command (add, 0xc0);
   
   OLED_command (add, 0xc8);
   OLED_command (add, 0xda);
   OLED_command (add, 0x12);
   OLED_command (add, 0x81);
   OLED_command (add, 0xfF);
   OLED_command (add, 0xa4);
   OLED_command (add, 0xa6) ;
   OLED_command (add, 0xd5);
   OLED_command (add, 0x80);
   OLED_command (add, 0x8d);
   OLED_command (add, 0x14) ;
   OLED_command (add, 0xAF) ;
   OLED_command (add, 0x20) ;
   OLED_command (add, 0x00) ;
}

void OLED_pixel(LONG x,long y)
{
   LONG nt;
   LONG pagina;
   LONG bit;
   pagina = y /8;
   bit= y-(pagina*8);
   nt= DISPLAY[pagina*128+x];
   nt |= 1 << bit;
   DISPLAY[pagina*128+x] = nt;
}

   VOID OLED_line (int x1, int y1, int x2, int y2)
{
      SIGNED int x, y, addx, addy, dx, dy;
      SIGNED long P;
      INT i;
      dx = abs ( (SIGNED int) (x2 - x1));
      dy = abs ( (SIGNED int) (y2 - y1));
      x = x1;
      y = y1;

      IF (x1 > x2)
         addx = -1;

      ELSE
      addx = 1;

      IF (y1 > y2)
         addy = -1;

      ELSE
      addy = 1;

      IF (dx >= dy)
      {
         P = 2 * dy - dx;
         FOR (i = 0; i <= dx;  ++i)
         {
            OLED_pixel (x, y);

            IF (P < 0)
            {
               P += 2*dy;
               x += addx;
            }

            ELSE
            {
               P += 2*dy - 2 * dx;
               x += addx;
               y += addy;
            }
         }
      }

      ELSE
      {
         P = 2 * dx - dy;
         FOR (i = 0; i <= dy;  ++i)
         {
            OLED_pixel (x, y);

            IF (P < 0)
            {
               P += 2*dx;
               y += addy;
            }

            ELSE
            {
               P += 2*dx - 2 * dy;
               x += addx;
               y += addy;
            }
         }
      }
}

   VOID OLED_circle(int x, int y, int radius, int1 riemp)
   {
      SIGNED int a, b, P;
      a = 0;
      b = radius;
      P = 1 - radius;

      DO
      {
         IF (riemp)
         {
            OLED_line (x - a, y+b, x + a, y + b);
            OLED_line (x - a, y-b, x + a, y - b);
            OLED_line (x - b, y+a, x + b, y + a);
            OLED_line (x - b, y-a, x + b, y - a);
         }

         ELSE
         {
            OLED_pixel (a + x, b+y);
            OLED_pixel (b + x, a+y);
            OLED_pixel (x - a, b+y);
            OLED_pixel (x - b, a+y);
            OLED_pixel (b + x, y-a);
            OLED_pixel (a + x, y-b);
            OLED_pixel (x - a, y-b);
            OLED_pixel (x - b, y-a);
         }

         IF (P < 0)
            P += 3 + 2 * a++;

         ELSE
         P += 5 + 2 * (a++ - b--);
      } WHILE (a <= b);
   }

   VOID OLED_text(int x, int y, char* textptr, int size)
   {
      INT i, j, k, l, m;     // Loop counters
      BYTE pixelData[5];     // Stores character data
      FOR (i = 0; textptr[i] != '\0'; ++i, ++x) // Loop through the passed string
      {
         IF (textptr[i] < 'S') // Checks if the letter is in the first text array
            memcpy (pixelData, TEXT[textptr[i] - ' '], 5) ;

         else IF (textptr[i] <= '~') // Check if the letter is in the second array
            memcpy (pixelData, TEXT2[textptr[i] - 'S'], 5) ;

         ELSE
         memcpy (pixelData, TEXT[0], 5); // DEFAULT to space
         IF (x + 5 * size >= 128)  // Performs character wrapping
         {
            x = 0;       // Set x at far left position
            y += 7*size + 1;    // Set y at next position down
         }

         FOR (j = 0; j<5; ++j, x += size)  // Loop through character byte data
         {
            FOR (k = 0; k<7 * size; ++k)  // Loop through the vertical pixels
            {
               IF (bit_test (pixelData[j], k)) // Check if the pixel should be set
               {
                  FOR (l = 0; l<size; ++l) // The next two loops change the
                  {
                     // character's size
                     FOR (m = 0; m<size; ++m)
                     {
                        OLED_pixel (x + m, y+k * size + l); // Draws the pixel
                     }
                  }
               }
            }
         }
      }
   }

   VOID OLED_rect(int x1, int y1, int x2, int y2, int riemp)
   {
      IF (riemp)
      {
         INT y, ymax;      // Find the y min and max
         IF (y1 < y2)
         {
            y = y1;
            ymax = y2;
         }

         ELSE
         {
            y = y2;
            ymax = y1;
         }

         FOR (; y <= ymax; ++y)     // Draw lines to fill the rectangle
            OLED_line (x1, y, x2, y);
      }

      ELSE
      {
         OLED_line (x1, y1, x2, y1); // Draw the 4 sides
         OLED_line (x1, y2, x2, y2);
         OLED_line (x1, y1, x1, y2);
         OLED_line (x2, y1, x2, y2);
      }
   }

   VOID logo()
   {
      LONG x;
      INT c;
      FOR (x = 0; x < 1024; x++)
      {
         c= LOGOS[x];
         DISPLAY[x] = c;
      }
   }

   VOID OLED_clear()
   {
      LONG x;
      FOR (x = 0; x < 1024; x++)
      {
         DISPLAY[x] = 0x00;
      }
   }

   VOID main()
   {
      CHAR txt[100];
      OLED_init(OLED);
      
        setup_adc(ADC_CLOCK_DIV_8);      // Set ADC conversion time to 8Tosc
        setup_adc_ports(sAN8);             // Configure RB2 (AN8) as analog
        set_adc_channel(8);
        
        OLED_clear();
        sprintf(txt,"   Ready       to       Measure");
        OLED_text(0,0,txt,2);
        OLED_write(OLED);
        delay_ms(2000); 
      WHILE (1)
      {
         
         //OLED_clear();
         //logo();
         //OLED_write(OLED);
         //delay_ms(3000);
         //OLED_clear();
        // sprintf(txt,"Distance is%f",distance);
        // OLED_text(0,0,txt,2);
         //OLED_circle(15,40,15,0);
         //OLED_rect(35,25,65,55,1);
         //OLED_circle(80,40,15,1);
         //OLED_rect(90,25,120,55,0);
         //OLED_write(OLED);
        // delay_ms(3000);
         
    output_high(LED);
    adcValue = read_adc();
    //Range 10 to 15 cm
    if(adcValue >= 510 && adcValue <= 730)
      {
         distance = (10 +((730 - adcValue)/44));
         OLED_clear();
         sprintf(txt,"Distance is%f",distance);
         OLED_text(0,0,txt,2);
      }
   //Range 15 to 20 cm
    else if(adcValue >= 400 && adcValue < 510)
      {
         distance = (15 +((510 - adcValue)/22));
         
      }
   //Range 20 to 30 cm
   else if(adcValue >= 286 && adcValue < 400)
      {
         distance = (20 +((400 - adcValue)/11.4));
         
      }
   //Range 30 to 40 cm
   else if(adcValue >= 220 && adcValue < 286)
      {
         distance = (30 +((286 - adcValue)/6.6));
         
      }
   //Range 40 to 50 cm
   else if(adcValue >= 175 && adcValue < 220)
      {
         distance = (40 +((220 - adcValue)/4.5));
        
      }
   //Range 50 to 60 cm
   else if(adcValue >= 160 && adcValue < 175)
      {
         distance = (50 +((175 - adcValue)/1.5));
         
      }
   //Range 60 to 70 cm
   else if(adcValue >= 144 && adcValue < 160)
      {
         distance = (60 +((160 - adcValue)/1.6));
         
      }
   //Range 70 to 80 cm
   else if(adcValue >= 120 && adcValue < 144)
      {
         distance = (70 +((144 - adcValue)/2.4));
         
      }
   else{ 
         OLED_clear();
         sprintf(txt,"Object is not in range\n");
         OLED_text(0,0,txt,2);
         OLED_write(OLED);
         delay_ms(2000);
       }
    //printf("Voltage :%lu \n",adcValue);
         OLED_clear();
         sprintf(txt,"Distance is%f",distance);
         OLED_text(0,0,txt,2);
         OLED_write(OLED);
         delay_ms(1000);
         output_low(LED);
         delay_ms(500);
      }
   } 
