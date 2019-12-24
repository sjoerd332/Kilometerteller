#define SET_BIT(REG, BIT)     ((REG) |= (BIT))
#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))
#define READ_BIT(REG, BIT)    ((REG) & (BIT))
#define WRITE_REG(REG, VAL)   ((REG) = (VAL))
#define READ_REG(REG)         ((REG))
#define MODIFY_REG(REG, CLEARMASK, SETMASK)  WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))

volatile uint16_t timer1Counter = 0;

ISR(TIMER1_COMPA_vect){
  if(timer1Counter >= 1000){ // count 1 to value +1
    PORTB ^= 1<<1;
    timer1Counter = 0;
  }
  timer1Counter++;
}

void setup() {
  cli();
  
  // set gpio 
  DDRB = 1 << 1; // pin 1 output
  
  // configure timer 1
  TCNT1 = 0x00; // start counting at zero
  TCCR1 |= 1 << 7; // reset on OCR1C compare
  uint8_t psc = 0b1000;// prescaler, log2(x)+1 in reg invullen
  MODIFY_REG(TCCR1,0x0F,psc);
  // write to both ORC1A and ORC1B to capture compare and reset timer
  uint8_t timerCounts = 128;// theory: 16.5 MHz / PSC / COUNTS = interrupt frequency. else PCK @64 MHz.
  OCR1A = timerCounts;
  OCR1C = timerCounts; 
  TIMSK |= 1 << 6;// timer interrupt mask register
  
  // enable interrupts
  sei();
  
}

void loop() {
  
}
