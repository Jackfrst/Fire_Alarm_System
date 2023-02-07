#include <Servo.h>
Servo myservo;
unsigned int adc;
volatile uint8_t tot_overflow;

void InitADC() {
  ADMUX = (1 << REFS1) | (1 << REFS0);                                 // For Aref=AVcc here we used the internal voltage 1.1V; 
  ADCSRA = (1 << ADEN) | (0 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);   //Rrescalar div factor = 8
}

uint16_t ReadADC(uint8_t ch) {
  //Select ADC Channel ch must be 0-7
  ch = ch & 0b00000111;
  ADMUX |= ch;

  //Start Single conversion
  ADCSRA |= (1 << ADSC);

  //Wait for conversion to complete
  while (ADCSRA & (1 << ADSC));

  return (ADC);
}

void timer1_init(){
    // set up timer with 1024 prescaling
    TCCR1B = (0 << CS12) | (1 << CS11) | (1 << CS10);
 
    // initialize counter
    TCNT1 = 0;

    TIMSK1 |= (1 << TOIE1);

    sei();

    //Number of time of overflow
    tot_overflow = 0;    
}

ISR(TIMER1_OVF_vect){
  //Using inturrupt to increase the overflow
    tot_overflow++;  
}

void setup() {
  DDRD |= ((1 << PD6 | 1 << PD7 | 1 << PD5));  
  DDRB &= ~((1 << PB1 | 1 << PB2));
  DDRB |= (1 << PB4);
  InitADC();
  timer1_init();  
  myservo.attach(8);
  myservo.write(0);
}

void loop() {

  adc = ReadADC(0);

  if((PINB & (1<< PB1)) and (PINB & (1 << PB2)) and (adc >= 500)){
    PORTD |= (1 << PD6);
    PORTD &= ~(1 << PD7);
    myservo.write(180);    
      //if(tot_overflow >=  2){
        if (TCNT1 == 200) {
            TCNT1 = 0;
            PORTD ^= (1 << PD5);
            PORTB ^= (1 << PB4);            // reset counter
        }
      //}
  }
  else{
    PORTD &= ~(1 << PD6);
    PORTD |= (1 << PD7);
    PORTB &= ~(1 << PB4);
    PORTD &= ~(1 << PD5);
    myservo.write(0);    
  }
}
