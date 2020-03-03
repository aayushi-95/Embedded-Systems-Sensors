#include <16Lf1825.h>
#use delay (internal = 8MHz)
#fuses HS,NOWDT,NOPROTECT,NOBROWNOUT,NOPUT

//LCD Module Connections
#define LCD_RS_PIN PIN_A0
#define LCD_RW_PIN PIN_A1
#define LCD_ENABLE_PIN PIN_C1
#define LCD_DATA4 PIN_C2
#define LCD_DATA5 PIN_C3
#define LCD_DATA6 PIN_C4
#define LCD_DATA7 PIN_C5
//End LCD Module Connections

//#include <main.h>
#include <lcd.c>

// #use delay (clock=8000000)

void main()
{
 int i=0;

  lcd_init();
  Delay_ms(100);

  while(TRUE)
  {
   lcd_putc('\f');                   //Clear Display
    lcd_putc("Hello World");
    Delay_ms(2000);
    lcd_putc('\f');                   //Clear Display
    lcd_putc("Welcome To");
    lcd_gotoxy(1,2);
    lcd_putc("LCD Library");
    Delay_ms(2000);
    lcd_putc('\f');                   //Clear Display
    printf(lcd_putc,"Count = %d", i); //Display Count
    Delay_ms(2000);
    i++;
  }
}
