#include <16F1825.h> 
#fuses INTRC_IO,NOWDT,NOPROTECT,PUT,NOLVP,NOBROWNOUT 
#use delay(crystal=4M) 
#use rs232(baud=9600, xmit=PIN_c4, rcv=PIN_c5, ERRORS, parity=N)

char c;

#int_rda
void serial_isr()
{
  c= getc();
  putc(c);
}

void main()
{
enable_interrupts(INT_RDA);
enable_interrupts(GLOBAL);
while(1);
}
