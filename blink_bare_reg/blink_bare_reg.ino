int swCount = 0;

ISR(TIMER1_COMPA_vect){
  if(swCount > 4){ // co0unt 1 to 5
    PORTB ^= 1<<1;
    swCount = 0;
  }
  swCount++;
}


void setup() {
  // set gpio 1 output
  DDRB = 1 << 1; // pin 1 output
  
  // configure timer 1
  TCCR1 |= 1 << 7; // reset on OCR1C compare
  TCCR1 |= 0b1111 << 0; // prescaler CK/16384
  // write to both ORC1A and ORC1B to capture compare and reset timer
  int timerCounts = 253;// theory: 16.5 MHz / PSC / COUNTS = 3.98 Hz
  OCR1A = timerCounts;
  OCR1C = timerCounts; 
  TIMSK |= 1 << 6;// timer interrupt mask register

  // configure pin interrupt
  
  // enable interrupts
  sei();
  
}

void loop() {
  //PORTB ^= 1<<1;
}
