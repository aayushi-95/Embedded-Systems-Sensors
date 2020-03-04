#include<16f1825.h>
#device PASS_STRINGS = IN_RAM
#use delay(INTERNAL = 8 MHz)
#fuses HS,NOWDT,NOPUT,NOBROWNOUT,NOPROTECT,INTRC_IO

#use rs232(baud=9600, rcv=PIN_C5, xmit=PIN_C4, parity=N,errors)

#define RELAY1 PIN_C1
#define RELAY2 PIN_C2
#define BUZZER PIN_C0
#define LED2 PIN_A5
#define LED1 PIN_A4

#define BUFFER_SIZE 50

int1 connect;
char serial_buffer[BUFFER_SIZE];
int buffer_index=0, index = 0;
char c, relay_num,relay_cmd,eop;
int1 found_cmd = false, off_relay = false;
int timer1_counter;
void process_commands();
#int_rda
void serial_isr()
{
   c = getc();
   //putc(c);
   if(c == 0x2B)
   {
      found_cmd = true;
      index = buffer_index;
   }
   if(c == '@')
      connect = true;
   serial_buffer[buffer_index++]=c;
   if(buffer_index ==  BUFFER_SIZE)
   {
      buffer_index = 0;
   }
}

#int_timer1
void timer1_isr()
{
   timer1_counter++;
   if(timer1_counter == 150)
   {
       off_relay = true;
       timer1_counter = 0;
   }
   set_timer1(15536);
}
void main()
{
  output_high(LED1);
  output_high(LED2);
  output_low(BUZZER);
  enable_interrupts(INT_RDA);
  enable_interrupts(GLOBAL);
  output_low(LED2);
  while(1)
  { 
    if(found_cmd)
    { 
      delay_ms(50);
      disable_interrupts(INT_RDA);
      printf("Found 2B");
      relay_num = serial_buffer[++index] - '0';
      relay_cmd = serial_buffer[++index] - '0';
      eop = serial_buffer[++index];
      printf("\nRelay cmd  %c, buzz cmd  %d  eop %x",relay_num,relay_cmd, eop);
      if(eop == 0x0D)
      {        
        printf("\nProcessing Commands");
        process_commands();
      }
      found_cmd = false;
      enable_interrupts(INT_RDA);
    }
    if(connect)
    {
      output_high(LED2);
      delay_ms(10);
      output_low(LED2);
      connect = false;
    }
    if(off_relay)
    {
      printf("\nTurning off relay 1");
      output_low(RELAY1);
      disable_interrupts(INT_TIMER1);
      off_relay = false;
    } 
  }
}
/*
1--- relay1 on relay2 off
2--- relay1 off relay2 on
3--- relay1 on relay 2 on
4--- relay1 off relay 2 off
6--- relay1 on 
7--- relay2 on
8--- relay1 off
9--- relay2 off
*/
void process_commands()
{
int16 relay_pin;
if(relay_num==1)
   relay_pin = RELAY1;
if(relay_num==2)
   relay_pin = RELAY2;
if(relay_cmd == 1)
{
   output_high(relay_pin);
   if(relay_pin==RELAY1)
   {
   setup_timer_1(T1_INTERNAL|T1_DIV_BY_8);
   set_timer1(15536);
   enable_interrupts(INT_TIMER1);
   }
}
if(relay_cmd == 0)
   output_low(relay_pin);
printf("@OK");
}
