#include <18f26K20.h>
#use delay( internal = 16MHz)
#fuses HS,NOWDT,NOPROTECT,NOBROWNOUT,NOPUT 
#use rs232(baud=9600, rcv=PIN_C7, xmit=PIN_C6, parity=N,errors)
#define LED PIN_A0

#define BUFFER_SIZE 12

char num_input[BUFFER_SIZE];
int1 buffer_full=FALSE;
int buffer_index=0;
char c;
char num_store 


#int_rda
void isr()
{
c=getc();
 num_input[buffer_index]=c;
 buffer_index++;
 if(buffer_index==BUFFER_SIZE)
 {
 buffer_full=TRUE;
 buffer_index=0;
 }
}

void main()
{
output_high(LED);
enable_interrupts(INT_RDA);
enable_interrupts(GLOBAL);
while(1)
{
if(buffer_full)
{

printf("%s",buf);
buffer_full=FALSE;
}
}
}

