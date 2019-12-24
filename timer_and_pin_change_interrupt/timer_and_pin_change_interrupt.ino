#define SET_BIT(REG, BIT)     ((REG) |= (BIT))
#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))
#define READ_BIT(REG, BIT)    ((REG) & (BIT))
#define WRITE_REG(REG, VAL)   ((REG) = (VAL))
#define READ_REG(REG)         ((REG))
#define MODIFY_REG(REG, CLEARMASK, SETMASK)  WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))

volatile uint16_t timer1Counter = 0;
volatile bool pulseDetected = false;
volatile bool velocityTooLow = false;
volatile uint16_t timerCountsBetweenPulses = 0;
volatile uint16_t velocityEstimate = 0;
const volatile uint16_t velocityGain = 8043; // wheel circumference / counter increment time * 3.6 = 2.23 [m] * 0.001 [s] * 3.6 = 

inline uint8_t binaryOutput(uint16_t number){
  // binary counter output on pins 0,1,3-5
  // pin 0 value 2
  // pin 1 value 4
  // pin 3 value 8
  // pin 4 value 16
  // pin 5 value 32
  uint8_t val = 0;
  val |= ((number >> 1) & 1) << 0;
  val |= ((number >> 2) & 1) << 1;
  val |= ((number >> 3) & 1) << 3; // pin shift here
  val |= ((number >> 4) & 1) << 4;
  //val |= ((number >> 5) & 1) << 5; // cannot be: pin5 is reset pin, not output

  return val;
}

ISR(TIMER1_COMPA_vect){
  // blink
  if(timer1Counter >= 1000){ // count 1 to value +1
    //PORTB ^= 1<<1;
    timer1Counter = 0;
  }
  timer1Counter++;

  //odometry
  timerCountsBetweenPulses++;
  
}

// pin change interrupt
// 1. check if counter > small value (debounce)
// 2. reset counter
// 3. set sum value and output flag
ISR(INT0_vect){
  //PORTB ^= 1<<4;
  pulseDetected = true;
}

void setup() {
  cli();
  
  // set gpio 
  DDRB |= 0b111011; // output pins
  //PORTB |= 1 << 2; // pull-up input 
  
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

  // configure pin interrupt
  MCUCR |= 0b10 << 0; // 10 falling edge, 11 rising edge.
  GIMSK |= 1 << 6; //INT0 enable, PCIE disable: this is always on any change
  
  // enable interrupts
  sei(); //SREG I-bit set
}

void loop() {
  if(timerCountsBetweenPulses > 4000) // V < 2 km/h
  { 
    timerCountsBetweenPulses = 4000;
    velocityTooLow = true;
  }
  else if(timerCountsBetweenPulses < 80) // V > 100 km/h
  {timerCountsBetweenPulses = 80;}
  
  if(pulseDetected == true)
  {
    velocityEstimate = velocityGain / timerCountsBetweenPulses;
    if(velocityEstimate > 100){velocityEstimate =100;}
    if(velocityEstimate < 2){velocityEstimate = 2;}
    timerCountsBetweenPulses = 0;
    MODIFY_REG(PORTB,0b111011,binaryOutput(velocityEstimate));
    pulseDetected = false;
  }
  if(velocityTooLow == true)
  {
    MODIFY_REG(PORTB,0b111011,0);
    velocityTooLow = false;
  }

}
