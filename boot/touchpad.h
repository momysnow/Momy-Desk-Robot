#define THRES 15
#define SAMPLES 50

typedef struct {
  int pin;
  int unpValue;
  int value;
  int state;
  int prevState;
  int toggleState;
} touchPad;

void touchPadInit(touchPad *pad, int pin){
  pad->pin=pin;
  pad->unpValue = (sampleB(pin) - sampleA(pin));
  DIDR0 |= 1;
  DIDR0 |= 1<<(pin-A0);
}

int sampleA(int sensePin){
  /* Sample capacitor is charged to VCC
   * via A0 pull-up resistor, touch pad
   * is discharged by pulling pin low
   */
  ADMUX = 0b01000000;
  pinMode(sensePin, OUTPUT);
  digitalWrite(sensePin, 0);
  
  pinMode(sensePin, INPUT);
  ADMUX = 0b01000000 | sensePin-A0;

  ADCSRA |= 1<<ADSC;
  while((ADCSRA & (1<<ADSC)) != 0);
  
  return ADC;
}

int sampleB(int sensePin){
  /* Sample capacitor is discharged by selecting
   * GND as ADC input, touch pad is charged to VCC
   * via pin pull-up resistor
   */
  ADMUX = 0b01001111;
  pinMode(sensePin, INPUT_PULLUP);
  
  pinMode(sensePin, INPUT);
  ADMUX = 0b01000000 | sensePin-A0;

  ADCSRA |= 1<<ADSC;
  while((ADCSRA & (1<<ADSC)) != 0);
  
  return ADC;
}

void touchPadScan(touchPad *pad){
  static float A, B;

  A=0;
  B=0;

  /* Get some readings from sensor and calculate average */
  for(int i=0; i<SAMPLES; i++){
    A += sampleA(pad->pin);
    B += sampleB(pad->pin);
  }

  A /= SAMPLES;
  B /= SAMPLES;
  
  pad->value = (B - A);

  /* Member unpValue is a running average of the unpressed readings.
   * A sudden change in sensor reading is interpreted as a touch
   */
  if(pad->value > (pad->unpValue + THRES))
    pad->state=1;
  else{
    pad->state=0;
    pad->unpValue=((float)pad->unpValue*0.9)+((float)pad->value*0.1);
  }
  
  if(pad->state == 1 && pad->prevState == 0)
    pad->toggleState = !pad->toggleState;

  pad->prevState = pad->state;
}