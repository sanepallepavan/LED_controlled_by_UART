#define    complete_transfer    *ucsr0a & 0x20


volatile char   *udr0    =  0xC6;
volatile char   *ubrr0h  =  0xC5;
volatile char   *ubrr0l  =  0xC4;
volatile char   *ucsr0a  =  0xC0;
volatile char   *ucsr0b  =  0xC1;
volatile char   *ucsr0c  =  0xC2;
volatile char   *dird    =  0x2A;
volatile char   *ind     =  0x29;
volatile char   *dirb    =  0x24;
volatile char   *outb    =  0x25;


volatile uint8_t receive_data;




//----------------------------------------------------------------------------------------------------------------//
//                                                   USART Functions                                              //
//----------------------------------------------------------------------------------------------------------------//

void init_USART0(){
 volatile uint16_t ubrr_value = 103;          // ubbr0 = ((clock frequency/(16*baud rate)) - 1)
                                              // ubbr0 = ((16,000,000/(16*9600)) -1) = 103
 *ubrr0h = (unsigned char)(ubrr_value >> 8);
 *ubrr0l = (unsigned char) ubrr_value;        // Select the 9600 baud rate
 *ucsr0b |= 0x98;                             // (RXCIE0 = RXEN0 = TXEN0 = 1) enable Tx, Rx and Rx complete interrupt
 *ucsr0c |= 0x0A;                             // (USBS0 = UCSZ00 = 1) set 2 stop bit & 8 data bit
}

void USART_send(volatile char data){
  while(!(complete_transfer));                // wait untill the Transfer is Completed
 *udr0 = data;
}




//----------------------------------------------------------------------------------------------------------------//
//                                                    Main Function                                               //
//----------------------------------------------------------------------------------------------------------------//

void setup() {
 *dird &= ~4;                  // set port-D 2nd pin as Input
 *dirb = 1;                    // set port-B 0th pin as Output

 init_USART0();                // Initialize the USART0

while(1){
 if((*ind & 4) == 0){                        // if switch is pressed
 USART_send('B');
 for(volatile long i=0; i<100000; i++);      // switch debouncing delay
 }

 if(receive_data == 'A'){                    // if data is received
 *outb ^= 1;                                 // toggle tha LED
 receive_data=0;
  }
 }
}




//----------------------------------------------------------------------------------------------------------------//
//                                              Interrupt Service Routine                                         //
//----------------------------------------------------------------------------------------------------------------//

ISR(USART_RX_vect){
 receive_data = *udr0;
}
