#include<18f26k20.h>
#include<stdio.h>
#use delay(internal = 16Mhz)
#fuses HS,NOWDT,NOPROTECT,NOBROWNOUT
#use rs232(baud=9600,rcv=PIN_C7,xmit=PIN_C6,parity=N,errors)

#define TOUCH PIN_B1
#define LED PIN_A0

#INT_RB1
void isr()
{


output_high(LED);
}
void main()
{

 enable_interrupts(GLOBAL);
  enable_interrupts(INT_RB1);

while(1)
{
   if (input(TOUCH))
   {
  //    output_high(LED);
       printf("Ahoy!\n");
     //delay_ms(1000);
   }
   else if(!input(TOUCH))
   {
   //    output_low(LED);
       printf("Do it again\n");
     //delay_ms(1000);
   }
   
   else
   {
      printf("none");
   }
   
   
   
  delay_ms(1000);
}

}





