#include <18f26k20.h>
#use delay (internal=8MHz)
#fuses INTRC_IO, HS,NOWDT,NOBROWNOUT,NOPROTECT,NOPUT
#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7, parity=N, errors)
#define LED PIN_A0

#define s0 PIN_B0 
#define s1 PIN_B1
#define s2 PIN_B2 
#define s3 PIN_B7 
#define VOUT PIN_C2

int8 capture_rising_edge; 
int8 got_pulse_width; 
int16  ccp_delta;
//float32 red=0, blue=0, green=0;


#int_ccp1 
void ccp1_isr(void) 
{ 
static int16 t1;


if(capture_rising_edge) 
  { 
   setup_ccp1(CCP_CAPTURE_FE); 
   capture_rising_edge = FALSE; 
   t1 = CCP_1;
  } 
else 
  { 
   setup_ccp1(CCP_CAPTURE_RE); 
   capture_rising_edge = TRUE; 
   ccp_delta = CCP_1 - t1; 
   got_pulse_width = TRUE; 
  } 

} 

void main()
{

output_high(VOUT);
output_high(s0);
output_low(s1);

int16 pulse_width;
int16 local_ccp_delta;
int16 i;
got_pulse_width = FALSE; 
capture_rising_edge = TRUE; 

setup_ccp1(CCP_CAPTURE_RE);
setup_timer_1(T1_INTERNAL); 
clear_interrupt(INT_CCP1);
enable_interrupts(INT_CCP1);
enable_interrupts(GLOBAL);

while(1)
{
if(got_pulse_width) 
     { 
      disable_interrupts(GLOBAL);  
      local_ccp_delta=ccp_delta;
      enable_interrupts(GLOBAL);
      pulse_width = local_ccp_delta; 
     // printf("\n Next:\n");
     printf("\n%lu\n\r", pulse_width);
      i = pulse_width;
      
      if (i>95&&i<129)
      {
          printf("The colour is white\n");
          output_high(PIN_C1);
          
          
      }
      
      if(i>129&&i<145)

       {
          printf("The colour is yellow\n");
          output_high(PIN_C3);
    
          
       }

if(i>145&&i<213)

       {
          printf("The colour is blue\n");
          output_high(PIN_C4);
          
       }
if(i>213&&i<240)

       {
          printf("The colour is green\n");
          output_high(PIN_B6);
       }
       
       if(i>240&&i<300)

       {
          printf("The colour is red\n");
           output_high(PIN_C5);
          
       }
if(i>300&&i<500)

       {
          printf("The colour is black\n");
       }
      
      
      got_pulse_width = FALSE; 
     } 
     
  

/*if (i>132&&i<140)
{
   printf("The colour is yellow\n");
}

if(i>335&&i<340)

{
   printf("The colour is black\n");
}

else
printf("Invalid\n");*/

  delay_ms(500); 
output_low(PIN_C1);
      output_low(PIN_C3);
      output_low(PIN_C4);
      output_low(PIN_B6);
      output_low(PIN_C5);

  

}
}
